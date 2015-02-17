//
//  clover.h
//  pirates
//
//  Created by Jacob Gordon on 2/1/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#ifndef __pirates__clover__
#define __pirates__clover__

#include "pirates.h"

typedef struct clover_ray_s* pirates_ray_object_ref ;

typedef void (*pirates_ray_eval_func)(pirates_ray_object_ref ray, pirates_scene scene, pirates_sphere sphere) ;

typedef struct clover_ray_s { Ray ray ; cn_point pixel ; pirates_Material material ; pirates_ray_eval_func ray_eval_func ; struct clover_ray_s* origin_ray ;
    
int num_of_spawned_rays ; int depth ; int* spheres ; int numspheres ; int run_state ; RKT_Lock raylock ; } clover_ray_object ;

typedef struct { pirates_bounding_box bounding_box ; pirates_spheres sphere_array ; int numspheres ; int group_id ; } pirates_group_object ;

typedef pirates_group_object* pirates_group ;

typedef RKList* raylist ;

typedef RKList* grouplist ;

typedef struct { pirates_scene scene ; raylist list_for_rays ; RKT_Lock ray_list_lock ; grouplist list_for_groups ;

RKT_Lock group_list_lock ; int kill_this_task ; RKT_Lock kill_lock ; int max_group_tasks ; int max_ray_tasks ;
    
RKT_Lock clover_context_lock ; RKTasks_ThreadGroup clover_threads ; RKTasks_TaskGroup clover_tasks ; } clover_context_object ;

typedef clover_context_object* clover_context ;

clover_context pirates_new_clover_context(pirates_scene scene, int max_group_tasks, int max_ray_tasks )  ;

#endif /* defined(__pirates__clover__) */
