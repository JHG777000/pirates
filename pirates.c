//
//  pirates.c
//  pirates
//
//  Created by Jacob Gordon on 10/21/14.
//  Copyright (c) 2014 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pirates.h"

static int framcount = 0 ;

typedef struct FastList_s { struct FastList_s* before ; struct FastList_s* after ; void* data ; } FastList_object ;

typedef FastList_object* FastList ;

static void pirates_destroymaterials( pirates_scene scene ) ;

static void pirates_delete_triangle_arrays( pirates_scene scene ) ;

static void ComputeBoundingSphere(  RKMVector A,  RKMVector B,  RKMVector C, float* a, float* b, float* c, float* r) ;

void pirates_addfloat( float float_, float** float_array_ptr, int* num_of_floats ) ;

static int ARandomNumber( int randmin, int randmax ) {
    
    static long state = 0 ;
    
    static int init = 0 ;
    
    int randval = 0 ;
    
    if ( randmax == -1 ) init = 0 ;
    
    randmax++ ;
    
    if ( randmax <= 0 ) randmax = 1 ;
    
    if (!init) {
        
        state = (unsigned)time(NULL) ;
        
        init = 1 ;
    }
    
    state ^= (state << 21) ;
    
    state ^= (state >> 35) ;
    
    state ^= (state << 4) ;
    
    randval = (int) state % randmax ;
    
    randval = (randval < 0) ? -randval : randval ;
    
    randval = (randval < randmin) ? ARandomNumber(randmin, randmax) : randval ;
    
    return  randval ;
    
}

raycolor Colorit(float red, float blue, float green) {
    
    raycolor newcolor ;
    
    newcolor.r = red ;
    
    newcolor.b = blue ;
    
    newcolor.g = green ;
    
    return newcolor ;
}

raycolor Colorthat(float that) {
    
    return Colorit(that,that,that) ;
}

raycolor Colorin(raypixel_ref color) {
    
    raycolor newcolor ;
    
    newcolor.r = ((float)color->r)/255.0f ;
    
    newcolor.b = ((float)color->b)/255.0f ;
    
    newcolor.g = ((float)color->g)/255.0f ;
    
    free(color) ;
    
    return newcolor ;
    
}

IDKColor Colorout(raycolor color) {
    
    IDKColor outcolor ;
    
    outcolor.red = color.r ;
    
    outcolor.blue = color.b ;
    
    outcolor.green = color.g ;
    
    return outcolor ;
}

raycolor Color_add(raycolor color_a, raycolor color_b) {
    
    return Colorit(color_a.r + color_b.r, color_a.b + color_b.b, color_a.g + color_b.g) ;
    
}

raycolor Color_mul(raycolor color_a, raycolor color_b) {
    
    return Colorit(color_a.r * color_b.r, color_a.b * color_b.b, color_a.g * color_b.g) ;
    
}

raycolor Color_clamp(raycolor color) {
    
    color.r  = ( color.r > 1 ) ? 1 : color.r ;
    
    color.r  = ( color.r < 0 ) ? 0 : color.r ;
    
    color.b  = ( color.b > 1 ) ? 1 :color.b ;
    
    color.b  = ( color.b < 0 ) ? 0 : color.b ;
    
    color.g  = ( color.g > 1 ) ? 1 : color.g ;
    
    color.g  = ( color.g < 0 ) ? 0 : color.g ;
    
    return color ;
}

pirates_scene pirates_new_scene( piretes2d_scene scene, int debug, double draw_distance, RKMVector position, RKMVector focus, int res_x, int res_y, int sort_min, int sort_max, int level_max) {
    
    framcount++ ;
    
    pirates_scene new_scene = RKMem_NewMemOfType(pirates_scene_object) ;
    
    new_scene->scene2d = scene ;
    
    new_scene->debug = debug ;
    
    new_scene->draw_distance = draw_distance ;
    
    new_scene->Camera = pr_newcam(position, focus) ;
    
    new_scene->res_x = res_x ;
    
    new_scene->res_y = res_y ;
    
    new_scene->materials = NULL ;
    
    new_scene->num_of_materials = 0 ;
    
    new_scene->sphere_array = NULL ;
    
    new_scene->numspheres = 0 ;
    
    new_scene->geom_data = NULL ;
    
    new_scene->primitive_list = NULL ;
    
    RKTasks_StartLock(new_scene->SubLock) ;
    
    new_scene->TaskGroup = NULL ;
    
    new_scene->box_init = 0 ;
    
    new_scene->sort_min = sort_min ;
    
    new_scene->sort_max = sort_max ;
    
    new_scene->level_max = level_max ;
    
    new_scene->scene_bin = NULL ;
    
    return new_scene ;
}

void pirates_change_camera(pirates_scene scene, float x, float y, float z, float fx, float fy, float fz) {
    
    RKMath_Vectorit(position,  x, y, z) ;
    
    RKMath_Vectorit(focus, fx, fy, fz) ;
    
    pr_usecam(scene->Camera, position, focus) ;
    
}

void pirates_destroy_bins( pirates_scene scene ) {
    
    int i = 0 ;
    
    while ( i < scene->scene_bin->num_of_bins_in_scene) {
        
         RKList_DeleteList(scene->scene_bin->scene_bin_array[i]->primitive_list) ;
        
         RKList_DeleteList(scene->scene_bin->scene_bin_array[i]->bin_list) ;
        
         free(scene->scene_bin->scene_bin_array[i]) ;
        
        i++ ;
    }
    
    RKList_DeleteList(scene->scene_bin->bin_list) ;
    
    free(scene->scene_bin->scene_bin_array) ;
    
    free(scene->scene_bin) ;
}

void pirates_destroy_scene( pirates_scene scene ) {
    
    pirates_destroymaterials(scene) ;
    
    RKList_DeleteList(scene->primitive_list) ;
    
    pirates_destroy_bins(scene) ;
    
    freecam(scene->Camera) ;
    
    if ( scene->TaskGroup != NULL ) RKTasks_KillTaskGroup(scene->TaskGroup) ;
    
    RKTasks_EndLock(scene->SubLock) ;
    
    free(scene) ;
    
}

pirates_Material pirates_newmaterial( raycolor color ) {
    
    pirates_Material newmaterial = RKMem_NewMemOfType(pirates_Material_object) ;
    
    newmaterial->color = color ;
    
    return newmaterial ;
}

int pirates_addmaterial( pirates_scene scene, pirates_Material material ) {
    
    scene->num_of_materials++ ;
    
    if ( scene->materials == NULL ) {
        
        scene->materials = RKMem_CArray( scene->num_of_materials, pirates_Material ) ;
        
    } else {
        
        scene->materials = RKMem_Realloc(scene->materials, scene->num_of_materials, scene->num_of_materials-1, pirates_Material, 1) ;
    }
    
    scene->materials[(scene->num_of_materials-1)] = material ;
    
    return scene->num_of_materials ;
}

void pirates_destroymaterials( pirates_scene scene ) {
    
    int i = 0 ;
    
    while ( i < scene->num_of_materials  ) {
        
        free(scene->materials[i]) ;
        
        i++ ;
    }
    
    free(scene->materials) ;
}

static float Sphere_intersection(Ray ray, pirates_sphere sphere) {
    
    RKMath_NewVector(distance, 3) ;
    
    RKMath_Sub(distance, ray.origin, sphere->position, 3) ;
    
    float b = RKMath_Dot(distance, ray.direction, 3) ;
    
    float c = RKMath_Dot(distance, distance, 3) - pow(sphere->radius,2) ;
    
    float d = (b*b) - c ;
    
    if (d > 0) {
        return -b - sqrt(d) ;
    } else
        return 0 ;
}

