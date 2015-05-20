//
//  Game.c
//  IDKApp
//
//  Created by Jacob Gordon on 4/17/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#include "Game.h"

static int init = 0 ;

static pirates3d_camera camera = NULL ;

static pirates3d_scene scene3d = NULL ;

static pirates3d_transform transform = NULL ;

static pirates3d_primitive_array primitive_array = NULL ;

static pirates3d_primitive_array primitive_array_2 = NULL ;

void GameInit( IDKDrawArea Area ) {
  
    RKMath_Vectorit(position, 0.0, 0.0 , 0.0) ;
    
    RKMath_Vectorit(focus, 0.0, 0.0, 0.0) ;
    
    pirates_scene scene_rt = pirates_new_scene(Area, 0, 1000000.0f, position, focus, 512, 512, 1, 1, 5) ;
    
    camera = pirates3d_new_camera(0, 0, 0, 0, 0, 0) ;
    
    scene3d = pirates3d_new_3dscene(camera,scene_rt) ;
    
    pirates3d_new_material(scene3d, "red", 1, 0, 0) ;
    
    pirates3d_material blue = pirates3d_new_material(scene3d, "blue", 0, 1, 0) ;
    
    float data[11] = { 0.0, 0.0, 0.9, 0.35, 0.35, 0.7, -0.35, 0.35, -0.7, 0, 1 } ;

    primitive_array = pirates3d_new_primitive_array(scene3d, data, Triangles, pirates3d_copy_data, 1) ;
    
    pirates3d_apply_material(blue, primitive_array) ;
    
    transform = pirates3d_new_transform() ;
    
    pirates3d_add_transform(transform, Rotation, chain, 0, 1, 0, 1) ;
    
    pirates3d_add_transform(transform, Rotation, chain, 1, 0, 0, 1) ;
    
    primitive_array_2 = pirates3d_new_sphere(scene3d, 50, 1.4f) ;
    
    pirates3d_material red = pirates3d_get_material(scene3d, "red") ;
    
    pirates3d_apply_material(red, primitive_array_2) ;
    
    init++ ;
    
}

void GameRun( IDKDrawArea Area ) {
    
    if (!init) GameInit(Area) ;
    
    pirates3d_apply_transform(transform, primitive_array) ;
    
    pirates3d_apply_transform(transform, primitive_array_2) ;
    
    static float xpos, ypos, zpos = 0 ;
    
    if ( IDK_GetKey(w_key) ) zpos += 0.01 ;
    
    if ( IDK_GetKey(s_key) ) zpos -= 0.01 ;
    
    if ( IDK_GetKey(a_key) ) xpos -= 0.01 ;
    
    if ( IDK_GetKey(d_key) ) xpos += 0.01 ;
    
    if ( IDK_GetKey(q_key) ) ypos -= 0.01 ;
    
    if ( IDK_GetKey(e_key) ) ypos += 0.01 ;
    
    static float xpos2, ypos2, zpos2 = 0 ;
    
    if ( IDK_GetKey(numpad_8) ) zpos2 += 0.01 ;
    
    if ( IDK_GetKey(numpad_5) ) zpos2 -= 0.01 ;
    
    if ( IDK_GetKey(numpad_4) ) xpos2 -= 0.01 ;
    
    if ( IDK_GetKey(numpad_6) ) xpos2 += 0.01 ;
    
    if ( IDK_GetKey(numpad_7) ) ypos2 -= 0.01 ;
    
    if ( IDK_GetKey(numpad_9) ) ypos2 += 0.01 ;
    
    pirates3d_change_camera(camera,xpos,ypos,zpos,xpos2,ypos2,zpos2) ;
    
    pirates3d_draw_3dscene(scene3d) ;
}