//
//  pirates3d.h
//  IDKApp
//
//  Created by Jacob Gordon on 4/6/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#ifndef __IDKApp__pirates3d__
#define __IDKApp__pirates3d__

#include <stdio.h>
#include "pirates.h"

typedef const char* pirates3d_material_label ;

typedef int pirates3d_material ;

typedef enum { pirates3d_copy_data = 1, pirates3d_dont_copy_data = 0, pirates3d_dont_copy_but_delete_data = 1 } pirates3d_copy_mode ;

typedef enum { Rotation, Scale, Translate } pirates3d_transform_type ;

typedef enum { Triangles } pirates3d_primitive_type ;

typedef int pirates3d_material ;

typedef RKList pirates3d_transform ;

typedef struct pirates3d_primitive_array_s* pirates3d_primitive_array ;

typedef struct pirates3d_camera_s* pirates3d_camera ;

typedef struct pirates3d_scene_s* pirates3d_scene ;

pirates3d_camera pirates3d_new_camera( float x, float y, float z, float fx, float fy, float fz ) ;

void pirates3d_set_camera_to_active( pirates3d_scene scene3d, pirates3d_camera camera ) ;

int pirates3d_is_camera_active( pirates3d_camera camera ) ;

pirates3d_material pirates3d_new_material( pirates3d_scene scene3d, pirates3d_material_label label, float red, float blue, float green ) ;

pirates3d_material pirates3d_get_material( pirates3d_scene scene3d, pirates3d_material_label label ) ;

pirates3d_material pirates3d_change_material( pirates3d_scene scene3d, pirates3d_material_label label, float red, float blue, float green ) ;

pirates3d_scene pirates3d_new_3dscene( pirates3d_camera camera, pirates_scene scene_rt ) ;

void pirates3d_destroy_3dscene( pirates3d_scene scene3d ) ;

void pirates3d_draw_3dscene( pirates3d_scene scene3d ) ;

pirates3d_transform pirates3d_new_transform( void ) ;

void pirates3d_add_transform( pirates3d_transform transform, pirates3d_transform_type transform_type, float x, float y, float z, float w ) ;

void pirates3d_delete_last_transform_set( pirates3d_transform transform ) ;

void pirates3d_apply_transform( pirates3d_transform transform, pirates3d_primitive_array primitive_array ) ;

int pirates3d_get_primitive_size( pirates3d_primitive_type primitive_type ) ;

pirates3d_primitive_array pirates3d_new_primitive_array( pirates3d_scene scene3d, void* primitive_data, pirates3d_primitive_type primitive_type, pirates3d_copy_mode copy_mode, int num_of_primitives ) ;

void pirates3d_destroy_primitive_array( pirates3d_primitive_array primitive_array ) ;

#endif /* defined(__IDKApp__pirates3d__) */