static float Sphere_Intersection(Ray ray, RKMVector sphere) {
    
    RKMath_NewVector(distance, 3) ;
    
    RKMath_Vectorit(position, sphere[0], sphere[1], sphere[2]) ;
    
    float radius = sphere[3] ;
    
    RKMath_Sub(distance, ray.origin, position, 3) ;
    
    float b = RKMath_Dot(distance, ray.direction, 3) ;
    
    float c = RKMath_Dot(distance, distance, 3) - pow(radius,2) ;
    
    float d = (b*b) - c ;
    
    if (d > 0) {
        return -b - RKMath_Sqrt(d) ;
    } else
        return 0 ;
}

//From: Fast, Minimum Storage Ray/Triangle Intersection: http://www.graphics.cornell.edu/pubs/1997/MT97.pdf

static double Trig_intersection(Ray ray, RKMVector vert1, RKMVector vert2, RKMVector vert3) {
    
    double u = 0 ;
    
    double v = 0 ;
    
    double t = 0 ;
    
    double det = 0 ;
    
    double inv_det = 0 ;
    
    #define EPSOLON 0.000001
    
    RKMath_NewVector(edge1, 3) ;
    
    RKMath_NewVector(edge2, 3) ;
    
    RKMath_NewVector(tvec, 3) ;
    
    RKMath_NewVector(pvec, 3) ;
    
    RKMath_NewVector(qvec, 3) ;
    
    RKMath_Sub(edge1, vert2, vert1, 3) ;
    
    RKMath_Sub(edge2, vert3, vert1, 3) ;
    
    RKMath_Cross(pvec, ray.direction, edge2) ;
    
    det = RKMath_Dot(edge1, pvec, 3) ;
    
    if (det > -EPSOLON && det < EPSOLON) return 0 ;
    
    inv_det = 1.0 / det ;
    
    if ( inv_det == 0 ) return 0 ;
    
    RKMath_Sub(tvec, ray.origin, vert1, 3) ;
    
    u = RKMath_Dot(tvec, pvec, 3) * inv_det ;
    
    if ( ( u < 0.0 ) || ( u > 1.0 ) ) return 0 ;
    
    RKMath_Cross(qvec, tvec, edge1) ;
    
    v = RKMath_Dot(ray.direction, qvec, 3) * inv_det ;
    
    if ( ( v < 0.0 ) || ( (u + v) > 1.0 ) ) return 0 ;
    
    t = RKMath_Dot(edge2, qvec, 3) * inv_det ;
    
    return  t ;
    
}

static double pirates_sphere_intersection(Ray ray, void* data) {
    
    RKMVector sphere = (RKMVector)data ;
    
    return Sphere_Intersection(ray, sphere) ;
}

static double pirates_triangle_intersection(Ray ray, void* data) {
    
    pirates_triangle triangle = (pirates_triangle)data ;
    
    return Trig_intersection(ray,&(triangle[pr_V1X]),&(triangle[pr_V2X]),&(triangle[pr_V3X])) ;
}

static int pirates_sphere_update_func(void* data) {
    
    RKMVector sphere = (RKMVector)data ;
    
    return sphere[4] ;
}

static int pirates_triangle_update_func(void* data) {
    
    pirates_triangle triangle = (pirates_triangle)data ;
    
    int retval = pr_trigupdate(triangle) ;
    
    pr_resettrigupdate(triangle) ;
    
    return retval ;
}

static int pirates_triangle_get_mid_func(void* data) {
    
    return (((RKMVector)data)[pr_M] - 1) ;
}

pirates_volume pirates_new_volume( void* data, int can_and_should_delete, pirates_intersection_func_type intersection_func, pirates_update_func_type update_func, pirates_get_mid_func_type get_mid_func ) {
    
    pirates_volume volume = RKMem_NewMemOfType(pirates_volume_object) ;
    
    volume->data = data ;
    
    volume->can_and_should_delete = can_and_should_delete ;
    
    volume->intersection_func = intersection_func ;
    
    volume->get_mid_func = get_mid_func ;
    
    volume->update_func = update_func ;
    
    return volume ;
}

void pirates_destroy_volume( pirates_volume volume ) {
    
    if (volume->can_and_should_delete) free(volume->data) ;
    
    free(volume) ;
}

void pirates_destroy_primitive( pirates_primitive primitive ) {
    
    pirates_destroy_volume(primitive->bounding_volume) ;
    
    pirates_destroy_volume(primitive->shape_volume) ;
    
    free(primitive->primitive_array) ;
    
    free(primitive) ;
}

static pirates_volume pirates_make_bounding_sphere_for_triangle(void* data) {
    
    float a = 0 ;
    
    float b = 0 ;
    
    float c = 0 ;
    
    float r = 0 ;
    
    RKMVector sphere = RKMath_NewVectorMalloc(4) ;
    
    pirates_triangle triangle = (pirates_triangle)data ;
    
    RKMVector vert1 = &(triangle[pr_V1X]) ;
    
    RKMVector vert2 = &(triangle[pr_V2X]) ;
    
    RKMVector vert3 = &(triangle[pr_V3X]) ;
    
    ComputeBoundingSphere(vert1, vert2, vert3, &a, &b, &c, &r) ;
    
    sphere[0] = a ;
    
    sphere[1] = b ;
    
    sphere[2] = c ;
    
    sphere[3] = r ;
    
    return pirates_new_volume((void*)sphere,1,pirates_sphere_intersection,NULL,NULL) ;
}

static pirates_bounding_box pirates_triangle_bounding_box_func(void* data) {
    
    pirates_triangle triangle = (pirates_triangle)data ;
    
    return pirates_compute_triangle_bounding_box(triangle) ;
}

float fast_bin_intersection( Ray r, pirates_bin box, fast_bin_intersection_type* fbit_ptr ) ;

static double pirates_bin_intersection(Ray ray, void* data) {
    
    pirates_bounding_box* box = (pirates_bounding_box*)data ;
    
    pirates_bin_object bin_object ;
    
    bin_object.bounding_box = *box ;
    
    return fast_bin_intersection(ray, &bin_object, &ray.fbit) ;
}

static pirates_volume pirates_triangle_bounding_box_volume_func(void* data) {
    
    pirates_bounding_box box = pirates_triangle_bounding_box_func(data) ;
    
    void* box_array = malloc(sizeof(box)) ;
    
    memcpy(box_array, &box, sizeof(box)) ;
    
    return pirates_new_volume((void*)box_array,1,pirates_bin_intersection,NULL,NULL) ;
}

int pirates_triangle_size( void ) {
    
    return 11 ;
}

void pirates_make_triangle_primitive( pirates_scene scene, pirates_primitive_array primitive_array, pirates_volume volume, pirates_bounding_box bounding_box ) {
    
    pirates_primitive primitive = RKMem_NewMemOfType(pirates_primitive_object) ;
    
    primitive->primitive_array = primitive_array ;
    
    primitive->bounding_box = bounding_box ;
    
    primitive->bounding_box_func = pirates_triangle_bounding_box_func ;
    
    primitive->shape_volume = volume ;
    
    primitive->bounding_volume_func = pirates_make_bounding_sphere_for_triangle ;
    
    primitive->bounding_volume = primitive->bounding_volume_func(primitive->shape_volume->data) ;
    
    RKTasks_LockLock(scene->SubLock) ;
    
    RKList_AddToList(scene->primitive_list, (void*)primitive) ;
    
    RKTasks_UnLockLock(scene->SubLock) ;
}

pirates_primitive_array pirates_new_primitive_array( void* primitive_data ) {
    
    pirates_primitive_array primitive_array = RKMem_NewMemOfType(void*) ;
    
    primitive_array[0] = primitive_data ;
    
    return primitive_array ;
}

void pirates_destroy_primitive_array( pirates_primitive_array primitive_array ) {
    
    primitive_array[0] = NULL ;
}

