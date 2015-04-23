//
//  pirates3d.c
//  IDKApp
//
//  Created by Jacob Gordon on 4/6/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pirates3d.h"

#define PR_MAX_PR_LETTER_NUM 65

typedef const char* pr_string ;

typedef struct pr_letter_s { int m_id ; struct pr_letter_s* next_alphabet ; } pr_letter ;

struct pirates3d_transform_entity_s { pirates3d_transform_type transform ; RKMath_NewVector(Transform, 4) ; }  ;

typedef struct pirates3d_transform_entity_s* pirates3d_transform_entity ;

struct pirates3d_camera_s { int active ; RKMath_NewVector(Position, 3) ; RKMath_NewVector(Focus, 3) ; } ;

struct pirates3d_primitive_array_s { pirates3d_primitive_type primitive_type ; int num_of_primitives ; int size ;
    
pirates_primitive_array primitive_array ; } ;

struct pirates3d_scene_s { pirates3d_camera camera ; pr_letter* pirates3d_material_library ; pirates_scene scene_rt ; }  ;

pirates3d_camera pirates3d_new_camera( float x, float y, float z, float fx, float fy, float fz ) {
    
    pirates3d_camera new_cam = RKMem_NewMemOfType(struct pirates3d_camera_s) ;
    
    RKMath_Vectorit(position, x, y, z) ;
    
    RKMath_Vectorit(focus, fx, fy, fz) ;
    
    RKMath_Equal(new_cam->Position, position, 3) ;
    
    RKMath_Equal(new_cam->Focus, focus, 3) ;
    
    return new_cam ;
}

void pirates3d_change_camera( pirates3d_camera  camera, float x, float y, float z, float fx, float fy, float fz ) {
    
    RKMath_Vectorit(position, x, y, z) ;
    
    RKMath_Vectorit(focus, fx, fy, fz) ;
    
    RKMath_Equal(camera->Position, position, 3) ;
    
    RKMath_Equal(camera->Focus, focus, 3) ;
}

void pirates3d_destroy_camera( pirates3d_camera  camera ) {
    
    free(camera) ;
}

void pirates3d_set_camera_to_active( pirates3d_scene scene3d, pirates3d_camera camera ) {
    
    scene3d->camera->active = 0 ;
    
    scene3d->camera = camera ;
    
    camera->active = 1 ;
}

int pirates3d_is_camera_active( pirates3d_camera camera ) {
    
    return camera->active ;
}

