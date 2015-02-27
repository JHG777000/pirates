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

static void pirates_destroymaterials( pirates_scene scene ) ;

static void pirates_delete_triangle_arrays( pirates_scene scene ) ;

void pirates_addfloat( float float_, float** float_array_ptr, int* num_of_floats ) ;

static int ARandomNumber( int randmax ) {
    
    static long state = 0 ;
    
    static int init = 0 ;
    
    int randval = 0 ;
    
    if ( randmax == -1 ) init = 0 ;
    
    randmax++ ;
    
    if ( randmax == 0 ) randmax = 1 ;
    
    if (!init) {
        
        state = (unsigned)time(NULL) ;
        
        init = 1 ;
    }
    
    state ^= (state << 21) ;
    
    state ^= (state >> 35) ;
    
    state ^= (state << 4) ;
    
    randval = (int) state % randmax ;
    
    return (randval < 0) ? -randval : randval ;
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

raypixel_ref Colorout(raycolor color) {
    
    return codename_NewColorObject((cn_RGBcolor)(color.r * 255), (cn_RGBcolor)(color.b * 255), (cn_RGBcolor)(color.g * 255)) ;
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

pirates_scene pirates_new_scene( piretes2d_scene scene, double draw_distance, RKMVector position, RKMVector focus, int res_x, int res_y, int sort_min, int sort_max, int level_max) {
    
    framcount++ ;
    
    pirates_scene new_scene = RKMem_NewMemOfType(pirates_scene_object) ;
    
    new_scene->scene = scene ;
    
    new_scene->draw_distance = draw_distance ;
    
    new_scene->Camera = newcam(position, focus) ;
    
    new_scene->res_x = res_x ;
    
    new_scene->res_y = res_y ;
    
    new_scene->materials = NULL ;
    
    new_scene->num_of_materials = 0 ;
    
    new_scene->sphere_array = NULL ;
    
    new_scene->numspheres = 0 ;
    
    new_scene->geom_data = NULL ;
    
    new_scene->scene_bin = pirates_create_scene_bin(new_scene,sort_min,sort_max,level_max) ;
    
    return new_scene ;
}

void pirates_change_camera(pirates_scene scene, float x, float y, float z) {
    
    free(scene->Camera) ;
    
    RKMath_Vectorit(position, 0.1 + x, 0.1 + y, -12.0 + z) ;
    
    RKMath_Vectorit(focus, 0.0, 0.0, 0.0) ;
    
    scene->Camera = newcam(position, focus) ;
    
}

void pirates_destroy_sphere_array( pirates_scene scene ) {
    
    int i = 0 ;
    
    while ( i < scene->numspheres) {
        
        free( scene->sphere_array[i]->position ) ;
        
        free(scene->sphere_array[i]) ;
        
        i++ ;
    }
    
    free(scene->sphere_array) ;
}

void pirates_destroy_bin_array( pirates_scene scene ) {
    
    int i = 0 ;
    
    while ( i < scene->scene_bin->num_of_bins_in_scene) {
        
        if (scene->scene_bin->scene_bin_array[i]->bin_type) free(scene->scene_bin->scene_bin_array[i]->sphere_array) ;
        
        free(scene->scene_bin->scene_bin_array[i]->bin_array) ;
        
        free(scene->scene_bin->scene_bin_array[i]) ;
        
        i++ ;
    }
    
    free(scene->scene_bin->scene_bin_array) ;
    
    free(scene->scene_bin->bin_array) ;
    
    free(scene->scene_bin) ;
}

void pirates_destroy_scene( pirates_scene scene ) {
    
    pirates_destroymaterials(scene) ;
    
    pirates_delete_triangle_arrays(scene) ;
    
    free(scene->geom_data) ;
    
    pirates_destroy_sphere_array(scene) ;
    
    pirates_destroy_bin_array(scene) ;
    
    freecam(scene->Camera) ;
    
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

pirates_grid pirates_newgrid( pirates_scene scene, float grid_x, float grid_y, float grid_z) {
    
    pirates_grid grid = RKMem_NewMemOfType(pirates_grid_object) ;
    
    grid->cell_array = NULL ;
    
    float GridSizeX = 0 ;
    
    float GridSizeY = 0 ;
    
    float GridSizeZ = 0 ;
    
    float GridSizeXMin = 0 ;
    
    float GridSizeYMin = 0 ;
    
    float GridSizeZMin = 0 ;
    
    int GridX = 1 ;
    
    int GridY = 1 ;
    
    int GridZ = 1 ;
        
        GridX = grid_x ;
        
        GridY = grid_y ;
        
        GridZ = grid_z ;
        
        float ResXMax = scene->max_x ;
        
        float ResYMax = scene->max_y ;
        
        float ResZMax = scene->max_z ;
    
        float ResXMin = scene->min_x ;
    
        float ResYMin = scene->min_y ;
    
        float ResZMin = scene->min_z ;
    
        //if ( GridX > ResX ) GridX = ResX ;
        
        GridSizeX = ResXMax / GridX ;
    
        GridSizeXMin = ResXMin / GridX ;
    
        //if ( GridSizeX < 1 ) GridSizeX = 3 ;
        
        //if ( GridY > ResY ) GridY = ResY ;
        
        GridSizeY = ResYMax / GridY ;
    
        GridSizeYMin = ResYMin / GridY ;
    
        //if ( GridSizeY < 1 ) GridSizeY = 3 ;
        
        //if ( GridZ > ResZ ) GridZ = ResZ ;
        
        GridSizeZ = ResZMax / GridZ ;
    
        GridSizeZMin = ResZMin / GridZ ;
    
        //if ( GridSizeZ < 1 ) GridSizeZ = 3 ;
        
        grid->GridX = GridX ;
        
        grid->GridY = GridY ;
        
        grid->GridZ = GridZ ;
        
        grid->GridSizeX = GridSizeX ;
        
        grid->GridSizeY = GridSizeY ;
        
        grid->GridSizeZ = GridSizeZ ;
   
    
    grid->cell_array = RKMem_CArray(GridX, pirates_grid_cell**) ;
    
    int x = 0 ;
    
    int y = 0 ;
    
    int z = 0 ;
    
    while ( x < GridX ) {
        
        grid->cell_array[x] = RKMem_CArray(GridY, pirates_grid_cell*) ;
        
        x++ ;
    }
    
    x = 0 ;
    
    while ( x < GridX ) {
    
    y = 0 ;
        
    while ( y < GridY ) {
        
        grid->cell_array[x][y] = RKMem_CArray(GridZ, pirates_grid_cell) ;
        
        y++ ;
    }
        x++ ;
    }
    
         
    x = 0 ;
    
    while ( x < GridX ) {
        
        y = 0 ;
        
        while ( y < GridY ) {
          
            z = 0 ;
            
          while ( z < GridZ ) {
            
            grid->cell_array[x][y][z].bounding_box.X = (x+1) * GridSizeX ;
            
            grid->cell_array[x][y][z].bounding_box.x = (x+0) * GridSizeXMin ;
            
            grid->cell_array[x][y][z].bounding_box.Y = (y+1) * GridSizeY ;
            
            grid->cell_array[x][y][z].bounding_box.y = (y+0) * GridSizeYMin ;
              
            grid->cell_array[x][y][z].bounding_box.Z = (z+1) * GridSizeZ ;
              
            grid->cell_array[x][y][z].bounding_box.z = (z+0) * GridSizeZMin ;
              
            grid->cell_array[x][y][z].sphere_array = NULL ;
              
            grid->cell_array[x][y][z].numspheres = 0 ;
           
            z++ ;
          }
            y++ ;
        }
        
        x++ ;
    }
    
    return grid ;
}

void pirates_clearthegrid( pirates_grid grid ) {
    
    int x = 0 ;
    
    int y = 0 ;
    
    int z = 0 ;
    
    int i = 0 ;
    
    while ( x < grid->GridX ) {
        
        y = 0 ;
        
        while ( y < grid->GridY ) {
            
            z = 0 ;
            
            while ( z < grid->GridZ ) {
                
                i = 0 ;
                
                if ( grid->cell_array[x][y][z].sphere_array != NULL ) {
                
                 while ( i < grid->cell_array[x][y][z].numspheres ) {
                    
                    //free(grid->cell_array[x][y][z].sphere_array[i]->position) ;
                     
                    //free(grid->cell_array[x][y][z].sphere_array[i]) ;
            
                     i++ ;
                 }
                
                }
                
                free(grid->cell_array[x][y][z].sphere_array) ;
                
                z++ ;
            }
            y++ ;
        }
        
        x++ ;
    }

    
}

void pirates_destroygrid( pirates_grid grid ) {
    
    int x = 0 ;
    
    int y = 0 ;
    
    pirates_clearthegrid(grid) ;
    
    while ( x < grid->GridX ) {
        
        y = 0 ;
        
        while ( y < grid->GridY ) {
            
            free(grid->cell_array[x][y]) ;
            
            y++ ;
        }
        x++ ;
    }

    x = 0 ;
    
    while ( x < grid->GridX ) {
        
        free(grid->cell_array[x]) ;
        
        x++ ;
    }
    
    free(grid->cell_array) ;
    
    free(grid) ;
    
}

pirates_triangle_array_buffer pirates_add_triangle_array( pirates_scene scene, pirates_triangles triangles, int numtrigs ) {
    
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

void pirates_remove_triangle_array( pirates_scene scene, pirates_triangle_array_buffer buffer ) {
    
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

void pirates_delete_triangle_arrays( pirates_scene scene ) {
    
    if ( scene->geom_data != NULL ) {
        
     while ( scene->geom_data->triangles != NULL ) { //scene->geom_data's before should always be NULL
        
        pirates_remove_triangle_array(scene,scene->geom_data) ;
         
     }
        
    }
}

pirates_bounding_box pirates_compute_bounding_box( pirates_triangle triangle ) {
    
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

void pirates_add_sphere_to_cell( int x, int y, int z, pirates_grid_cell*** cell_array, pirates_sphere sphere ) {
    
    pirates_add_sphere(&(cell_array[x][y][z].sphere_array),&(cell_array[x][y][z].numspheres),sphere) ;
    
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

#define pr_fieldX(x) (x * scene->field_x)

#define pr_fieldY(y) (y * scene->field_y)

#define pr_fieldZ(z) (z * scene->field_z)

void pirates_compute_grid( pirates_scene scene, pirates_sphere sphere ) {
    
    int i = 0 ;
    
    int j = 0 ;
    
    int k = 0 ;
    
    while ( i < scene->grid->GridX ) {
        
        j = 0 ;
        
        while ( j < scene->grid->GridY ) {
            
            k = 0 ;
            
            while ( k < scene->grid->GridZ ) {
            
            if ( CheckX(sphere->bounding_box.X, sphere->bounding_box.x, scene->grid->cell_array[i][j][k].bounding_box.X, scene->grid->cell_array[i][j][k].bounding_box.x) && CheckY(sphere->bounding_box.Y, sphere->bounding_box.y, scene->grid->cell_array[i][j][k].bounding_box.Y, scene->grid->cell_array[i][j][k].bounding_box.y) && CheckZ(sphere->bounding_box.Z, sphere->bounding_box.z, scene->grid->cell_array[i][j][k].bounding_box.Z, scene->grid->cell_array[i][j][k].bounding_box.z) ) {
                
                pirates_add_sphere_to_cell(i,j,k, scene->grid->cell_array, sphere) ;
               
            }
             
                k++ ;
            }
            
            j++ ;
        }
        
        i++ ;
    }
}

void pirates_compute_scene_bounding_box( pirates_scene scene, pirates_bounding_box bounding_box ) ;

void pirates_add_geom_to_bins( pirates_scene scene, pirates_scene_bin scene_bin ) {
    
    scene_bin->sphere_array = scene->sphere_array ;
    
    scene_bin->numspheres = scene->numspheres ;
    
    scene_bin->bounding_box = scene->bounding_box ;
}

void pirates_proc_scene( pirates_scene scene ) {
    
    pirates_triangle_array_buffer node = NULL ;
    
    pirates_triangle triangle ;
    
    int i = 0 ;
    
    if ( scene->geom_data != NULL ) {
        
        node = scene->geom_data ;
        
        while ( node != NULL ) {
            
            i = 0 ;
            
            while ( i < node->numtrigs ) {
                
                triangle = (&(node->triangles[i*10])) ;
                
                pirates_add_sphere(&(scene->sphere_array),&(scene->numspheres),pirates_compute_bounding_sphere(triangle,pirates_compute_bounding_box(triangle))) ;
                
                i++ ;
            }
            
            node = node->after ;
        }
        
    }
    
    if ( scene->sphere_array != NULL ) {
        
        i = 0 ;
        
        while ( i < scene->numspheres ) {
            
            pirates_compute_scene_bounding_box(scene, scene->sphere_array[i]->bounding_box) ;
            
            i++ ;
        }
        
        pirates_createbins(scene) ;
        
    }
    
}

pirates_scene_bin pirates_create_scene_bin(pirates_scene scene, int sort_min, int sort_max, int level_max) {
    
    pirates_scene_bin scene_bin = RKMem_NewMemOfType(pirates_scene_bin_object) ;
    
    scene_bin->sort_min = sort_min ;
    
    scene_bin->sort_max = sort_max ;
    
    scene_bin->level_max = level_max ;
    
    scene_bin->bin_id = 0 ;
    
    scene_bin->bin_type = 0 ;
    
    scene_bin->num_of_bins = 0 ;
    
    scene_bin->bin_array = NULL ;
    
    scene_bin->bigger_bin = NULL ;
    
    scene_bin->scene_bin_array = NULL ;
    
    scene_bin->num_of_bins_in_scene = 0 ;
    
    scene_bin->sphere_array = scene->sphere_array ;
    
    scene_bin->numspheres = scene->numspheres ;
    
    scene_bin->bounding_box = scene->bounding_box ;
    
    return scene_bin ;
    
}

typedef struct { int flag ; pirates_bounding_box bounding_box ; pirates_spheres sphere_array ; int numspheres ; } Group_object ;

typedef Group_object* Group ;

Group NewGroup( pirates_sphere sphere ) {
    
    Group new_group = RKMem_NewMemOfType(Group_object) ;
    
    new_group->sphere_array = RKMem_NewMemOfType(pirates_sphere) ;
    
    new_group->numspheres = 1 ;
    
    new_group->sphere_array[0] = sphere ;
    
    new_group->flag = 0 ;
    
    new_group->bounding_box = sphere->bounding_box ;
    
    return new_group ;
}

void pirates_addSphere_to_Group( pirates_sphere sphere,  Group Group_ ) {
    
    Group_->numspheres++ ;
    
    Group_->sphere_array = RKMem_Realloc(Group_->sphere_array, Group_->numspheres, Group_->numspheres-1, pirates_sphere, 1) ;
    
    Group_->sphere_array[Group_->numspheres-1] = sphere ;
    
    Group_->bounding_box.X += sphere->bounding_box.X ;
    
    Group_->bounding_box.x += sphere->bounding_box.x ;
    
    Group_->bounding_box.Y += sphere->bounding_box.Y ;
    
    Group_->bounding_box.y += sphere->bounding_box.y ;
    
    Group_->bounding_box.Z += sphere->bounding_box.Z ;
    
    Group_->bounding_box.z += sphere->bounding_box.z ;
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

pirates_bin pirates_new_bin( int type, pirates_bin bigger_bin, Group Group_ ) {
    
    pirates_bin new_bin = RKMem_NewMemOfType(pirates_bin_object) ;
    
    new_bin->bin_type = type ;
    
    new_bin->num_of_bins = 0 ;
    
    new_bin->bin_array = NULL ;
    
    new_bin->bigger_bin = bigger_bin ;
    
    new_bin->bin_id = !(bigger_bin->bin_id) ;
    
    new_bin->numspheres = Group_->numspheres ;
    
    new_bin->sphere_array = Group_->sphere_array ;
    
    new_bin->bounding_box = Group_->bounding_box ;
    
    return new_bin ;
}

void pirates_add_bin( Group Group_, pirates_bin bin, pirates_bins* new_bins, int* num_of_new_bins, pirates_scene_bin scene_bin ) {
    
    pirates_bin new_bin = pirates_new_bin(1,bin,Group_) ;
    
    pirates_addBin(new_bin,new_bins,num_of_new_bins) ;
    
    pirates_addBin(new_bin,&(scene_bin->scene_bin_array),&(scene_bin->num_of_bins_in_scene)) ;
    
    pirates_addBin(new_bin,&(bin->bin_array),&(bin->num_of_bins)) ;

}

void pirates_bins_good_sort( pirates_bin bin, pirates_bins* new_bins, int* num_of_new_bins, pirates_scene_bin scene_bin ) {
    
    int num_of_groups = (int) (bin->numspheres * 0.05f) ;
    
    if (num_of_groups <= 0) num_of_groups = 1 ;
    
    float last_float = ARandomNumber((bin->numspheres-1)) ;
    
    float new_float = ARandomNumber((bin->numspheres-1)) ;
    
    float* float_array = NULL ;
    
    int num_of_floats = 0 ;
    
    int i = 0 ;
    
    int j = 0 ;
    
    int k = 0 ;
    
    while (i < num_of_groups) {
        
        while ( new_float == last_float ) {
        
        new_float = ARandomNumber((bin->numspheres-1)) ;
            
        }
        
        pirates_addfloat(new_float, &float_array, &num_of_floats) ;
        
        last_float = new_float ;
        
        i++ ;
    }
    
    Group* Group_array = NULL ;
    
    int num_of_Groups = 0 ;

    i = 0 ;
    
    while ( i < num_of_floats ) {
        
        pirates_addGroup(NewGroup(bin->sphere_array[(int)float_array[i]]), &Group_array, &num_of_Groups) ;
        
        i++ ;
    }
    
    free( float_array ) ;
    
    i = 0 ;
    
    while ( i < num_of_groups ) {
        
        j = 0 ;
    
        while ( j < num_of_groups ) {
            
            if ( i != j ) {
                
                if ( !(Group_array[j]->flag) ) {
                    
                    if ( CheckXYZ(Group_array[i]->bounding_box, Group_array[j]->bounding_box) ) {
                        
                        k = 0 ;
                        
                        while ( k < Group_array[j]->numspheres ) {
                            
                            pirates_addSphere_to_Group(Group_array[j]->sphere_array[k], Group_array[i]) ;
                            
                            k++ ;
                        }
                        
                        //Group_array[j]->flag = 1 ;
                    }
                    
                }
                
            }
                
            j++ ;
        }
        
        i++ ;
    }
    
    Group* Group_array_2 = NULL ;
    
    int num_of_Groups_2 = 0 ;
    
    i = 0 ;
    
    while ( i < num_of_groups ) {
        
        if ( !(Group_array[i]->flag) )
        
        pirates_addGroup(Group_array[i], &Group_array_2, &num_of_Groups_2) ;
        
        i++ ;
    }
    
    free(Group_array) ;
    
    Group_array = Group_array_2 ;
    
    num_of_groups = num_of_Groups_2 ;
    
    float size_base = 1.0f/num_of_groups ;
    
    i = 0 ;
    
    while ( i < num_of_groups ) {
        
        Group_array[i]->bounding_box.X += ( Group_array[i]->bounding_box.X *= (size_base - (Group_array[i]->bounding_box.X/bin->bounding_box.X)) ) ;
        
        Group_array[i]->bounding_box.Y += ( Group_array[i]->bounding_box.Y *= (size_base - (Group_array[i]->bounding_box.Y/bin->bounding_box.Y)) ) ;
        
        Group_array[i]->bounding_box.Z += ( Group_array[i]->bounding_box.Z *= (size_base - (Group_array[i]->bounding_box.Z/bin->bounding_box.Z)) ) ;
        
        //if ( CheckInside(Group_array[i]->bounding_box, bin->bounding_box) ) {
            
            pirates_add_bin(Group_array[i],bin,new_bins,num_of_new_bins,scene_bin) ;
            
            free(Group_array[i]) ;
        //}
        
        i++ ;
    }
    
    i = 0 ;
    
    while ( i < (*num_of_new_bins) ) {
        
        j = 0 ;
        
        while ( j < bin->numspheres ) {
            
            if ( CheckX(bin->sphere_array[j]->bounding_box.X, bin->sphere_array[j]->bounding_box.x, (*new_bins)[i]->bounding_box.X, (*new_bins)[i]->bounding_box.x) && CheckY(bin->sphere_array[j]->bounding_box.Y, bin->sphere_array[j]->bounding_box.y, (*new_bins)[i]->bounding_box.Y, (*new_bins)[i]->bounding_box.y) && CheckZ(bin->sphere_array[j]->bounding_box.Z, bin->sphere_array[j]->bounding_box.z, (*new_bins)[i]->bounding_box.Z, (*new_bins)[i]->bounding_box.z) ) {
                
                pirates_add_sphere(&((*new_bins)[i]->sphere_array),&((*new_bins)[i]->numspheres),bin->sphere_array[j]) ;
                
            }
            
            j++ ;
        }
        
        i++ ;
    }
    
    free(Group_array) ;
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
        
        if ( ((*level_bins)[i]->numspheres > scene_bin->sort_min) && ((*level_bins)[i]->numspheres < scene_bin->sort_max) )
            pirates_bins_good_sort((*level_bins)[i],&new_bins,&num_of_new_bins,scene_bin) ;
        
        
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

    pirates_add_geom_to_bins(scene, scene->scene_bin) ;
    
    pirates_makebins(scene->scene_bin) ;
}

void pirates_compute_scene_bounding_box( pirates_scene scene, pirates_bounding_box bounding_box ) {
    
    if ( bounding_box.X > scene->bounding_box.X ) scene->bounding_box.X = bounding_box.X ;
    
    if ( bounding_box.x < scene->bounding_box.x ) scene->bounding_box.x = bounding_box.x ;
    
    if ( bounding_box.Y > scene->bounding_box.Y ) scene->bounding_box.Y = bounding_box.Y ;
    
    if ( bounding_box.y < scene->bounding_box.y ) scene->bounding_box.y = bounding_box.y ;
    
    if ( bounding_box.Z > scene->bounding_box.Z ) scene->bounding_box.Z = bounding_box.Z ;
    
    if ( bounding_box.z < scene->bounding_box.z ) scene->bounding_box.z = bounding_box.z ;
}

Ray newray(RKMVector origin , RKMVector direction ) {
    
    Ray newray ;
    
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

Raycam newcam(RKMVector position, RKMVector focus) {
    
    Raycam newraycam ;
    
    newraycam = (Raycam) malloc(sizeof(Raycam_object)) ;
    
    newraycam->position = RKMath_NewVectorMalloc(3) ;
    
    RKMath_VectorCopy(newraycam->position, position) ;
    
    newraycam->direction = RKMath_NewVectorMalloc(3) ;
    
    RKMath_Sub(newraycam->direction, position, focus, 3) ;
    
    RKMath_Norm(newraycam->direction, newraycam->direction, 3) ;
    
    return newraycam ;
    
}

void freecam( Raycam cam ) {
    
    free( cam->position ) ;
    
    free( cam->direction ) ;
    
    free( cam ) ;
}

Ray CastRay(pirates_scene scene, Raycam raycam, float x, float y) {
    
    RKMath_Vectorit(direction, (x * (0.5/scene->res_x)) - (0.5/2.0f), (y * (0.5/scene->res_y)) - (0.5/2.0f), 1) ;
    
    return newray(raycam->position, direction) ;
}

float Sphere_intersection(Ray ray, pirates_sphere sphere) {
    
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

double Trig_intersection(Ray ray, RKMVector vert1, RKMVector vert2, RKMVector vert3) {
    
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

pirates_sphere pirates_find_sphere(pirates_scene scene, Ray r, pirates_spheres spheres, int numspheres) {
    
    int i = 0 ;
    
    int hit = -1 ;
    
    double t = 0 ;
    
    double trig_test = scene->draw_distance ;
    
    double sphere_test = scene->draw_distance ;
    
    float* sphere_array = NULL ;
    
    int num_of_hits = 0 ;
    
    while (i < numspheres) {
        
        t = Sphere_intersection(r, spheres[i]) ;
        
        if (t) {
            
            pirates_addfloat(i,&sphere_array,&num_of_hits) ;
    
        }
        
        i++ ;
    }
    
    if (!num_of_hits) {
        
        free(sphere_array) ;
        
        return NULL ;
    
    }
    
    i = 0 ;
    
    int hit2 = -1 ;
    
    pirates_triangle triangle = NULL ;
    
    RKMVector vert1 = NULL ;
    
    RKMVector vert2 = NULL ;
    
    RKMVector vert3 = NULL ;
    
    while ( i < num_of_hits ) {
    
    hit = (int) sphere_array[i] ;
        
    triangle = spheres[hit]->triangle ;
    
    vert1 = &(triangle[pr_V1X]) ;
    
    vert2 = &(triangle[pr_V2X]) ;
    
    vert3 = &(triangle[pr_V3X]) ;
    
    t = Trig_intersection(r,vert1,vert2,vert3) ;
    
    if ((t) && (0 < t < trig_test) ) {
        
        trig_test = t ;
        
        hit2 = i ;
    }
        i++ ;
    
    }
    
    hit = -1 ;
    
    if (!(hit2+1)) return NULL ;
    
    hit = (int) sphere_array[hit2] ;
    
    free(sphere_array) ;
    
    spheres[hit]->t = t ;
    
    if ((hit+1)) return spheres[hit] ;
    
    return NULL ;
}

float fracf(float x) {
    
    return x - floorf(x) ;
}

float box_intersection( Ray r, pirates_bin box ) {
    
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


pirates_Material pirates_find_object_via_bins( pirates_scene scene, Ray r ) {

    pirates_Material material = NULL ;
    
    pirates_sphere sphere = NULL ;
    
    pirates_bins bin_array = NULL ;
    
    int num_of_bins = 0 ;
    
    pirates_spheres sphere_array = NULL ;
    
    int numsphers = 0 ;
    
    material = pirates_newmaterial(Colorit(0.0, 0.0, 0.0)) ;
    
    pirates_scene_bin scene_bin = scene->scene_bin ;
    
    if (box_intersection(r,(pirates_bin)scene_bin)) {
        
        material->color = Color_add(material->color, Colorit(0.2, 0.2, 0.2)) ;
        
        numsphers = scene_bin->numspheres ;
        
        sphere_array = RKMem_CArray(numsphers, pirates_sphere) ;
        
        memcpy(sphere_array, scene_bin->sphere_array, (numsphers * sizeof(pirates_sphere))) ;
        
        num_of_bins = scene_bin->num_of_bins ;
        
        bin_array = RKMem_CArray(num_of_bins, pirates_bin) ;
        
        memcpy(bin_array, scene_bin->bin_array, (num_of_bins * sizeof(pirates_bin))) ;
        
        int i = 0 ;
        
        while ( i < num_of_bins ) {
           
            if (box_intersection(r,bin_array[i])) {
            
                material->color = Color_add(material->color, Colorit(0.0, 0.0, 0.2)) ;
                
             if ( bin_array[i]->num_of_bins > 0 ) {
                
                 RKMem_AddToArray(bin_array,bin_array[i]->bin_array,num_of_bins,bin_array[i]->num_of_bins,pirates_bin) ;
                 
                 num_of_bins += bin_array[i]->num_of_bins ;
             }
            
             if ( bin_array[i]->numspheres > 0 ) {
                    
                 RKMem_AddToArray(sphere_array,bin_array[i]->sphere_array,numsphers,bin_array[i]->numspheres,pirates_sphere) ;
                    
                 numsphers += bin_array[i]->numspheres ;
             }
                
            }
            
            i++ ;
        }
        
       sphere = pirates_find_sphere(scene,r,sphere_array,numsphers) ;
        
       if (sphere != NULL) material->color = Color_add(material->color, pirates_get_material(scene, sphere->triangle)->color) ;
    }
    
    free(bin_array) ;
    
    free(sphere_array) ;
    
    return material ;
}

pirates_Material pirates_get_first_intersection( pirates_scene scene, Ray r ) {
    
     return pirates_find_object_via_bins(scene,r) ;
}

pirates_Material pirates_get_material(pirates_scene scene, pirates_triangle triangle ) {
    
    return scene->materials[(int)(triangle[pr_M] - 1 )] ;
}

raycolor pirates_ray_cast_func(Ray r, pirates_scene scene) {
    
     raycolor fincolor = Colorthat(0) ;
    
     pirates_Material sphere = pirates_get_first_intersection( scene, r ) ;
    
     if (sphere == NULL) return fincolor ;
    
     fincolor = Color_add(fincolor, sphere->color) ;
    
     free(sphere) ;
    
     return fincolor ;
}

void pirates_render(pirates_scene scene) {
    
    int i = 0 ;
    
    int j = 0 ;
    
    while (j < scene->res_y) {
        i = 0 ;
        
        while (i < scene->res_x) {
            
            cnpoint_SetColor_and_free(scene->scene, i, j, Colorout(Color_clamp( pirates_ray_cast_func(CastRay(scene, scene->Camera, i, j),scene)))) ;
            
            i++ ;
        }
        
        j++ ;
    }
    
   
}