void pirates_add_triangle_array( pirates_scene scene, pirates_primitive_array primitive_array, int numtrigs, int can_and_should_delete ) {
    
    int i = 0 ;
    
    pirates_triangle triangle ;
    
    if ( primitive_array == NULL ) return ;
    
    pirates_triangles triangles = (pirates_triangles)primitive_array[0] ;
    
    if ( triangles == NULL ) return ;
    
    if ( scene->primitive_list == NULL ) scene->primitive_list = RKList_NewList() ;
    
    while ( i < numtrigs ) {
        
        triangle = pr_gettriangle(triangles,i) ;
        
        pirates_make_triangle_primitive(scene,primitive_array,pirates_new_volume((void*)triangle,can_and_should_delete,pirates_triangle_intersection,pirates_triangle_update_func,pirates_triangle_get_mid_func),pirates_compute_triangle_bounding_box(triangle)) ;
        
        i++ ;
    }
}


pirates_triangle_array_buffer pirates_add_triangle_arrayold( pirates_scene scene, pirates_triangles triangles, int numtrigs ) {
    
    if ( scene->geom_data == NULL ) {
        
        pirates_triangle_array_buffer firstnode = RKMem_NewMemOfType(pirates_triangle_array_buffer_object) ;
        
        firstnode->before = NULL ;
        
        firstnode->after = NULL ;
        
        firstnode->last = firstnode ;
        
        scene->geom_data = firstnode ;
        
    } else {
        
        scene->geom_data->last->after = RKMem_NewMemOfType(pirates_triangle_array_buffer_object) ;
        
        scene->geom_data->last->after->before = scene->geom_data->last ;
        
        scene->geom_data->last->after->after = NULL ;
        
        scene->geom_data->last = scene->geom_data->last->after ;
    }
    
    float* trig_array = RKMem_CArray((10 * numtrigs), float) ;
    
    memcpy(trig_array,triangles,((10 * numtrigs)*sizeof(float))) ;
    
    scene->geom_data->last->numtrigs = numtrigs ;
    
    scene->geom_data->last->triangles = trig_array ;
    
    return scene->geom_data->last ;
}

void pirates_remove_triangle_arrayold( pirates_scene scene, pirates_triangle_array_buffer buffer ) {
    
    if ( buffer->before == NULL ) {
       
      if ( buffer->after != NULL ) { //if no node to replace the first one than don't remove
        
       buffer->after->before = buffer->before ;
      
       buffer->after->last = buffer->after ;
          
       scene->geom_data = buffer->after ;
        
       free(buffer->triangles) ;
         
       buffer->triangles = NULL ;
          
       free(buffer) ;
          
      } else {
        
       free(buffer->triangles) ; //still need to remove triangles
          
       buffer->triangles = NULL ;
          
      }
        
    } else {
        
       buffer->before->after = buffer->after ;
        
       if ( buffer->after != NULL) buffer->after->before = buffer->before ;
       
       free(buffer->triangles) ;
        
       buffer->triangles = NULL ;
        
       free(buffer) ;
    }

}

void pirates_delete_triangle_arraysold( pirates_scene scene ) {
    
    if ( scene->geom_data != NULL ) {
        
     while ( scene->geom_data->triangles != NULL ) { //scene->geom_data's before should always be NULL
        
        //pirates_remove_triangle_array(scene,scene->geom_data) ;
         
     }
        
    }
}

pirates_bounding_box pirates_compute_triangle_bounding_box( pirates_triangle triangle ) {
    
    pirates_bounding_box bounding_box ;
    
    bounding_box.x = triangle[pr_V1X] ;
    
    bounding_box.X = triangle[pr_V1X] ;
    
    bounding_box.y = triangle[pr_V1Y] ;
    
    bounding_box.Y = triangle[pr_V1Y] ;
    
    bounding_box.z = triangle[pr_V1Z] ;
    
    bounding_box.Z = triangle[pr_V1Z] ;
    
    if ( triangle[pr_V1X] < bounding_box.x ) bounding_box.x = triangle[pr_V1X] ;
    
    if ( triangle[pr_V1X] > bounding_box.X ) bounding_box.X = triangle[pr_V1X] ;
    
    if ( triangle[pr_V1Y] < bounding_box.y ) bounding_box.y = triangle[pr_V1Y] ;
    
    if ( triangle[pr_V1Y] > bounding_box.Y) bounding_box.Y = triangle[pr_V1Y] ;
    
    if ( triangle[pr_V1Z] < bounding_box.z ) bounding_box.z = triangle[pr_V1Z] ;
    
    if ( triangle[pr_V1Z] > bounding_box.Z) bounding_box.Z = triangle[pr_V1Z] ;
    
    if ( triangle[pr_V2X] < bounding_box.x ) bounding_box.x = triangle[pr_V2X] ;
    
    if ( triangle[pr_V2X] > bounding_box.X ) bounding_box.X = triangle[pr_V2X] ;
    
    if ( triangle[pr_V2Y] < bounding_box.y ) bounding_box.y = triangle[pr_V2Y] ;
    
    if ( triangle[pr_V2Y] > bounding_box.Y) bounding_box.Y = triangle[pr_V2Y] ;
    
    if ( triangle[pr_V2Z] < bounding_box.z ) bounding_box.z = triangle[pr_V2Z] ;
    
    if ( triangle[pr_V2Z] > bounding_box.Z) bounding_box.Z = triangle[pr_V2Z] ;

    if ( triangle[pr_V3X] < bounding_box.x ) bounding_box.x = triangle[pr_V3X] ;
    
    if ( triangle[pr_V3X] > bounding_box.X ) bounding_box.X = triangle[pr_V3X] ;
    
    if ( triangle[pr_V3Y] < bounding_box.y ) bounding_box.y = triangle[pr_V3Y] ;
    
    if ( triangle[pr_V3Y] > bounding_box.Y) bounding_box.Y = triangle[pr_V3Y] ;
    
    if ( triangle[pr_V3Z] < bounding_box.z ) bounding_box.z = triangle[pr_V3Z] ;
    
    if ( triangle[pr_V3Z] > bounding_box.Z) bounding_box.Z = triangle[pr_V3Z] ;
    
    return bounding_box ;
}

static void ComputeBoundingSphere(  RKMVector A,  RKMVector B,  RKMVector C, float* a, float* b, float* c, float* r) {
    
    RKMath_NewVector(D, 3) ;
    
    float da = RKMath_Distance(B, C, 3) ;
    
    float db = RKMath_Distance(A, C, 3) ;
    
    float dc = RKMath_Distance(A, B, 3) ;
    
    float p = da + db + dc ;
    
    *a = (((da * A[RKM_X]) + (db * B[RKM_X]) + (dc * C[RKM_X])) / p) ;
    
    *b = (((da * A[RKM_Y]) + (db * B[RKM_Y]) + (dc * C[RKM_Y])) / p) ;
    
    *c = (((da * A[RKM_Z]) + (db * B[RKM_Z]) + (dc * C[RKM_Z])) / p) ;
    
    D[RKM_X] = *a ;
    
    D[RKM_Y] = *b ;
    
    D[RKM_Z] = *c ;
    
    *r = MAX_JHG(MAX_JHG(RKMath_Distance(D,A,3), RKMath_Distance(D,B,3)), RKMath_Distance(D,C,3)) ;
    
}