static int PR_LookUpCharID( char c ) {
    
    switch (c) {
        case 'A':
            
            return 1 ;
            
            break;
        case 'B':
            
            return 2 ;
            
            break;
        case 'C':
            
            return 3 ;
            
            break;
        case 'D':
            
            return 4 ;
            
            break;
        case 'E':
            
            return 5 ;
            
            break;
        case 'F':
            
            return 6 ;
            
            break;
        case 'G':
            
            return 7 ;
            
            break;
        case 'H':
            
            return 8 ;
            
            break;
        case 'I':
            
            return 9 ;
            
            break;
        case 'J':
            
            return 10 ;
            
            break;
        case 'K':
            
            return 11 ;
            
            break;
        case 'L':
            
            return 12 ;
            
            break;
        case 'M':
            
            return 13 ;
            
            break;
        case 'N':
            
            return 14 ;
            
            break;
        case 'O':
            
            return 15 ;
            
            break;
        case 'P':
            
            return 16 ;
            
            break;
        case 'Q':
            
            return 17 ;
            
            break;
        case 'R':
            
            return 18 ;
            
            break;
        case 'S':
            
            return 19 ;
            
            break;
        case 'T':
            
            return 20 ;
            
            break;
        case 'U':
            
            return 21 ;
            
            break;
        case 'V':
            
            return 22 ;
            
            break;
        case 'W':
            
            return 23 ;
            
            break;
        case 'X':
            
            return 24 ;
            
            break;
        case 'Y':
            
            return 25 ;
            
            break;
        case 'Z':
            
            return 26 ;
            
            break;
        case '.':
            
            return 27 ;
            
            break;
            
        case 'a':
            
            return 28 ;
            
            break;
        case 'b':
            
            return 29 ;
            
            break;
        case 'c':
            
            return 30 ;
            
            break;
        case 'd':
            
            return 31 ;
            
            break;
        case 'e':
            
            return 32 ;
            
            break;
        case 'f':
            
            return 33 ;
            
            break;
        case 'g':
            
            return 34 ;
            
            break;
        case 'h':
            
            return 35 ;
            
            break;
        case 'i':
            
            return 36 ;
            
            break;
        case 'j':
            
            return 37 ;
            
            break;
        case 'k':
            
            return 38 ;
            
            break;
        case 'l':
            
            return 39 ;
            
            break;
        case 'm':
            
            return 40 ;
            
            break;
        case 'n':
            
            return 41 ;
            
            break;
        case 'o':
            
            return 42 ;
            
            break;
        case 'p':
            
            return 43 ;
            
            break;
        case 'q':
            
            return 44 ;
            
            break;
        case 'r':
            
            return 45 ;
            
            break;
        case 's':
            
            return 46 ;
            
            break;
        case 't':
            
            return 47 ;
            
            break;
        case 'u':
            
            return 48 ;
            
            break;
        case 'v':
            
            return 49 ;
            
            break;
        case 'w':
            
            return 50 ;
            
            break;
        case 'x':
            
            return 51 ;
            
            break;
        case 'y':
            
            return 52 ;
            
            break;
        case 'z':
            
            return 53 ;
            
            break;
        case '0':
            
            return 54 ;
            
            break;
            
        case '1':
            
            return 55 ;
            
            break;
            
        case '2':
            
            return 56 ;
            
            break;
            
        case '3':
            
            return 57 ;
            
            break;
            
        case '4':
            
            return 58 ;
            
            break ;
            
        case '5':
            
            return 59 ;
            
            break;
            
        case '6':
            
            return 60 ;
            
            break;
            
        case '7':
            
            return 61 ;
            
            break;
            
        case '8':
            
            return 62 ;
            
            break;
            
        case '9':
            
            return 63 ;
            
            break;
            
        case '-':
            
            return 64 ;
            
            break;
            
        case '_':
            
            return 65 ;
            
            break;
            
        default:
            
            return 0 ;
            
            break;
    }
    
    return 0 ;
    
}

static pr_letter* PR_NewAlphabet( void ) {
    
    pr_letter* pr_alphabet = NULL ;
    
    int size = PR_MAX_PR_LETTER_NUM + 1 ;
    
    pr_alphabet = RKMem_CArray(size, pr_letter) ;
    
    int i = 0 ;
    
    while ( i < size ) {
        
        pr_alphabet[i].m_id = 1 ;
        
        pr_alphabet[i].next_alphabet = NULL ;
        
        i++ ;
    }
    
    return pr_alphabet ;
    
}

static int PR_LookUpTypeID( pirates3d_scene scene3d, int* buffer, int size, int m_id ) {
    
    pr_letter* current_alphabet = NULL ;
    
    int i = 0 ;
    
    int value = 0 ;
    
    if ( scene3d->pirates3d_material_library == NULL ) {
        
        scene3d->pirates3d_material_library = PR_NewAlphabet() ;
    }
    
    current_alphabet = scene3d->pirates3d_material_library ;
    
    while ( i < size ) {
        
        value = buffer[i] ;
        
        if ( value != 0 ) {
            
            if ( i == ( size - 1 ) ) {
                
                if ( m_id == -1 ) {
                    
                    return current_alphabet[value].m_id ;
                    
                } else {
                    
                    current_alphabet[value].m_id = m_id ;
                    
                    return current_alphabet[value].m_id ;

                }
                
            } else {
                
                if ( current_alphabet[value].next_alphabet == NULL ) current_alphabet[value].next_alphabet = PR_NewAlphabet() ;
                
                current_alphabet = current_alphabet[value].next_alphabet ;
                
            }
            
        }
        
        i++ ;
    }
    
    return 0 ;
}

