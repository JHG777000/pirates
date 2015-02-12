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
    
    RKTasks_StartLock(new_clover_ray_object->raylock) ;
    
    return new_clover_ray_object ;
}

void pirates_kill_clover_ray_object( pirates_ray_object_ref clover_ray_object ) {
    
    free( clover_ray_object->material ) ;
    
    free( clover_ray_object->spheres ) ;
    
    RKTasks_EndLock( clover_ray_object->raylock ) ;
    
    free( clover_ray_object ) ;
    
}