pirates_sphere pirates_compute_bounding_sphere( pirates_triangle triangle, pirates_bounding_box bounding_box ) {
    
    float a = 0 ;
    
    float b = 0 ;
    
    float c = 0 ;
    
    float r = 0 ;
    
    RKMVector vert1 = &(triangle[pr_V1X]) ;
    
    RKMVector vert2 = &(triangle[pr_V2X]) ;
    
    RKMVector vert3 = &(triangle[pr_V3X]) ;
    
    ComputeBoundingSphere(vert1, vert2, vert3, &a, &b, &c, &r) ;
    
    pirates_sphere sphere = RKMem_NewMemOfType(pirates_sphere_object) ;
    
    sphere->bounding_box = bounding_box ;
    
    sphere->triangle = triangle ;
    
    sphere->position = RKMath_NewVectorMalloc(3) ;
    
    RKMath_Vectorit(pos,a,b,c) ;
    
    RKMath_Equal(sphere->position, pos, 3) ;
    
    sphere->radius = r ;
    
    sphere->t = 0 ;
    
    return sphere ;
    
}

void pirates_add_sphere( pirates_spheres* sphere_array, int* numspheres, pirates_sphere sphere ) {
    
    (*numspheres)++ ;
    
    if ( (*sphere_array) == NULL ) {
        
        (*sphere_array) = RKMem_CArray( (*numspheres), pirates_sphere ) ;
        
    } else {
        
        (*sphere_array) = RKMem_Realloc( (*sphere_array), (*numspheres), (*numspheres)-1, pirates_sphere, 1 ) ;
    }
    
    (*sphere_array)[((*numspheres)-1)] = sphere ;
    
}

static int CheckX( float X, float x, float X_tile, float x_tile) {
    
    if ( X < x_tile ) return 0 ;
    
    if ( x > X_tile ) return 0 ;
    
    return 1 ;
    
}

static int CheckY( float Y, float y, float Y_tile, float y_tile) {
    
    if ( Y < y_tile ) return 0 ;
    
    if ( y > Y_tile ) return 0 ;
    
    return 1 ;
    
}

static int CheckZ( float Z, float z, float Z_tile, float z_tile) {
    
    if ( Z < z_tile ) return 0 ;
    
    if ( z > Z_tile ) return 0 ;
    
    return 1 ;
    
}

static int CheckXYZ( pirates_bounding_box object,  pirates_bounding_box box ) {
    
    if ( CheckX(object.X, object.x, box.X, box.x) && CheckY(object.Y, object.y, box.Y, box.y) && CheckZ(object.Z, object.z, box.Z, box.z) ) return 1 ;
    
    return 0 ;
}

static int CheckInside( pirates_bounding_box object, pirates_bounding_box box ) {
    
    if ( (object.X > box.x) && (object.X <= box.X) && (object.Y > box.y) && (object.Y <= box.Y) && (object.Z > box.z) && (object.Z <= box.Z) && (object.x < box.X) && (object.x >= box.x) && (object.y < box.Y) && (object.y >= box.y) && (object.z < box.Z) && (object.z >= box.z)) return 1 ;
    
    return 0 ;
}

void pirates_compute_scene_bounding_box( pirates_scene scene, pirates_bounding_box bounding_box ) ;

void pirates_add_geom_to_bins( pirates_scene scene, pirates_scene_bin scene_bin ) {
    
    scene_bin->primitive_list = scene->primitive_list ;
    
    scene_bin->bounding_box = scene->bounding_box ;
}

void pirates_make_the_bins( pirates_scene_bin scene_bin, pirates_primitive primitive ) ;

void pirates_proc_scene( pirates_scene scene ) {
    
    pirates_geom_list_node deadnode = NULL ;
    
    pirates_geom_list_node node = NULL ;
    
    pirates_primitive primitive = NULL ;
    
    //scene->scene_bin = pirates_create_scene_bin(scene, scene->sort_min, scene->sort_max, scene->level_max) ;
    
    //pirates_add_geom_to_bins(scene, scene->scene_bin) ;
    
    node = RKList_GetFirstNode(scene->primitive_list) ;
    
    scene->box_init = 0 ;
    
    while (node != NULL) {
        
        if ( deadnode != NULL ) {
            
            RKList_DeleteNode(scene->primitive_list, deadnode) ;
            
            deadnode = NULL ;
        }
        
        primitive = (pirates_primitive)RKList_GetData(node) ;
        
        if ( primitive->primitive_array[0] != NULL ) {
            
            if (primitive->shape_volume->update_func(primitive->shape_volume->data)) {
                
                primitive->bounding_box = primitive->bounding_box_func(primitive->shape_volume->data) ;
                
                pirates_destroy_volume(primitive->bounding_volume) ;
                
                primitive->bounding_volume = primitive->bounding_volume_func(primitive->shape_volume->data) ;
        
            }
            
            //pirates_make_the_bins(scene->scene_bin,primitive) ;
            
            pirates_compute_scene_bounding_box(scene, primitive->bounding_box) ;
            
        } else {
            
            pirates_destroy_primitive(primitive) ;
            
            deadnode = node ;
        }
        
        node = RKList_GetNextNode(node) ;
    }
    
    pirates_createbins(scene) ;
    
}

pirates_scene_bin pirates_create_scene_bin(pirates_scene scene, int sort_min, int sort_max, int level_max) {
    
    pirates_scene_bin scene_bin = RKMem_NewMemOfType(pirates_scene_bin_object) ;
    
    scene_bin->sort_min = sort_min ;
    
    scene_bin->sort_max = sort_max ;
    
    scene_bin->level_max = level_max ;
    
    scene_bin->root = 1 ;
    
    scene_bin->bin_id = 0 ;
    
    scene_bin->num_of_bins = 0 ;
    
    scene_bin->bin_array = NULL ;
    
    scene_bin->bigger_bin = NULL ;
    
    scene_bin->bin_list = RKList_NewList() ;
    
    scene_bin->list_node = NULL ;
    
    scene_bin->scene_bin_array = NULL ;
    
    scene_bin->num_of_bins_in_scene = 0 ;
    
    return scene_bin ;
    
}

void pirates_addBin( pirates_bin bin, pirates_bin** bin_array_ptr, int* num_of_bins ) ;

pirates_bin pirates_make_bin( pirates_scene_bin scene_bin, pirates_primitive primitive, raycolor bin_color ) {
    
    pirates_bin new_bin = RKMem_NewMemOfType(pirates_bin_object) ;
    
    new_bin->root = 0 ;
    
    new_bin->bin_id = 0 ;
    
    new_bin->num_of_bins = 0 ;
    
    new_bin->bin_array = NULL ;
    
    new_bin->color = bin_color ;
    
    new_bin->bigger_bin = NULL ;
    
    new_bin->primitive_list = RKList_NewList() ;
    
    if (primitive != NULL) RKList_AddToList(new_bin->primitive_list, (void*)primitive) ;
    
    new_bin->bin_list = RKList_NewList() ;
    
    new_bin->list_node = NULL ;
    
    if (primitive != NULL) new_bin->bounding_box = primitive->bounding_box ;
    
    pirates_addBin(new_bin,&(scene_bin->scene_bin_array),&(scene_bin->num_of_bins_in_scene)) ;
    
    return new_bin ;

}

void pirates_recompute_bin_bounding_box( pirates_primitive primitive, pirates_bin bin ) {
    
    if ( primitive->bounding_box.X > bin->bounding_box.X ) bin->bounding_box.X = primitive->bounding_box.X ;
    
    if ( primitive->bounding_box.x < bin->bounding_box.x ) bin->bounding_box.x = primitive->bounding_box.x ;
    
    if ( primitive->bounding_box.Y > bin->bounding_box.Y ) bin->bounding_box.Y = primitive->bounding_box.Y ;
    
    if ( primitive->bounding_box.y < bin->bounding_box.y ) bin->bounding_box.y = primitive->bounding_box.y ;
    
    if ( primitive->bounding_box.Z > bin->bounding_box.Z ) bin->bounding_box.Z = primitive->bounding_box.Z ;
    
    if ( primitive->bounding_box.z < bin->bounding_box.z ) bin->bounding_box.z = primitive->bounding_box.z ;

}