static int PR_GetSetM_ID( pirates3d_scene scene3d, pr_string symbol, int m_id ) {
    
    int buffer[100] ;
    
    int strilen  = 0 ;
    int i = 0 ;
    
    strilen = (int) strlen( symbol ) ;
    
    if ( strilen > 100 ) strilen = 100 ;
    
    while (i < strilen) {
        
        buffer[i] = PR_LookUpCharID( symbol[i] ) ;
        
        i++ ;
    }
    
    return PR_LookUpTypeID( scene3d, buffer, strilen, m_id ) ;
    
}

static void pirates3d_apply_material_to_triangles( pirates3d_material material, pirates3d_primitive_array primitive_array ) {
    
    int i = 0 ;
    
    pirates_triangles triangles = NULL ;
    
    pirates_triangle triangle = NULL ;
    
    triangles = (pirates_triangles)primitive_array->primitive_array[0] ;
    
    while ( i < primitive_array->num_of_primitives ) {
        
        triangle = pr_gettriangle(triangles, i) ;
        
        triangle[pr_M] = material ;
        
        i++ ;
    }
}

void pirates3d_apply_material( pirates3d_material material, pirates3d_primitive_array primitive_array )  {
    
    switch (primitive_array->primitive_type) {
            
        case Triangles:
            pirates3d_apply_material_to_triangles(material,primitive_array) ;
            break;
            
        default:
            break;
    }
    
}

pirates3d_material pirates3d_new_material( pirates3d_scene scene3d, pirates3d_material_label label, float red, float blue, float green ) {
    
    pirates_Material material = pirates_newmaterial(Colorit(red, blue, green)) ;
    
    int m_id = pirates_addmaterial(scene3d->scene_rt, material) ;
    
    return PR_GetSetM_ID(scene3d, label, m_id) ;
}

pirates3d_material pirates3d_get_material( pirates3d_scene scene3d, pirates3d_material_label label ) {
    
    return PR_GetSetM_ID(scene3d, label, -1) ;
}

pirates3d_material pirates3d_change_material( pirates3d_scene scene3d, pirates3d_material material, float red, float blue, float green ) {
    
    pirates_Material _material_ = pirates_get_material_with_id(scene3d->scene_rt, material) ;
    
    _material_->color.r = red ;
    
    _material_->color.b = blue ;
    
    _material_->color.g = green ;
    
    return material ;
}

pirates3d_material pirates3d_change_material_with_label( pirates3d_scene scene3d, pirates3d_material_label label, float red, float blue, float green ) {
    
    int m_id = PR_GetSetM_ID(scene3d, label, -1) ;
    
    pirates_Material material = pirates_get_material_with_id(scene3d->scene_rt, m_id) ;
    
    material->color.r = red ;
    
    material->color.b = blue ;
    
    material->color.g = green ;
    
    return m_id ;
}

pirates3d_transform pirates3d_new_transform( void ) {
    
    return RKList_NewList() ;
}

