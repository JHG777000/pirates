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

typedef struct { pirates_bounding_box bounding_box ; pirates_spheres sphere_array ; int numspheres ; int group_id ;  } pirates_group_object ;

typedef pirates_group_object* pirates_group ;

#endif /* defined(__pirates__clover__) */