void pirates_addprimitive_to_bin( pirates_primitive primitive, pirates_bin bin ) {
    
    RKList_AddToList(bin->primitive_list, (void*)primitive) ;
}


static int IsClose( pirates_scene_bin scene_bin, pirates_bounding_box object,  pirates_bounding_box box ) ;

void pirates_add_bin_to_bin( pirates_bin bin, pirates_bin bigger_bin ) ;

static float GetDistance( pirates_bounding_box object,  pirates_bounding_box box ) {
    
    RKMath_Vectorit(boxpos, box.x, box.y, box.z) ;
    
    RKMath_Vectorit(objectpos, object.x, object.y, object.z) ;
    
    return RKMath_Distance(objectpos, boxpos, 3) ;
}

static int IsClose( pirates_scene_bin scene_bin, pirates_bounding_box object,  pirates_bounding_box box ) {
    
    float dist = 0 ;
    
    float dist2 = 0 ;
    
    RKMath_Vectorit(boxpos, box.x, box.y, box.z) ;
    
    RKMath_Vectorit(objectpos, object.x, object.y, object.z) ;
    
    RKMath_Vectorit(min, scene_bin->bounding_box.x, scene_bin->bounding_box.y, scene_bin->bounding_box.z) ;
    
    RKMath_Vectorit(max, scene_bin->bounding_box.X, scene_bin->bounding_box.Y, scene_bin->bounding_box.Z) ;
    
    dist = RKMath_Distance(objectpos, boxpos, 3) ;
    
    dist2 = RKMath_Distance(min, max, 3) ;
    
    float close = (dist/dist2) * 100 ;
    
    if ( (close < 5) && (close > 0) ) return 1 ;
    
    return 0 ;
}

void pirates_add_primitive_to_bin( pirates_scene_bin scene_bin, pirates_primitive primitive, pirates_bin bin, int primitive_max ) {
    
    pirates_bin nearest_bin = NULL ;
    
    pirates_bin new_bin = NULL ;
    
    pirates_bin next_bin = NULL ;
    
    RKList_node node = NULL ;
    
    float nearest = 0 ;
    
    float value = 0 ;
    
    int gate = 0 ;
    
    if (bin == NULL) return ;
    
        if (!bin->root) pirates_recompute_bin_bounding_box(primitive,bin) ;
        
        if ( (RKList_GetNumOfNodes(bin->primitive_list) > primitive_max) || (bin->root) ) {
            
            if ( RKList_GetNumOfNodes(bin->bin_list) == 0 ) {
                
                new_bin = pirates_make_bin(scene_bin, primitive, Colorthat(0.2)) ;
                
                pirates_add_bin_to_bin(new_bin,bin) ;
                
            } else {
                
                gate = 0 ;
                
                node = RKList_GetFirstNode(bin->bin_list) ;
                
                while ( node != NULL ) {
                    
                    next_bin = RKList_GetData(node) ;
                    
                    value = GetDistance(primitive->bounding_box, next_bin->bounding_box) ;
                    
                    if ( nearest_bin == NULL ) {
                        
                        nearest = value ;
                        
                        nearest_bin = next_bin ;
                    }
                    
                    if ( value < nearest ) {
                        
                        nearest = value ;
                        
                        nearest_bin = next_bin ;
                    }
                    
                    if ( CheckXYZ(primitive->bounding_box, next_bin->bounding_box) || IsClose(scene_bin, primitive->bounding_box, next_bin->bounding_box) ) {
                        
                        pirates_add_primitive_to_bin(scene_bin, primitive, next_bin, primitive_max) ;
                        
                        gate = 1 ;
                        
                        break ;
                    }
                    
                    node = RKList_GetNextNode(node) ;
                }
 
                if (!gate)  {
                
                if ( RKList_GetNumOfNodes(bin->bin_list) < scene_bin->bin_max  ) {
                 
                    new_bin = pirates_make_bin(scene_bin, primitive, Colorthat(0.2)) ;
                    
                    pirates_add_bin_to_bin(new_bin,bin) ;
                    
                } else {
                 
                    pirates_add_primitive_to_bin(scene_bin, primitive, nearest_bin, primitive_max) ;
                    
                }
                 
                }
            }
            
        } else {
            
           pirates_addprimitive_to_bin(primitive,bin) ;
            
        }
    
    }

void pirates_add_bin_to_bin( pirates_bin bin, pirates_bin bigger_bin ) {
    
    if ( (bin->bigger_bin == NULL) || (bin->bigger_bin == bigger_bin) ) {
    
    bin->bigger_bin = bigger_bin ;
    
    bin->list_node = RKList_AddToList(bigger_bin->bin_list, (void*)bin) ;
        
    } else {
      
    bin->list_node = RKList_MoveNodeFromListToList(bin->list_node, bin->bigger_bin->bin_list, bigger_bin->bin_list) ;
        
    bin->bigger_bin = bigger_bin ;
        
    }
}

typedef struct { int flag ; pirates_bounding_box bounding_box ; pirates_geom_list primitive_list ; } Group_object ;

typedef Group_object* Group ;

Group NewGroup( pirates_bounding_box box ) {
    
    Group new_group = RKMem_NewMemOfType(Group_object) ;
    
    new_group->primitive_list = RKList_NewList() ;
    
    new_group->flag = 0 ;
    
    new_group->bounding_box = box ;
    
    return new_group ;
}

void pirates_addPrimitive_to_Group( pirates_primitive primitive,  Group Group_ ) {
    
    RKList_AddToList(Group_->primitive_list, (void*)primitive) ;
}

void pirates_addGroup( Group Group_, Group** Group_array_ptr, int* num_of_Groups ) {
    
    Group* Group_array = (*Group_array_ptr) ;
    
    (*num_of_Groups)++ ;
    
    if ( Group_array == NULL ) {
        
        Group_array = RKMem_CArray( (*num_of_Groups), Group ) ;
        
    } else {
        
        Group_array = RKMem_Realloc(Group_array, (*num_of_Groups), (*num_of_Groups)-1, Group, 1) ;
    }
    
    Group_array[((*num_of_Groups)-1)] = Group_ ;
    
    (*Group_array_ptr) = Group_array ;
    
}

void pirates_addBin( pirates_bin bin, pirates_bin** bin_array_ptr, int* num_of_bins ) {
    
    pirates_bins bin_array = (*bin_array_ptr) ;
    
    (*num_of_bins)++ ;
    
    if ( bin_array == NULL ) {
        
        bin_array = RKMem_CArray( (*num_of_bins), pirates_bin ) ;
        
    } else {
        
        bin_array = RKMem_Realloc(bin_array, (*num_of_bins), (*num_of_bins)-1, pirates_bin, 1) ;
    }
    
    bin_array[((*num_of_bins)-1)] = bin ;
    
    (*bin_array_ptr) = bin_array ;
    
}

pirates_bin pirates_new_bin( pirates_bin bigger_bin, Group Group_, raycolor bin_color ) {
    
    pirates_bin new_bin = RKMem_NewMemOfType(pirates_bin_object) ;
    
    new_bin->root = 0 ;
    
    new_bin->bin_id = 0 ;
    
    new_bin->num_of_bins = 0 ;
    
    new_bin->bin_array = NULL ;
    
    new_bin->color = bin_color ;
    
    new_bin->bigger_bin = bigger_bin ;
    
    new_bin->bin_list = RKList_NewList() ;
    
    new_bin->list_node = NULL ;
    
    new_bin->primitive_list = Group_->primitive_list ;
    
    new_bin->bounding_box = Group_->bounding_box ;
    
    return new_bin ;
}