static void pirates3d_apply_transform_to_transform_list( pirates3d_transform_entity transform, pirates3d_transform transform_list, pirates3d_transform_mode mode) {
    
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
    
    if ( mode == replace ) {
        
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

void pirates3d_add_transform( pirates3d_transform transform, pirates3d_transform_type transform_type, pirates3d_transform_mode mode, float x, float y, float z, float w ) {
    
    pirates3d_transform_entity transform_entity = NULL ;
    
    if ( (mode == chain) || (RKList_GetNumOfNodes(transform) == 0) ) {
    
     transform_entity = RKMem_NewMemOfType(struct pirates3d_transform_entity_s) ;
        
    } else {
        
     transform_entity = (pirates3d_transform_entity)RKList_GetData(RKList_GetLastNode(transform)) ;
        
    }
    
    transform_entity->transform = transform_type ;
    
    transform_entity->Transform[0] = x ;
    
    transform_entity->Transform[1] = y ;
    
    transform_entity->Transform[2] = z ;
    
    transform_entity->Transform[3] = w ;
    
    pirates3d_apply_transform_to_transform_list(transform_entity,transform,mode) ;
}

void pirates3d_delete_last_transform_set( pirates3d_transform transform ) {
    
    RKList_node node = RKList_GetLastNode(transform) ;
    
    RKList_DeleteNode(transform, node) ;
}

pirates3d_scene pirates3d_new_3dscene( pirates3d_camera camera, pirates_scene scene_rt ) {
    
    pirates3d_scene scene3d = RKMem_NewMemOfType(struct pirates3d_scene_s) ;
    
    scene3d->pirates3d_material_library = NULL ;
    
    scene3d->scene_rt = scene_rt ;
    
    scene3d->camera = camera ;
    
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

typedef void (*pirates3d_apply_transform_to_vertex)(pirates3d_transform_entity transform, RKMVector vertex) ;

//From: A faster quaternion-vector multiplication: https://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/

static void pirates3d_apply_Rotation_to_triangle_vertex( pirates3d_transform_entity transform, RKMVector vertex ) {
    
    RKMath_NewVector(t, 3) ;
    
    RKMath_NewVector(t2, 3) ;
    
    RKMath_Vectorthat(two, 2) ;
    
    RKMath_Vectorthat(w,transform->Transform[3]) ;
    
    RKMath_Cross(t, transform->Transform, vertex) ;
    
    RKMath_Mul(t, two, t, 3) ;
    
    RKMath_Mul(w, w, t, 3) ;
    
    RKMath_Add(vertex, vertex, w, 3) ;
    
    RKMath_Cross(t2, transform->Transform, t) ;
    
    RKMath_Add(vertex, vertex, t2, 3) ;
    
    //t = 2 * cross(q.xyz, v)
    //v' = v + q.w * t + cross(q.xyz, t)
}

static void pirates3d_apply_Scale_to_triangle_vertex( pirates3d_transform_entity transform, RKMVector vertex ) {
    
    RKMath_Mul(vertex,transform->Transform,vertex,3) ;
}

static void pirates3d_apply_Translate_to_triangle_vertex( pirates3d_transform_entity transform, RKMVector vertex ) {
    
    RKMath_Add(vertex,transform->Transform,vertex,3) ;
}

static void pirates3d_apply_triangle_transform_to_vertexes( pirates3d_transform_entity transform, pirates3d_primitive_array primitive_array, pirates3d_apply_transform_to_vertex apply_transform_to_vertex ) {
    
    int i = 0 ;
    
    pirates_triangles triangles = NULL ;
    
    pirates_triangle triangle = NULL ;
    
    triangles = (pirates_triangles)primitive_array->primitive_array[0] ;
    
    while ( i < primitive_array->num_of_primitives ) {
        
        triangle = pr_gettriangle(triangles, i) ;
        
        apply_transform_to_vertex(transform, &(triangle[pr_V1X])) ;
        
        apply_transform_to_vertex(transform, &(triangle[pr_V2X])) ;
        
        apply_transform_to_vertex(transform, &(triangle[pr_V3X])) ;
        
        triangle[10] = 1 ;
        
        i++ ;
    }
}

static void pirates3d_apply_triangle_transform( pirates3d_transform transform, pirates3d_primitive_array primitive_array ) {
    
    RKList_node node = RKList_GetFirstNode(transform) ;
    
    pirates3d_transform_entity entity = NULL ;
    
    while ( node != NULL ) {
    
    entity = RKList_GetData(node) ;
        
    switch (entity->transform) {
            
        case Rotation:
          
            pirates3d_apply_triangle_transform_to_vertexes(entity, primitive_array, pirates3d_apply_Rotation_to_triangle_vertex) ;
            
            break;
            
        case Scale:
            
            pirates3d_apply_triangle_transform_to_vertexes(entity, primitive_array, pirates3d_apply_Scale_to_triangle_vertex) ;
            
            break;
            
        case Translate:
            
            pirates3d_apply_triangle_transform_to_vertexes(entity, primitive_array, pirates3d_apply_Translate_to_triangle_vertex) ;
            
            break;
            
        default:
            break;
    }
    
    node = RKList_GetNextNode(node) ;
        
    }
}

void pirates3d_apply_transform( pirates3d_transform transform, pirates3d_primitive_array primitive_array ) {
    
    switch (primitive_array->primitive_type) {
            
        case Triangles:
            
            pirates3d_apply_triangle_transform(transform,primitive_array) ;
            
            break;
            
        default:
            break;
    }
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
        
    } else if ( (copy_mode == pirates3d_dont_copy_data) || (copy_mode == pirates3d_dont_copy_but_delete_data) ) {
        
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
