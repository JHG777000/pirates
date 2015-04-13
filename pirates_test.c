//
//  pirates_test.c
//  pirates
//
//  Created by Jacob Gordon on 11/15/14.
//  Copyright (c) 2014 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "pirates.h"

static pirates_scene scene3d = NULL ;

static RKMVector triangle = NULL ;

static RKMVector triangle2 = NULL ;

static int blue_id ;

static int red_id ;

static pirates_primitive_array triangle_one = NULL ;

static pirates_primitive_array triangle_two = NULL ;

void pirates_init(IDKDrawArea Area) {
    
    RKMath_Vectorit(position, 0.1, 0.1 , -12.0) ;
    
    RKMath_Vectorit(focus, 0.0, 0.0, 0.0) ;
    
    scene3d = pirates_new_scene(Area, 0, 1000000.0f, position, focus, 512, 512, 1, 35, 1) ;
    
    pirates_Material blue = pirates_newmaterial(Colorit(0.0, 1.0, 0.0)) ;
    
    pirates_Material red = pirates_newmaterial(Colorit(1.0, 0.0, 0.0)) ;
    
    blue_id = pirates_addmaterial(scene3d, blue) ;
    
    red_id = pirates_addmaterial(scene3d, red) ;
    
    triangle = RKMath_NewVectorMalloc(11) ;
    
    triangle2 = RKMath_NewVectorMalloc(11) ;
    
    float data[11] = { 0.0, 0.0, 0.9, 0.35, 0.35, 0.7, -0.35, 0.35, -0.7, red_id, 0 } ;
    
    float data2[11] = { 0.0, 0.0, 0.9, 0.35, 0.35, 0.7, -0.35, 0.35, -0.7, blue_id, 0  } ;
    
    RKMath_Equal(triangle, data, 11) ;
    
    RKMath_Equal(triangle2, data2, 11) ;
    
    triangle_one = pirates_new_primitive_array(triangle) ;
    
    triangle_two = pirates_new_primitive_array(triangle2) ;
    
    pirates_add_triangle_array(scene3d, triangle_one, 1, 1) ;
    
    pirates_add_triangle_array(scene3d, triangle_two, 1, 1) ;
}

void pirates_teststuff(IDKDrawArea Area) {
    
    static float xpos, ypos, zpos = 0 ;
    
    if ( IDK_GetKey(w_key) ) zpos += 0.01 ;
    
    if ( IDK_GetKey(s_key) ) zpos -= 0.01 ;
    
    if ( IDK_GetKey(a_key) ) xpos -= 0.01 ;
    
    if ( IDK_GetKey(d_key) ) xpos += 0.01 ;
    
    if ( IDK_GetLeftMouseButton() ) ypos -= 0.01 ;
    
    if ( IDK_GetRightMouseButton() ) ypos += 0.01 ;
    
    pirates_change_camera(scene3d,xpos,ypos,zpos,0,0,0) ;
    
    static float xpos2, ypos2, zpos2 = 0 ;
    
    if ( IDK_GetKey(up_key) ) ypos2 += 0.01 ;
    
    if ( IDK_GetKey(down_key) ) ypos2 -= 0.01 ;
    
    if ( IDK_GetKey(left_key) ) xpos2 -= 0.01 ;
    
    if ( IDK_GetKey(right_key) ) xpos2 += 0.01 ;

    //if ( IDK_GetLeftMouseButton() ) ypos2 -= 0.01 ;
    
    //if ( IDK_GetRightMouseButton() ) ypos2 += 0.01 ;
    
    float data[11] = { 0.0+xpos2, 0.0+ypos2, 0.9+zpos2, 0.35+xpos2, 0.35+ypos2, 0.7+zpos2, -0.35+xpos2, 0.35+ypos2, -0.7+zpos2, red_id, 1 } ;
    
    float data2[11] = { 0.0+(IDK_GetMouseX()), 0.0, 0.9+(IDK_GetMouseY()), 0.35+(IDK_GetMouseX()), 0.35, 0.7+(IDK_GetMouseY()), -0.35+(IDK_GetMouseX()), 0.35, -0.7+(IDK_GetMouseY()), blue_id, 1  } ;
    
    RKMath_Equal(triangle, data, 11) ;
    
    RKMath_Equal(triangle2, data2, 11) ;
    
    pirates_proc_scene(scene3d) ;
    
    pirates_render(scene3d) ;
    
    pirates_destroy_bins(scene3d) ;
    
    }

void pirates_shutdown(void) {
    
    pirates_destroy_primitive_array(triangle_one) ;
    
    pirates_destroy_primitive_array(triangle_two) ;
    
    pirates_proc_scene(scene3d) ;
    
    pirates_destroy_scene(scene3d) ;
}