void pirates_add_bin( Group Group_, raycolor bin_color, pirates_bin bin, pirates_bins* new_bins, int* num_of_new_bins, pirates_scene_bin scene_bin ) {
    
    pirates_bin new_bin = pirates_new_bin(bin,Group_,bin_color) ;
    
    pirates_addBin(new_bin,new_bins,num_of_new_bins) ;
    
    pirates_addBin(new_bin,&(scene_bin->scene_bin_array),&(scene_bin->num_of_bins_in_scene)) ;
    
    pirates_addBin(new_bin,&(bin->bin_array),&(bin->num_of_bins)) ;
    
    new_bin->bin_id = bin->num_of_bins ;

}

void pirates_make_the_bins( pirates_scene_bin scene_bin, pirates_primitive primitive ) {
    
     scene_bin->bin_max = 8 ;
    
     pirates_add_primitive_to_bin(scene_bin,primitive,(pirates_bin)scene_bin,scene_bin->sort_max) ;
}

void pirates_make_bins( pirates_scene_bin scene_bin ) {
    
    pirates_geom_list_node node = NULL ;
    
    pirates_primitive primitive = NULL ;
    
    node = RKList_GetFirstNode(scene_bin->primitive_list) ;
    
    while ( node != NULL ) {
        
        primitive = (pirates_primitive)RKList_GetData(node) ;
        
        pirates_make_the_bins(scene_bin,primitive) ;
        
        node = RKList_GetNextNode(node) ;
    }
}

void pirates_bins_basic_sort( pirates_bin bin, pirates_scene_bin scene_bin ) {
    
    pirates_bin new_bin = NULL ;
    
    pirates_bounding_box box = bin->bounding_box ;
    
    pirates_geom_list_node node = NULL ;
    
    pirates_primitive primitive = NULL ;
    
    int i = 0 ;
    
    int n = 4 ;
    
    float box_xval = box.x ;
    
    pirates_bounding_box* boxes = RKMem_CArray(n, pirates_bounding_box) ;
    
    Group* Groups = RKMem_CArray(n,Group) ;
    
    while ( i < n ) {
        
        boxes[i] = box ;
        
        boxes[i].x = box_xval ;
        
        box_xval = boxes[i].X = (((box.X) / n) * (i+1)) ;
        
        Groups[i] = NewGroup(boxes[i]) ;
        
        node = RKList_GetFirstNode(bin->primitive_list) ;
        
        while ( node != NULL ) {
            
            primitive = (pirates_primitive)RKList_GetData(node) ;
            
            if ( CheckXYZ(primitive->bounding_box,Groups[i]->bounding_box) ) {
                
                pirates_addPrimitive_to_Group(primitive,Groups[i]) ;
                
            }
            
            node = RKList_GetNextNode(node) ;
        }
        
        new_bin = pirates_new_bin(NULL, Groups[i], Colorit(i*0.2, i*0.2, i*0.2)) ;
        
        pirates_add_bin_to_bin(new_bin, bin) ;
        
        //pirates_add_bin(Groups[i],Colorit(i*0.2, i*0.2, i*0.2),bin,new_bins,num_of_new_bins,scene_bin) ;
        
        i++ ;
    }
    
    i = 0 ;
    
    while ( i < n ) {
        
        free(Groups[i]) ;
        
        i++ ;
    }
    
    
    if (!bin->root) RKList_DeleteAllNodesInList(bin->primitive_list) ;
    
    free(boxes) ;
    
    free(Groups) ;

}

void pirates_bins_good_sort( pirates_bin bin, pirates_bins* new_bins, int* num_of_new_bins, pirates_scene_bin scene_bin ) {
    
    pirates_bounding_box box = bin->bounding_box ;
    
    pirates_geom_list_node node = NULL ;
    
    pirates_primitive primitive = NULL ;
    
    int i = 0 ;
    
    int n = 4 ;
    
    float box_xval = box.x ;
    
    pirates_bounding_box* boxes = RKMem_CArray(n, pirates_bounding_box) ;
    
    Group* Groups = RKMem_CArray(n,Group) ;
    
    while ( i < n ) {
        
        boxes[i] = box ;
        
        boxes[i].x = box_xval ;
        
        box_xval = boxes[i].X = (((box.X) / n) * (i+1)) ;
        
        Groups[i] = NewGroup(boxes[i]) ;
        
        node = RKList_GetFirstNode(bin->primitive_list) ;
        
        while ( node != NULL ) {
            
            primitive = (pirates_primitive)RKList_GetData(node) ;
            
            if ( CheckXYZ(primitive->bounding_box,Groups[i]->bounding_box) ) {
                
                pirates_addPrimitive_to_Group(primitive,Groups[i]) ;
                
            }
            
            node = RKList_GetNextNode(node) ;
        }
        
        pirates_add_bin(Groups[i],Colorit(i*0.2, i*0.2, i*0.2),bin,new_bins,num_of_new_bins,scene_bin) ;
        
        i++ ;
    }
    
    i = 0 ;
    
    while ( i < n ) {
        
        free(Groups[i]) ;
        
        i++ ;
    }
    
    
    if (!bin->root) RKList_DeleteAllNodesInList(bin->primitive_list) ;
    
    free(boxes) ;
    
    free(Groups) ;
    
}

void pirates_makelevels( pirates_bins* level_bins, int* num_of_level_bins, pirates_scene_bin scene_bin ) {
    
    pirates_bins new_bins = NULL ;
    
    int num_of_new_bins = 0 ;
    
    int i = 0 ;
    
    if ( (*level_bins) == NULL ) {
        
        (*level_bins) = RKMem_NewMemOfType(pirates_bin) ;
        
        (*level_bins)[0] = (pirates_bin) scene_bin ;
        
        (*num_of_level_bins) = 1 ;
    }
    
    while ( i < (*num_of_level_bins) ) {
        
        if ( (RKList_GetNumOfNodes((*level_bins)[i]->primitive_list) < scene_bin->sort_max) )
            pirates_bins_basic_sort((*level_bins)[i],scene_bin) ;
        
        
        //if ( ((*level_bins)[i]->numspheres > scene_bin->sort_max) ) //pirates_bins_bad_sort((*level_bins)[i]&new_bins,&num_of_new_bins,scene_bin) ;
        
        i++ ;
    }
    
    
    free((*level_bins)) ;
    
    (*level_bins) = new_bins ;
    
    (*num_of_level_bins) = num_of_new_bins ;
}

void pirates_makebins( pirates_scene_bin scene_bin ) {
    
    int level = 0 ;
    
    int num_of_level_bins = 0 ;
    
    pirates_bins level_bins = NULL ;
    
    while ( level < scene_bin->level_max ) {
        
        pirates_makelevels(&level_bins,&num_of_level_bins,scene_bin) ;
        
        level++ ;
    }
    
    free(level_bins) ;
}

void pirates_createbins(pirates_scene scene) {

    scene->scene_bin = pirates_create_scene_bin(scene, scene->sort_min, scene->sort_max, scene->level_max) ;
    
    pirates_add_geom_to_bins(scene, scene->scene_bin) ;
    
    pirates_make_bins( scene->scene_bin ) ;
    
    //pirates_makebins(scene->scene_bin) ;
}

void pirates_compute_scene_bounding_box( pirates_scene scene, pirates_bounding_box bounding_box ) {
    
    if ( !scene->box_init ) {
    
    scene->bounding_box.x = bounding_box.x ;
    
    scene->bounding_box.X = bounding_box.X ;
    
    scene->bounding_box.y = bounding_box.y ;
    
    scene->bounding_box.Y = bounding_box.Y ;
    
    scene->bounding_box.z = bounding_box.z ;
    
    scene->bounding_box.Z = bounding_box.Z ;
    
    scene->box_init++ ;
        
    }
    
    if ( bounding_box.X > scene->bounding_box.X ) scene->bounding_box.X = bounding_box.X ;
    
    if ( bounding_box.x < scene->bounding_box.x ) scene->bounding_box.x = bounding_box.x ;
    
    if ( bounding_box.Y > scene->bounding_box.Y ) scene->bounding_box.Y = bounding_box.Y ;
    
    if ( bounding_box.y < scene->bounding_box.y ) scene->bounding_box.y = bounding_box.y ;
    
    if ( bounding_box.Z > scene->bounding_box.Z ) scene->bounding_box.Z = bounding_box.Z ;
    
    if ( bounding_box.z < scene->bounding_box.z ) scene->bounding_box.z = bounding_box.z ;
}

