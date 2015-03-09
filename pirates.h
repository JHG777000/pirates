//
//  pirates.h
//  pirates
//
//  Created by Jacob Gordon on 10/21/14.
//  Copyright (c) 2014 Jacob Gordon. All rights reserved.
//

#ifndef __pirates__pirates__
#define __pirates__pirates__

#include <stdio.h>
#include "RKMem.h"
#include "RKMath.h"
#include "RKTasks.h"
#include "codename.h"

//Color
typedef codename_pixelcolor raypixel_ref ;
typedef struct {float r ; float b ; float g ;} raycolor ;
raycolor Colorit(float red, float blue, float green) ;
raycolor Colorin(raypixel_ref color) ;
raypixel_ref Colorout(raycolor color) ;
raycolor Color_add(raycolor color_a, raycolor color_b) ;
raycolor Color_mul(raycolor color_a, raycolor color_b) ;
raycolor Color_clamp(raycolor color) ;

//Ray
typedef struct { RKMath_NewVector(origin, 3) ; RKMath_NewVector(direction, 3) ; } Ray ;
Ray newray(RKMVector origin , RKMVector direction ) ;
void Ray_position(RKMVector retvec, Ray ray, float distance) ;

struct pirates_scene_s ;

//Raycam
typedef struct {RKMVector position ; RKMVector direction ; } Raycam_object ;
typedef Raycam_object* Raycam ;
Raycam newcam(RKMVector position, RKMVector focus) ;
void freecam( Raycam cam ) ;
Ray CastRay(struct pirates_scene_s* scene, Raycam raycam, float x, float y) ;


typedef codename_scene piretes2d_scene ;

typedef struct { raycolor color ; } pirates_Material_object ;

typedef pirates_Material_object* pirates_Material ;

typedef pirates_Material* pirates_Materials ;

typedef struct { float X ; float x ; float Y ; float y ; float Z ; float z ; } pirates_bounding_box ;

typedef RKMVector pirates_triangle ; //10-Vector

typedef RKMVector pirates_triangles ; //N-Vector

#define pr_gettriangle(pirates_triangles,n) (&(pirates_triangles[n]))

#define pr_V1X 0

#define pr_V1Y 1

#define pr_V1Z 2

#define pr_V2X 3

#define pr_V2Y 4

#define pr_V2Z 5

#define pr_V3X 6

#define pr_V3Y 7

#define pr_V3Z 8

#define pr_M 9

typedef struct { RKMVector position ; float radius ; pirates_bounding_box bounding_box ; pirates_triangle triangle ;
    
double t ; int sphere_id ; RKT_Lock sphere_lock ; } pirates_sphere_object ;

typedef pirates_sphere_object* pirates_sphere ;

typedef pirates_sphere* pirates_spheres ;

typedef struct pirates_bin_s { pirates_bounding_box bounding_box ; struct pirates_bin_s* bigger_bin ; struct pirates_bin_s** bin_array ; int num_of_bins ;

pirates_spheres sphere_array ; int numspheres ; int bin_type ; int bin_id ; } pirates_bin_object ;

typedef pirates_bin_object* pirates_bin ;

typedef pirates_bin* pirates_bins ;

typedef struct { pirates_bounding_box bounding_box ; struct pirates_bin_s* bigger_bin ; pirates_bins bin_array ; int num_of_bins ; pirates_spheres sphere_array ;
    
int numspheres ; int bin_type ; int bin_id ; int sort_min ; int sort_max ; int level_max ;
    
pirates_bins scene_bin_array ; int num_of_bins_in_scene ; } pirates_scene_bin_object ;

typedef pirates_scene_bin_object* pirates_scene_bin ;

typedef struct { pirates_bounding_box bounding_box ; pirates_spheres sphere_array ; int numspheres ; } pirates_grid_cell ;

typedef struct { int GridX ; int GridY ; int GridZ ; float GridSizeX ; float GridSizeY ; float GridSizeZ ;

pirates_grid_cell*** cell_array ; } pirates_grid_object ;

typedef pirates_grid_object* pirates_grid ;

typedef RKList pirates_geom_list ;

typedef RKList_node pirates_geom_list_node ;

typedef enum { pr_triangles } pirates_geom_type ;

typedef struct { pirates_geom_type type ; pirates_geom_list_node node ;
    
union { struct { pirates_triangles triangles ; int numtrigs ; } triangles ; } ; } pirates_geom_block_object ;

typedef pirates_geom_block_object* pirates_geom_block ;

typedef struct pirates_triangle_array_buffer_s { pirates_triangles triangles ; int numtrigs ; struct pirates_triangle_array_buffer_s* before ;
    
struct pirates_triangle_array_buffer_s* after ; struct pirates_triangle_array_buffer_s* last ; } pirates_triangle_array_buffer_object ;

typedef pirates_triangle_array_buffer_object* pirates_triangle_array_buffer ;

typedef pirates_triangle_array_buffer pirates_geom_data ;

typedef struct pirates_scene_s {
    
piretes2d_scene scene ; Raycam Camera ;
    
pirates_Materials materials ; int num_of_materials ; pirates_geom_list geom_list ; pirates_geom_data geom_data ; pirates_grid grid ; int res_x ; int res_y ;
    
int grid_x ; int grid_y ; int grid_z ; float max_x ; float max_y ; float max_z ; float min_x ; float min_y ; float min_z ;
    
pirates_scene_bin scene_bin ; double draw_distance ; pirates_bounding_box bounding_box ;
    
pirates_spheres sphere_array ; int numspheres ; } pirates_scene_object ;

typedef pirates_scene_object* pirates_scene ;

pirates_scene pirates_new_scene( piretes2d_scene scene, double draw_distance, RKMVector position, RKMVector focus, int res_x, int res_y, int sort_min, int sort_max, int level_max) ;

pirates_grid pirates_newgrid( pirates_scene scene, float grid_x, float grid_y, float grid_z ) ;

pirates_triangle_array_buffer pirates_add_triangle_array( pirates_scene scene, pirates_triangles triangles, int numtrigs ) ;

void pirates_destroygrid( pirates_grid grid ) ;

pirates_Material pirates_newmaterial( raycolor color ) ;

pirates_Material pirates_get_material(pirates_scene scene, pirates_triangle triangle ) ;

int pirates_addmaterial( pirates_scene scene, pirates_Material material ) ;

void pirates_destroy_scene( pirates_scene scene ) ;

void pirates_proc_scene( pirates_scene scene ) ;

void pirates_render(pirates_scene scene) ;

void pirates_add_sphere( pirates_spheres* sphere_array, int* numspheres, pirates_sphere sphere ) ;

pirates_bounding_box pirates_compute_bounding_box( pirates_triangle triangle ) ;

void pirates_change_camera(pirates_scene scene, float x, float y, float z) ;

void pirates_createbins(pirates_scene scene) ;

float box_intersection( Ray r, pirates_bin box ) ;

float Sphere_intersection(Ray ray, pirates_sphere sphere) ;

double Trig_intersection(Ray ray, RKMVector vert1, RKMVector vert2, RKMVector vert3) ;

pirates_scene_bin pirates_create_scene_bin(pirates_scene scene, int sort_min, int sort_max, int level_max) ;

#endif /* defined(__pirates__pirates__) */
