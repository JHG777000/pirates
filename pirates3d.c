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

struct pirates3d_transform_entity_s { pirates3d_transform_type transform ; RKMath_NewVector(Transform, 4) ; }  ;

typedef struct pirates3d_transform_entity_s* pirates3d_transform_entity ;

struct pirates3d_camera_s { int active ; RKMath_NewVector(Position, 3) ; RKMath_NewVector(Focus, 3) ; } ;

struct pirates3d_primitive_array_s { pirates3d_primitive_type primitive_type ; int num_of_primitives ; int size ;
    
pirates_primitive_array primitive_array ; } ;

struct pirates3d_scene_s { pirates3d_camera camera ; pirates_scene scene_rt ; }  ;

pirates3d_transform pirates3d_new_transform( void ) {
    
    return RKList_NewList() ;
}

static void pirates3d_apply_transform_to_transform_list( pirates3d_transform_entity transform, pirates3d_transform transform_list) {
    
    if ( transform->transform == Rotation ) {
        
        RKMath_NewVector(AxisAngle, 4) ;
        
        RKMath_Equal(AxisAngle, transform->Transform, 4) ;
        
        double radians = (AxisAngle[3] / 180) * M_PI;
        
        float value = sin( radians / 2 ) ;
        
        RKMath_Vectorthat(value_vec, value) ;
        
        RKMath_Equal(transform->Transform, value_vec, 3) ;
        
        RKMath_Mul(transform->Transform, transform->Transform, AxisAngle, 3) ;
        
        transform->Transform[3] = cos( radians / 2 ) ;
    }
    
    if ( RKList_GetNumOfNodes(transform_list) == 0 ) {
        
        RKList_AddToList(transform_list, (void*)transform) ;
        
        return ;
    }
    
    pirates3d_transform_entity old_transform = (pirates3d_transform_entity)RKList_GetData(RKList_GetLastNode(transform_list)) ;
    
    if ( transform->transform == old_transform->transform ) {
        
        RKMVector v0 = old_transform->Transform ;
        
        RKMVector v1 = transform->Transform ;
        
        switch (transform->transform) {
                
            case Rotation:
                
                v0[0] = v0[3] * v1[0] + v0[0] * v1[3] + v0[1] * v1[2] - v0[2] * v1[1];
                v0[1] = v0[3] * v1[1] + v0[1] * v1[3] + v0[2] * v1[0] - v0[0] * v1[2];
                v0[2] = v0[3] * v1[2] + v0[2] * v1[3] + v0[0] * v1[1] - v0[1] * v1[0];
                v0[3] = v0[3] * v1[3] - v0[0] * v1[0] - v0[1] * v1[1] - v0[2] * v1[2];
                
                RKMath_Norm(v0, v0, 4) ;
                
                break;
                
            case Scale:
                
                RKMath_Mul(v0, v0, v1, 3) ;
                
                break;
                
            case Translate:
                
                RKMath_Add(v0, v0, v1, 3) ;
                
                break;
                
            default:
                break;
        }
        
        
    } else {
        
        RKList_AddToList(transform_list, (void*)transform) ;
    }
}

void pirates3d_add_transform( pirates3d_transform transform, pirates3d_transform_type transform_type, float x, float y, float z, float w ) {
    
    pirates3d_transform_entity transform_entity = RKMem_NewMemOfType(struct pirates3d_transform_entity_s) ;
    
    transform_entity->transform = transform_type ;
    
    transform_entity->Transform[0] = x ;
    
    transform_entity->Transform[1] = y ;
    
    transform_entity->Transform[2] = z ;
    
    transform_entity->Transform[3] = w ;
    
    pirates3d_apply_transform_to_transform_list(transform_entity,transform) ;
}

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