Ray newray(RKMVector origin , RKMVector direction ) {
    
    Ray newray ;
    
    newray.fbit.init = 0 ;
    
    RKMath_VectorCopy(newray.origin, origin) ;
    
    RKMath_Norm(newray.direction, direction, 3) ;
    
    return newray ;
    
}

void Ray_position(RKMVector retvec, Ray ray, float distance) {
    
    RKMath_NewVector(mulvec, 3) ;
    
    RKMath_Vectorthat(that, distance) ;
    
    RKMath_Mul(mulvec, ray.direction, that, 3) ;
    
    RKMath_Add(retvec, ray.origin, mulvec, 3) ;
}

Raycam pr_newcam(RKMVector position, RKMVector focus) {
    
    Raycam newraycam = (Raycam) malloc(sizeof(Raycam_object)) ;
    
    pr_usecam(newraycam,position,focus) ;
    
    return newraycam ;
}

void pr_usecam(Raycam raycam, RKMVector position, RKMVector focus) {
    
    RKMath_Vectorit(vec, 0.1, 0.1, -12.0) ;
    
    RKMath_Add(raycam->position, position, vec, 3) ;
    
    RKMath_Sub(raycam->direction, raycam->position, focus, 3) ;
    
    RKMath_Norm(raycam->direction, raycam->direction, 3) ;
}

void freecam( Raycam cam ) {
    
    free( cam ) ;
}

Ray CastRay(pirates_scene scene, Raycam raycam, float x, float y) {
    
    float fov = 30 * (M_PI / 180) ;
    
    float screen_distance = (scene->res_y/2)/tan(fov/2) ;
    
    RKMath_Vectorit(up_vec, 0, 1, 0) ;
    
    RKMath_NewVector(right_vec, 3) ;
    
    RKMath_Cross(right_vec, raycam->direction, up_vec) ;
    
    RKMath_Vectorthat(screen, screen_distance) ;
    
    RKMath_Vectorthat(x_vec, (x-(scene->res_x/2)) ) ;
    
    RKMath_Vectorthat(y_vec, (y-(scene->res_y/2)) ) ;
    
    RKMath_Neg(up_vec, 3) ;
    
    RKMath_Mul(x_vec, x_vec, right_vec, 3) ;
    
    RKMath_Mul(y_vec, y_vec, up_vec, 3) ;
    
    RKMath_Mul(screen, screen, raycam->direction, 3) ;
    
    RKMath_Add(screen, screen, raycam->position, 3) ;
    
    RKMath_Add(screen, screen, x_vec, 3) ;
    
    RKMath_Add(screen, screen, y_vec, 3) ;
    
    return newray(raycam->position, screen) ;
}

void pirates_addfloat( float float_, float** float_array_ptr, int* num_of_floats ) {
    
    float* float_array = (*float_array_ptr) ;
    
    (*num_of_floats)++ ;
    
    if ( float_array == NULL ) {
        
        float_array = RKMem_CArray( (*num_of_floats), float ) ;
        
    } else {
        
        float_array = RKMem_Realloc(float_array, (*num_of_floats), (*num_of_floats)-1, float, 1) ;
    }
    
    float_array[((*num_of_floats)-1)] = float_ ;
    
    (*float_array_ptr) = float_array ;
    
}

float fracf(float x) {
    
    return x - floorf(x) ;
}

//From: An Efficient and Robust Ray–Box Intersection Algorithm: http://www.cs.utah.edu/~awilliam/box/box.pdf

float fast_bin_intersection( Ray r, pirates_bin box, fast_bin_intersection_type* fbit_ptr ) {
    
    void* bounds[2] ;
    
    RKMath_Vectorit(boxmin, box->bounding_box.x, box->bounding_box.y, box->bounding_box.z) ;
    
    RKMath_Vectorit(boxmax, box->bounding_box.X, box->bounding_box.Y, box->bounding_box.Z) ;
    
    bounds[0] = boxmin ;
    
    bounds[1] = boxmax ;
    
    if ( !(fbit_ptr->init) ) {
        
        RKMath_NewVector(one, 3) ;
        
        RKMath_Set_Vec_Equal_To_A_Const(one,1,3) ;
        
        RKMath_Div(fbit_ptr->inv_dir, one, r.direction, 3) ;
        
        fbit_ptr->sign[0] = (fbit_ptr->inv_dir[RKM_X] < 0) ;
        
        fbit_ptr->sign[1] = (fbit_ptr->inv_dir[RKM_Y] < 0) ;
        
        fbit_ptr->sign[2] = (fbit_ptr->inv_dir[RKM_Z] < 0) ;
        
        fbit_ptr->init++ ;
    }
    
    float tmin, tmax, tymin, tymax, tzmin, tzmax ;
    
    tmin = (((RKMVector)bounds[fbit_ptr->sign[0]])[RKM_X] - r.origin[RKM_X]) * fbit_ptr->inv_dir[RKM_X] ;
    
    tmax = (((RKMVector)bounds[1-fbit_ptr->sign[0]])[RKM_X] - r.origin[RKM_X]) * fbit_ptr->inv_dir[RKM_X] ;
    
    tymin = (((RKMVector)bounds[fbit_ptr->sign[1]])[RKM_Y] - r.origin[RKM_Y]) * fbit_ptr->inv_dir[RKM_Y] ;
    
    tymax = (((RKMVector)bounds[1-fbit_ptr->sign[1]])[RKM_Y] - r.origin[RKM_Y]) * fbit_ptr->inv_dir[RKM_Y] ;
    
    if ( (tmin > tymax) || (tymin > tmax) )
        return 0 ;
    
    if (tymin > tmin)
        tmin = tymin ;
    
    if (tymax < tmax)
        tmax = tymax ;
    
    tzmin = (((RKMVector)bounds[fbit_ptr->sign[2]])[RKM_Z] - r.origin[RKM_Z]) * fbit_ptr->inv_dir[RKM_Z] ;
    
    tzmax = (((RKMVector)bounds[1-fbit_ptr->sign[2]])[RKM_Z] - r.origin[RKM_Z]) * fbit_ptr->inv_dir[RKM_Z] ;
    
    if ( (tmin > tzmax) || (tzmin > tmax) )
    return 0 ;
    
    fbit_ptr->entry = tmin ;
    
    fbit_ptr->exit = tmax ;
    
    return 1 ;
}

float bin_intersection( Ray r, pirates_bin box ) {
    
    RKMath_NewVector(tmin, 3) ;
    
    RKMath_NewVector(tmax, 3) ;
    
    RKMath_NewVector(t1, 3) ;
    
    RKMath_NewVector(t2, 3) ;
    
    RKMath_Vectorit(boxmin, box->bounding_box.x, box->bounding_box.y, box->bounding_box.z) ;
    
    RKMath_Vectorit(boxmax, box->bounding_box.X, box->bounding_box.Y, box->bounding_box.Z) ;
    
    RKMath_Sub(tmin, boxmin, r.origin, 3) ;
    
    RKMath_Sub(tmax, boxmax, r.origin, 3) ;
    
    RKMath_Div(tmin, tmin, r.direction, 3) ;
    
    RKMath_Div(tmax, tmax, r.direction, 3) ;
    
    RKMath_MinMax(t1, t2, tmin, tmax, 3) ;
    
    float tNear = MAX_JHG(MAX_JHG(t1[RKM_X], t1[RKM_Y]), t1[RKM_Z]) ;
    float tFar = MIN_JHG(MIN_JHG(t2[RKM_X], t2[RKM_Y]), t2[RKM_Z]) ;
    
    if ( (tNear > 0) && (tNear < tFar) ) return tNear ;
    
    return 0 ;
}


