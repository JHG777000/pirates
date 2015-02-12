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
    
    RKMath_Vectorit(position, 0.1, 0.1 , -12.0) ;
    
    RKMath_Vectorit(focus, 0.0, 0.0, 0.0) ;
    
    pirates_scene scene3d = pirates_new_scene(scene, 1000000.0f, position, focus, 1024, 1024, 10, 35, 1) ;
    
    pirates_Material blue = pirates_newmaterial(Colorit(0.0, 1.0, 0.0)) ;
    
    pirates_Material red = pirates_newmaterial(Colorit(1.0, 0.0, 0.0)) ;
    
    int blue_id = pirates_addmaterial(scene3d, blue) ;
    
    int red_id = pirates_addmaterial(scene3d, red) ;
    
    float triangle[10] = { 0.0, 0.0, 0.9, 0.35, 0.35, 0.7, -0.35, 0.35, -0.7, red_id  } ;
    
    float triangle2[10] = { 0.0+(MouseX/1024), 0.0, 0.9+(MouseY/1024), 0.35+(MouseX/1024), 0.35, 0.7+(MouseY/1024), -0.35+(MouseX/1024), 0.35, -0.7+(MouseY/1024), blue_id  } ;
    
    //float triangle3[10] = { 0.0, 0.0, 0.9, 0.35, 0.35, 0.7, -0.35, 0.35, -0.7, blue_id  } ;
    
    pirates_add_triangle_array(scene3d, triangle, 1) ;
    
    pirates_add_triangle_array(scene3d, triangle2, 1) ;
    
    //int i = 0 ;
    
    //int j = 0 ;
    /*
    while ( i < 100 ) {
        
        j = RandomNumber(1, 9) ;
        
        triangle3[0] += (j/10) ;
        
        triangle3[3] += (j/10) ;
        
        triangle3[6] += (j/10) ;
        
        pirates_add_triangle_array(scene3d, triangle3, 1) ;
        
        i++ ;
    }
    */
    pirates_proc_scene(scene3d) ;
    
    pirates_render(scene3d) ;
    
    pirates_destroy_scene(scene3d) ;
    
}