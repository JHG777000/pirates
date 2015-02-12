//
//  PlaceToDrawStuff.c
//  MacGLEssentials
//
//  Created by Jacob Gordon on 6/3/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "RKMath.h"
#include "codename.h"
#include "codename2D.h"
#include "RKWindow.h"
#include "PlaceToDrawStuff.h"

void pirates_teststuff(codename_scene scene, float MouseX, float MouseY) ;

void pirates_test2dstuff(codename_scene scene, float MouseX, float MouseY) ;

static int forceupdate = 0 ;

static float MouseX = 0 ;

static float MouseY = 0 ;

static float oldX = 0 ;

static float oldY = 0 ;

 void GetFPSFuncinDev( void ) {
    
    static int init = 0 ;
     
    static float numofframes = 0 ;
    
    static long time_count = 0 ;
     
    long seconds = 0 ;
     
    float fps = 0 ;
     
     if (!init) {
         
         time_count = (unsigned)time(NULL) ;
         
         init++ ;
     }
     
    numofframes++ ;
    
    seconds = ((unsigned)time(NULL) - time_count) ;
     
    if (  seconds >= 1 ) {
        
        fps = (numofframes / seconds) ;
        
        printf("%f, and %ld, and %f\n", fps, seconds, numofframes) ;
        
        seconds = 0 ;
        
        numofframes = 0 ;
        
        time_count = (unsigned)time(NULL) ;
        
    }
    
}

int MyDrawFuntion( codename2D_Canvus MainCanvus ) {
    
    int ray = 1 ;
    
    if (GetdoDisplayNeedUpdate()) {
        
        JHGPixels_Reset_To_Monocolor(MainCanvus->scene->pixelscene, 0) ;
        
    }
    
    codename_ChangeBackGroundColor(MainCanvus->scene, 0, 255, 0) ;
    
    if (!ray) {
    
    /*cnpoint_linesegment(MainCanvus->scene,100,100,100,200,255,0,0) ;
    
    cnpoint_linesegment(MainCanvus->scene,100,200,200,200,255,0,0) ;
    
    cnpoint_linesegment(MainCanvus->scene,200,200,200,100,255,0,0) ;
    
    cnpoint_linesegment(MainCanvus->scene,200,100,100,100,255,0,0) ;
    
    cn_SetPoint(MainCanvus->scene, 100, 100, 0, 0, 255) ;
    
    cn_SetPoint(MainCanvus->scene, 100, 200, 0, 0, 255) ;
    
    cn_SetPoint(MainCanvus->scene, 200, 200, 0, 0, 255) ;
    
    cn_SetPoint(MainCanvus->scene, 200, 100, 0, 0, 255) ;
    
    cnpoint_wikiCircle(MainCanvus->scene, MouseX, MouseY, 10, MouseY + 100, MouseY, MouseX) ;
    
    cnpoint_linesegment(MainCanvus->scene,100,200,100,100,MouseY,MouseX,MouseY) ;
    
    cnpoint_wikiArc(MainCanvus->scene, MouseX+100, MouseY, 10,180,360, MouseY + 100, MouseY, MouseX) ;
    
    cnpoint_Curve(MainCanvus->scene,100,100,MouseX,MouseY,400,400,0,255,0) ;
    
    cnpoint_String(MainCanvus->scene,"ABCDEFGHIJKLMNOPQRSTUVWXYZ.!?1234567890", 4, 100, 700, 0, 0, 255) ;
    
    cnpoint_String(MainCanvus->scene,"HELLO WORLD!!!!", 4, MouseX, MouseY, 255, 0, 0) ;
    
    cnpoint_printchar(MainCanvus->scene,'A', 4, 100, 100, 0, 255, 0) ;
    
    cnpoint_Rect(MainCanvus->scene, 2, 2, 400, 400, 0, 0, 255) ;
    
    cnpoint_Rect(MainCanvus->scene, 100, 100, 800, 800, 255, 255, 255) ;
    
    cnpoint_wikiCircle(MainCanvus->scene, MouseX, 300, MouseY-600, MouseY + 500, MouseY, MouseX) ;
    
    cnpoint_linesegment(MainCanvus->scene,MouseX,MouseY,200,200,255,255,255) ;
    
    cnpoint_linesegment(MainCanvus->scene,0.3,0.3,0.3,0.4,255,0,0) ;
    
    cnpoint_linesegment(MainCanvus->scene,0.3,0.4,0.4,0.4,255,0,0) ;
    
    cnpoint_linesegment(MainCanvus->scene,0.4,0.4,0.4,0.3,255,0,0) ;
    
    cnpoint_linesegment(MainCanvus->scene,0.4,0.3,0.3,0.3,255,0,0) ;
    */
        
    pirates_test2dstuff(MainCanvus->scene,MouseX,MouseY) ;
        
    } else {
        
    pirates_teststuff(MainCanvus->scene,MouseX,MouseY) ;
        
    //SetDisplayNeedsToUpdate() ;
    
    }
 
    GetFPSFuncinDev() ;
    
    return 0 ;

}

void theMouse( float x, float y ) {

    MouseX = x ;
    
    MouseY = y ;    
}

void SetDisplayNeedsToUpdate( void ) {
    
    forceupdate++ ;
}

int GetdoDisplayNeedUpdate(void) {
    
    if ( forceupdate ) {
        
        forceupdate = 0 ;
        
        return 1 ;
    }
    
    if ((MouseX != oldX) || (MouseY != oldY)) {
        
        oldX = MouseX ;
        
        oldY = MouseY ;
        
        return 1 ;
        
    }
    
    return 0 ;
}