typedef struct { RKList bin_list ; } briefcase ;

static briefcase new_briefcase( pirates_scene scene ) {
    
    briefcase brief ;
    
    brief.bin_list = RKList_NewList() ;
    
    scene->scene_bin->list_node = RKList_AddToList(brief.bin_list, (void*)scene->scene_bin) ;
    
    return brief ;
}

static void old_briefcase( briefcase brief ) {
    
    RKList_DeleteList(brief.bin_list) ;
}

typedef struct { raycolor color ; int hit ; double t ; } hitobj_object ;

typedef hitobj_object hitobj ;

static void* BinArrayGetData(void* array, int index) {
    
    pirates_bins bins = (pirates_bins)array ;
    
    return (void*)bins[index] ;
}

pirates_Material pirates_get_material(pirates_scene scene, pirates_triangle triangle ) {
    
    return scene->materials[(int)(triangle[pr_M] - 1 )] ;
}

pirates_Material pirates_get_material_with_id(pirates_scene scene, int m_id) {
    
    return scene->materials[m_id - 1] ;
}

hitobj pirates_find_object_via_bins( pirates_scene scene, briefcase brief, Ray r ) {
    
    FastList primitive_node ;
    
    raycolor color = Colorthat(0) ;
    
    pirates_primitive primitive ;
    
    pirates_primitive hit_primitive = NULL ;
    
    pirates_bin bin = NULL ;
    
    RKList bin_list = NULL ;
    
    FastList bin_node = NULL ;
    
    hitobj hit ;
    
    hit.color = Colorthat(0) ;
    
    hit.hit = 0 ;
    
    double t = 0 ;
    
    FastList_object object ;
    
    double draw_distance = scene->draw_distance ;
    
        bin_list = brief.bin_list ;
    
        bin_node = (FastList)RKList_GetFirstNode(bin_list) ;
    
        while (1) {
            
        while ( bin_node != NULL ) {
            
            bin = (pirates_bin)bin_node->data ;
            
            if ( fast_bin_intersection(r,bin,&r.fbit) ) {
                
                if (scene->debug) {
                
                    if (bin->root) {
                    
                    color = Color_add(color, Colorit(0.2, 0.2, 0.2)) ;
                    
                    } else {
                        
                    color = Color_add(color, bin->color) ;
                        
                    }
                    
                }
                
                if ((RKList_GetNumOfNodes(bin->primitive_list) > 0) && (!bin->root)) {
                
                primitive_node = (FastList)RKList_GetFirstNode(bin->primitive_list) ;
                
                while (primitive_node != NULL) {
                    
                    primitive = (pirates_primitive)primitive_node->data ;
                    
                    if ( primitive->bounding_volume->intersection_func(r,primitive->bounding_volume->data) ) {
                        
                        t = primitive->shape_volume->intersection_func(r,primitive->shape_volume->data) ;
                        
                        if ( (t) && (0 < t < draw_distance) ) {
                            
                            hit.hit = 1 ;
                            
                            draw_distance = t ;
                            
                            hit_primitive = primitive ;
                        }
                    
                    }
                    
                    primitive_node = primitive_node->after ;
                }

            }
                
                if ( RKList_GetNumOfNodes(bin->bin_list) > 0 ) {
                    
                    bin_list = bin->bin_list ;
                    
                    bin_node = (FastList)RKList_GetFirstNode(bin_list) ;
                    
                    object.after = bin_node ;
                    
                    bin_node = &object ;
                }

            }
            
            bin_node = bin_node->after ;
        }
            
            if ( bin == NULL ) break ;
            
            if ( bin->bigger_bin != NULL ) {
                
                bin_node = ((FastList)bin->list_node)->after ;
                
                bin = bin->bigger_bin ;
                
            } else {
                
                break ;
            }
            
        }
        
        if (hit.hit)  {
            
         hit.color = scene->materials[hit_primitive->shape_volume->get_mid_func(hit_primitive->shape_volume->data)]->color ;
        
         if (scene->debug) color = Color_add(color, hit.color) ;
            
        }
    
    if ( scene->debug ) {
    
    hit.color = color ;
    
    }
        
    return hit ;
    
}

hitobj pirates_get_first_intersection( pirates_scene scene, briefcase brief, Ray r ) {
    
     return pirates_find_object_via_bins(scene,brief,r) ;
}

raycolor pirates_ray_cast_func(Ray r, briefcase brief, pirates_scene scene) {
    
     raycolor fincolor = Colorthat(0) ;
    
     hitobj hit = pirates_get_first_intersection( scene, brief, r ) ;
    
     fincolor = Color_add(fincolor, hit.color) ;
   
     return fincolor ;
}

static void xytest( int* x, int* y, int* pixel, int max_x, int max_y ) {
    
    int max = ( max_x * max_y ) ;
    
    if ( (*pixel) >= max ) {
     
        (*pixel) = -1 ;
        
        return ;
    }
    
    (*x) = (*pixel) ;
    
    if ((*x) >= max_x) {
        
        (*y) = (*x) / max_x ;
        
        (*x) = (*x) - (max_x * (*y)) ;
    }
}

RKTasks_CreateTask(pirates_render_task, pirates_scene scene ; int num_of_tasks ; ,
    
    briefcase brief = new_briefcase(RKTArgs->scene) ;
                   
    int x = 0 ;
                   
    int y = 0 ;
                   
    int pixel = 0 ;
                   
    pixel = RKTasks_GetTaskID(ThisTask) ;
                   
    while ( 1 ) {
                   
     xytest(&(x),&(y),&(pixel),RKTArgs->scene->res_x, RKTArgs->scene->res_y) ;
     
     if (pixel < 0) break ;
        
     IDK_SetColor(RKTArgs->scene->scene2d, x, y, Colorout(Color_clamp( pirates_ray_cast_func(CastRay(RKTArgs->scene, RKTArgs->scene->Camera, x, y),brief,RKTArgs->scene)))) ;
         
     pixel += RKTArgs->num_of_tasks ;
                       
    }
                   
    old_briefcase(brief) ;
) ;

void pirates_render(pirates_scene scene) {
    
    int i = 0 ;
    
    int num_of_tasks = 1000 ;
    
    RKTasks_ThreadGroup ThreadGroup = IDK_GetThreads(scene->scene2d) ;
    
    if ( scene->TaskGroup == NULL ) {
    
    scene->TaskGroup = RKTasks_NewTaskGroup() ;
    
    RKTasks_BindTaskGroupToThreadGroup(scene->TaskGroup, ThreadGroup) ;
    
    RKTasks_RunThreadGroup(ThreadGroup) ;
        
    RKTasks_Args(pirates_render_task) ;
        
        RKTasks_UseArgs(pirates_render_task,0) ;
        
        pirates_render_task_Args->scene = scene ;
        
        pirates_render_task_Args->num_of_tasks = num_of_tasks ;
        
        while (i < num_of_tasks) {
            
            RKTasks_AddTask(scene->TaskGroup, pirates_render_task, pirates_render_task_Args) ;
            
            i++ ;
        }
        
        
    } else {
        
        RKTasks_BindTaskGroupToThreadGroup(scene->TaskGroup, ThreadGroup) ;
        
        RKTasks_UseTaskGroup(scene->TaskGroup) ;
        
        RKTasks_WaitForTasksToBeDone(scene->TaskGroup) ;
    }
}
