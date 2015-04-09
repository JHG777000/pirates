//
//  pirates3d.c
//  IDKApp
//
//  Created by Jacob Gordon on 4/6/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "pirates3d.h"

struct pirates3d_entity_camera_s { pirates3d_entity_type entity_type ; RKMath_NewVector(Position, 3) ; RKMath_NewVector(Focus, 3) ; } ;

typedef struct pirates3d_entity_camera_s* pirates3d_entity_camera ;

typedef struct { RKMath_NewVector(Transform, 4) ; } transform_type ;

struct { pirates3d_entity_type entity_type ; transform_type transform ; transform_type final_transform ; } pirates3d_entity_transform_s ;

typedef struct pirates3d_entity_transform_s* pirates3d_entity_transform ;

struct { pirates3d_entity_type entity_type ; int m_id ; } pirates3d_entity_material_s ;

typedef struct pirates3d_entity_material_s* pirates3d_entity_material ;

typedef void (*pirates3d_proc_geom_func)(void* geom, void* shape) ;

struct pirates3d_entity_geom_s { pirates3d_entity_type entity_type ; pirates3d_primitive_type primitive_type ;
    
pirates3d_entity_material material ; void* geom ; void* shape ; pirates3d_proc_geom_func proc_geom_func ; } ;

typedef struct pirates3d_entity_geom_s* pirates3d_entity_geom ;

struct pirates3d_entity_s { pirates3d_entity_type entity_type ; } ;

struct pirates3d_scene_s { pirates_scene scene_rt ; }  ;