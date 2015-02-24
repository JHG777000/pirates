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

clover_context pirates_new_clover_context(pirates_scene scene, int max_group_tasks, int max_ray_tasks ) {
    
    clover_context clover = RKMem_NewMemOfType(clover_context_object) ;
    
    clover->max_group_tasks = max_group_tasks ;
    
    clover->max_ray_tasks = max_ray_tasks ;
    
    clover->list_for_groups = NULL ;
    
    clover->list_for_rays = NULL ;
    
    clover->kill_this_task = 0 ;
    
    RKTasks_StartLock(clover->clover_context_lock) ;
    
    RKTasks_StartLock(clover->group_list_lock) ;
    
    RKTasks_StartLock(clover->ray_list_lock) ;
    
    RKTasks_StartLock(clover->kill_lock) ;
    
    return clover ;
    
}

RKTasks_CreateTask(clover_group_task, clover_context clover;,
    
  RKList groups = (*RKTArgs->clover->list_for_groups) ;
                   
  RKList rays = (*RKTArgs->clover->list_for_rays) ;
  
  RKList_node group = NULL ;
                   
  RKList_node ray = NULL ;
  
  group = RKList_GetFirstNode(groups) ;
                   
  while ( group != NULL ) {
     
   ray = RKList_GetFirstNode(rays) ;
      
      while ( ray != NULL ) {
          
          
          ray = RKList_GetNextNode(ray) ;
      }
      
      group = RKList_GetNextNode(group) ;
  }
                   
                   
) ;

void clover_make_tasks( clover_context clover ) {
    
    clover->clover_threads = RKTasks_NewThreadGroup(1, 10, 4, 1, 7) ;
    
    clover->clover_tasks = RKTasks_NewTaskGroup() ;
    
    RKTasks_BindTaskGroupToThreadGroup(clover->clover_tasks, clover->clover_threads) ;
    
    RKTasks_RunThreadGroup(clover->clover_threads) ;
    
    RKTasks_UseTaskGroup(clover->clover_tasks) ;
    
    RKTasks_Args(clover_group_task) ;
    
    int i = 0 ;
    
    int numtasks = MIN_JHG(clover->max_group_tasks, RKList_GetNumOfNodes((*clover->list_for_groups))) ;
    
    while (i < numtasks) {
        
        RKTasks_UseArgs(clover_group_task) ;
        
        clover_group_task_Args->clover = clover ;
        
        RKTasks_AddTask(clover->clover_tasks, clover_group_task, clover_group_task_Args) ;
        
        i++ ;
    }
    
    RKTasks_WaitForTasksToBeDone(clover->clover_tasks) ;
}

void clover_add_more_tasks( clover_context clover ) {
    
    RKTasks_Args(clover_group_task) ;
    
    int i = 0 ;
    
    int numtasks = MIN_JHG(clover->max_group_tasks - RKTasks_GetNumOfTasks(clover->clover_tasks), RKList_GetNumOfNodes((*clover->list_for_groups))) ;
    
    while (i < numtasks) {
        
        RKTasks_UseArgs(clover_group_task) ;
        
        clover_group_task_Args->clover = clover ;
        
        RKTasks_AddTask(clover->clover_tasks, clover_group_task, clover_group_task_Args) ;
        
        i++ ;
    }
    
}

void clover_restart_tasks( clover_context clover ) {
    
    RKTasks_UseTaskGroup(clover->clover_tasks) ;
    
    if ( RKList_GetNumOfNodes((*clover->list_for_groups)) > RKTasks_GetNumOfTasks(clover->clover_tasks) ) clover_add_more_tasks(clover) ;
    
    RKTasks_WaitForTasksToBeDone(clover->clover_tasks) ;
}

void clover_end_tasks( clover_context clover ) {
    
    RKTasks_WaitForTasksToBeDone(clover->clover_tasks) ;
    
    RKTasks_KillThreadGroup(clover->clover_threads) ;
    
    RKTasks_KillTaskGroup(clover->clover_tasks) ;

}

void clover_end_lists( clover_context clover ) {
    
    RKTasks_LockLock(clover->ray_list_lock) ;
    
    RKList_DeleteList(*(clover->list_for_rays)) ;
    
    RKTasks_UnLockLock(clover->ray_list_lock) ;
    
    RKTasks_LockLock(clover->group_list_lock) ;
    
    RKList_node node = RKList_GetFirstNode(*(clover->list_for_groups)) ;
    
    pirates_group group = NULL ;
    
    while ( node != NULL ) {
        
        group = (pirates_group)RKList_GetData(node) ;
        
        free(group->sphere_array) ;
        
        free(group) ;
        
        node = RKList_GetNextNode(node) ;
    }
    
    RKTasks_UnLockLock(clover->group_list_lock) ;
}

void clover_end_context( clover_context clover ) {
    
    clover_end_tasks(clover) ;
    
    clover_end_lists(clover) ;
    
    RKTasks_EndLock(clover->kill_lock) ;
    
    RKTasks_EndLock(clover->clover_context_lock) ;
    
    RKTasks_EndLock(clover->group_list_lock) ;
    
    RKTasks_EndLock(clover->ray_list_lock) ;
}

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

void pirates_compute_group_bounding_box( pirates_group group, pirates_bounding_box bounding_box ) {
    
    if ( bounding_box.X > group->bounding_box.X ) group->bounding_box.X = bounding_box.X ;
    
    if ( bounding_box.x < group->bounding_box.x ) group->bounding_box.x = bounding_box.x ;
    
    if ( bounding_box.Y > group->bounding_box.Y ) group->bounding_box.Y = bounding_box.Y ;
    
    if ( bounding_box.y < group->bounding_box.y ) group->bounding_box.y = bounding_box.y ;
    
    if ( bounding_box.Z > group->bounding_box.Z ) group->bounding_box.Z = bounding_box.Z ;
    
    if ( bounding_box.z < group->bounding_box.z ) group->bounding_box.z = bounding_box.z ;
}

void pirates_add_sphere_to_group( pirates_sphere sphere, pirates_group group ) {
    
    pirates_add_sphere(&(group->sphere_array), &(group->numspheres), sphere) ;
    
    pirates_compute_group_bounding_box(group, sphere->bounding_box) ;
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
    
    if ( numspheres <= clover->max_group_tasks ) {
    
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

        
    } else {
        
        while ( numspheres > 0 ) {
            
            i = 0 ;
            
            while ( i < clover->max_group_tasks ) {
                
                pirates_make_group(clover,scene->sphere_array[i],j) ;
                
                i++ ;
            }
            
            j++ ;
            
            if ( j >= clover->max_group_tasks ) j = 0 ;
            
            numspheres -= clover->max_group_tasks ;
        }
        
    }
}