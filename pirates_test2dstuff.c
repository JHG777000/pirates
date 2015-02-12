//
//  pirates_test2dstuff.c
//  pirates
//
//  Created by Jacob Gordon on 12/21/14.
//  Copyright (c) 2014 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PlaceToDrawStuff.h"
#include "pirates.h"

static float oldX = 0 ;

static float oldY = 0 ;

static int _NumOfPoints_ = 0 ;

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

static int ARandomNumber( int randmax ) {
    
    static long state = 0 ;
    
    static int init = 0 ;
    
    int randval = 0 ;
    
    if ( randmax == -1 ) init = 0 ;
    
    randmax++ ;
    
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

static void AddFloat( float float_, float** float_array_ptr, int* num_of_floats ) {
    
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

static RKMVector AddPoint( RKMVector Point, RKMVector Points ) {
    
    AddFloat(Point[RKM_X],&Points,&_NumOfPoints_) ;
    
    AddFloat(Point[RKM_Y],&Points,&_NumOfPoints_) ;
    
    AddFloat(Point[RKM_Z],&Points,&_NumOfPoints_) ;
    
    return Points ;
}

static RKMVector AddTestPoints( int NumOfPoints ) {
    
    RKMVector Points = NULL ;
    
    RKMath_NewVector(Point, 3) ;
    
    int i = 0 ;
    
    while ( i < NumOfPoints ) {
        
      Point[RKM_X] = RandomNumber(300,1024) ;
        
      Point[RKM_Y] = RandomNumber(500,1024) ;
        
      Point[RKM_Z] = RandomNumber(100,255) ;
        
      Points = AddPoint(Point, Points) ;
        
        i++ ;
    }
    
    return Points ;
}

static void DrawPoints( codename_scene scene, RKMVector Points, int NumOfPoints ) {
    
    int i = 0 ;
    
    RKMVector Point ;
    
    while ( i < NumOfPoints ) {
        
        Point = (&(Points[i*3])) ;
        
        cnpoint_Rect(scene, 5, 5, Point[RKM_X], Point[RKM_Y], Point[RKM_Z], Point[RKM_Z], Point[RKM_Z]) ;
        
        i++ ;
    }

}

void pirates_test2dstuff(codename_scene scene, float MouseX, float MouseY) {
    
    static int draw = 1 ;
    
    int NumOfPoints = 10 ;
    
    RKMVector Points = NULL ;
    
    if ( draw ) {
        
        Points = AddTestPoints( NumOfPoints ) ;
        
        DrawPoints(scene, Points, NumOfPoints) ;
        
        free(Points) ;
        
        _NumOfPoints_ = 0 ;
        
        draw-- ;
    }
    
    if ((MouseX != oldX) || (MouseY != oldY)) {
        
        oldX = MouseX ;
        
        oldY = MouseY ;
        
        SetDisplayNeedsToUpdate() ;
        
        draw = 1 ;
        
    }
 
}