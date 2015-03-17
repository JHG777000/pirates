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

static int RandomNumber( int randmin, int randmax ) {
    
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
    
    randval = (randval < randmin) ? RandomNumber(randmin, randmax) : randval ;
    
    return  randval ;
    
}

void pirates_teststuff(codename_scene scene, float MouseX, float MouseY) {
    
    static pirates_scene scene3d = NULL ;
    
    static int count = 0 ;
    
    static int init = 0 ;
    
    RKMath_Vectorit(position, 0.1, 0.1 , -12.0) ;
    
    RKMath_Vectorit(focus, 0.0, 0.0, 0.0) ;
    
    if (!init) scene3d = pirates_new_scene(scene, 1000000.0f, position, focus, 1024, 1024, 1, 35, 1) ;
    
    pirates_change_camera(scene3d,0,MouseY/1024,MouseX/10240) ;
    
    static pirates_Material blue ;
    
    static pirates_Material red ;
    
    if (!init) blue = pirates_newmaterial(Colorit(0.0, 1.0, 0.0)) ;
    
    if (!init) red = pirates_newmaterial(Colorit(1.0, 0.0, 0.0)) ;
    
    static int blue_id ;
    
    static int red_id ;
    
    if (!init) blue_id = pirates_addmaterial(scene3d, blue) ;
    
    if (!init) red_id = pirates_addmaterial(scene3d, red) ;
    
    static RKMVector triangle ;
    
    static RKMVector triangle2 ;
    
    if (!init)  triangle = RKMath_NewVectorMalloc(11) ;
    
    if (!init)  triangle2 = RKMath_NewVectorMalloc(11) ;
    
    float data[11] = { 0.0, 0.0, 0.9, 0.35, 0.35, 0.7, -0.35, 0.35, -0.7, red_id, 0 } ;
    
    float data2[11] = { 0.0+(MouseX/1024), 0.0, 0.9+(MouseY/1024), 0.35+(MouseX/1024), 0.35, 0.7+(MouseY/1024), -0.35+(MouseX/1024), 0.35, -0.7+(MouseY/1024), blue_id, 1  } ;
    
    RKMath_Equal(triangle, data, 11) ;
    
    RKMath_Equal(triangle2, data2, 11) ;
    
    static pirates_primitive_array triangle_one ;
    
    static pirates_primitive_array triangle_two ;
    
    if (!init) triangle_one = pirates_new_primitive_array(triangle) ;
    
    if (!init) triangle_two = pirates_new_primitive_array(triangle2) ;
    
    if (!init) pirates_add_triangle_array(scene3d, triangle_one, 1, 1) ;
    
    if (!init) pirates_add_triangle_array(scene3d, triangle_two, 1, 1) ;
    
    if (!init) init++ ;
    
    if ( count > 10 ) {
        
        pirates_destroy_primitive_array(triangle_one) ;
        
        pirates_destroy_primitive_array(triangle_two) ;
    }
    
    pirates_proc_scene(scene3d) ;
    
    pirates_render(scene3d) ;
    
    if ( count < 10 ) pirates_destroy_bins(scene3d) ;
    
    if ( count > 10 ) {
       
    pirates_destroy_scene(scene3d) ;
    
    count = 0 ;
        
    init-- ;
        
    }
    
    count++ ;
    
}