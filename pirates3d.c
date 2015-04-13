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

typedef enum { Rotation, Scale, Translate } pirates3d_transform_type ;

struct pirates3d_entity_transform_s { pirates3d_transform_type transform ; RKMath_NewVector(Transform, 4) ; }  ;

typedef struct pirates3d_entity_transform_s* pirates3d_entity_transform ;

struct pirates3d_camera_s { int active ; RKMath_NewVector(Position, 3) ; RKMath_NewVector(Focus, 3) ; } ;

struct pirates3d_primitive_array_s { pirates3d_primitive_type primitive_type ; int num_of_primitives ; int size ;
    
pirates_primitive_array primitive_array ; } ;

struct pirates3d_scene_s { pirates3d_camera camera ; pirates_scene scene_rt ; }  ;

pirates3d_scene pirates3d_new_3dscene( pirates_scene scene_rt ) {
    
    pirates3d_scene scene3d = RKMem_NewMemOfType(struct pirates3d_scene_s) ;
    
    scene3d->scene_rt = scene_rt ;
    
    return scene3d ;
}

void pirates3d_destroy_3dscene( pirates3d_scene scene3d ) {
    
    pirates_proc_scene(scene3d->scene_rt) ;
    
    pirates_destroy_scene(scene3d->scene_rt) ;
}

void pirates3d_draw_3dscene( pirates3d_scene scene3d ) {
    
    pirates_change_camera(scene3d->scene_rt, scene3d->camera->Position[RKM_X], scene3d->camera->Position[RKM_Y], scene3d->camera->Position[RKM_Z], scene3d->camera->Focus[RKM_X], scene3d->camera->Focus[RKM_Y], scene3d->camera->Focus[RKM_Z]) ;
    
    pirates_proc_scene(scene3d->scene_rt) ;
    
    pirates_render(scene3d->scene_rt) ;
    
    pirates_destroy_bins(scene3d->scene_rt) ;
}

static void pirates3d_submit_primitive_array( pirates_scene scene, pirates3d_primitive_type primitive_type, pirates_primitive_array primitive_array, int num_of_primitives, int can_and_should_delete ) {
    
    switch (primitive_type) {
            
        case Triangles:
            
            pirates_add_triangle_array(scene,primitive_array,num_of_primitives,can_and_should_delete) ;
            
            break;
            
        default:
            break;
    }
}

int pirates3d_get_primitive_size( pirates3d_primitive_type primitive_type ) {
    
    int size = 0 ;
    
    switch (primitive_type) {
        
    case Triangles:
        
        size = pirates_triangle_size() ;
        
        break;
        
    default:
        break;
    }
    
    return size ;
}

pirates3d_primitive_array pirates3d_new_primitive_array( pirates3d_scene scene3d, void* primitive_data, pirates3d_primitive_type primitive_type, pirates3d_copy_mode copy_mode, int num_of_primitives ) {
    
    pirates3d_primitive_array primitive_array = RKMem_NewMemOfType(struct pirates3d_primitive_array_s) ;

    int size = pirates3d_get_primitive_size(primitive_type) * num_of_primitives ;
    
    RKMVector primitive = NULL ;
    
    if ( copy_mode == pirates3d_copy_data ) {
        
        primitive = RKMath_NewVectorMalloc( size ) ;
        
        RKMath_Equal(primitive, (RKMVector)primitive_data, size) ;
    }
    
    if ( copy_mode == (pirates3d_dont_copy_data || pirates3d_dont_copy_but_delete_data) ) {
        
        primitive = (RKMVector)primitive_data ;
    }
    
    primitive_array->primitive_array = pirates_new_primitive_array(primitive) ;
    
    primitive_array->num_of_primitives = num_of_primitives ;
    
    primitive_array->primitive_type = primitive_type ;
    
    primitive_array->size = size ;
    
    pirates3d_submit_primitive_array(scene3d->scene_rt,primitive_array->primitive_type,primitive_array->primitive_array,primitive_array->num_of_primitives,copy_mode) ;
    
    return primitive_array ;
}

void pirates3d_destroy_primitive_array( pirates3d_primitive_array primitive_array ) {
    
    pirates_destroy_primitive_array(primitive_array->primitive_array) ;
    
    free(primitive_array) ;
}
