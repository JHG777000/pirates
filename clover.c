//
//  clover.c
//  pirates
//
//  Created by Jacob Gordon on 2/1/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "clover.h"

pirates_ray_object_ref pirates_new_clover_ray_object(RKMVector origin , RKMVector direction, cn_point pixel, pirates_ray_eval_func ray_eval_func, pirates_ray_object_ref origin_ray, int depth) {
    
    pirates_ray_object_ref new_clover_ray_object = RKMem_NewMemOfType(clover_ray_object) ;
    
    new_clover_ray_object->ray = newray(origin, direction) ;
    
    new_clover_ray_object->pixel = pixel ;
    
    new_clover_ray_object->ray_eval_func = ray_eval_func ;
    
    new_clover_ray_object->origin_ray = origin_ray ;
    
    new_clover_ray_object->depth = depth ;
    
    new_clover_ray_object->material = NULL ;
    
    new_clover_ray_object->num_of_spawned_rays = 0 ;
    
    new_clover_ray_object->spheres = NULL ;
    
    new_clover_ray_object->numspheres = 0 ;
    
    new_clover_ray_object->run_state = 0 ;
    
    RKTasks_StartLock(new_clover_ray_object->raylock) ;
    
    return new_clover_ray_object ;
}

void pirates_kill_clover_ray_object( pirates_ray_object_ref clover_ray_object ) {
    
    free( clover_ray_object->material ) ;
    
    free( clover_ray_object->spheres ) ;
    
    RKTasks_EndLock( clover_ray_object->raylock ) ;
    
    free( clover_ray_object ) ;
    
}

void pirates_submit_ray( clover_context clover, RKMVector origin , RKMVector direction, cn_point pixel, pirates_ray_eval_func ray_eval_func, pirates_ray_object_ref origin_ray, int depth ) {
    
    pirates_ray_object_ref newray = pirates_new_clover_ray_object(origin,direction,pixel,ray_eval_func,origin_ray,depth) ;
    
    if ( (*clover->list_for_rays) == NULL ) (*clover->list_for_rays) = RKList_NewList() ;
    
    RKList_AddToList((*clover->list_for_rays), (void*)newray) ;
}

void pirates_add_sphere_to_group( pirates_sphere sphere, pirates_group group ) {
    
    pirates_add_sphere(&(group->sphere_array), &(group->numspheres), sphere) ;
}

pirates_group pirates_new_group( pirates_sphere sphere ) {
    
    pirates_group newgroup = RKMem_NewMemOfType(pirates_group_object) ;
    
    newgroup->sphere_array = NULL ;
    
    newgroup->numspheres = 0 ;
    
    pirates_add_sphere_to_group(sphere,newgroup) ;
    
    return newgroup ;
}

void pirates_make_group( clover_context clover, pirates_sphere sphere, int index) {
    
    if ( (*clover->list_for_groups) == NULL ) (*clover->list_for_groups) = RKList_NewList() ;
    
    RKList_node node = RKList_GetNode((*clover->list_for_groups), index) ;
    
    if ( node == NULL ) {
        
        RKList_AddToList((*clover->list_for_groups), (void*)pirates_new_group(sphere)) ;
        
    } else {

        pirates_add_sphere_to_group(sphere, (pirates_group)RKList_GetData(node)) ;
    }
}

void pirates_make_groups( clover_context clover ) {
    
    pirates_scene scene = clover->scene ;
    
    int numspheres = scene->numspheres ;
    
    int i = 0 ;
    
    int j = 0 ;
    
    while ( numspheres > 0 ) {
    
      i = 0 ;
        
      while ( i < clover->max_group_tasks ) {
        
          pirates_make_group(clover,scene->sphere_array[j],i) ;
         
          i++ ;
         
          j++ ;
         
          if ( j >= numspheres ) break ;
      }
        
       numspheres -= clover->max_group_tasks ;
    }

}