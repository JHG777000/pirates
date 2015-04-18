//
//  PlaceToDrawStuff.c
//  MacGLEssentials
//
//  Created by Jacob Gordon on 6/3/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "PlaceToDrawStuff.h"

void pirates_teststuff(IDKDrawArea Area) ;

static void xytest( int* x, int* y, int pixel ) {
    
    const int max = 100 ;
    
    (*x) = pixel ;
    
    if ((*x) >= max) {
        
        (*y) = (*x) / max ;
        
        (*x) = (*x) - (max * (*y)) ;
    }
}

void MyDrawArea( IDKDrawArea Area ) {
    
    int ray = 1 ;
    
    if (!ray) {
        
    int x,y = 0 ;
    static int pixel = 0 ;
     
    if (IDK_GetRightMouseButton()) pixel++ ;
        
    if (IDK_GetLeftMouseButton()) pixel-- ;
    
    if (pixel >= 10000) pixel = 0 ;
        
    xytest(&x, &y, pixel) ;
        
    IDK_DisplayInt(Area, x, 0.22, 0.2, 0.2, 1, 1, 1) ;
        
    IDK_DisplayInt(Area, y, 0.22, 0.5, 0.2, 1, 1, 1) ;
        
    IDK_DisplayInt(Area, pixel, 0.22, 0.9, 0.2, 1, 1, 1) ;
        
    IDK_String(Area, "HELLO WORLD!!!!", 0.22, IDK_GetMouseX(), IDK_GetMouseY(), 0, 1, 0) ;
    
    if (IDK_GetRightMouseButton()) IDK_Rect(Area, 0.15, 0.20, 0.80, 0.5, 0, 1, 0) ;
    
    if (IDK_GetLeftMouseButton()) IDK_Rect(Area, 0.15, 0.20, 0.10, 0.5, 1, 0, 0) ;
    
    if (IDK_GetMiddleMouseButton()) IDK_Rect(Area, 0.15, 0.20, 0.30, 0.5, 0, 0, 1) ;
    
    if (IDK_GetRightMouseButton() && IDK_GetLeftMouseButton()) IDK_Rect(Area, 0.15, 0.20, 0.30, 0.7, 0, 1, 1) ;
    
    static float xpos, ypos = 0 ;
        
    if ( IDK_GetKey(w_key) ) ypos += 0.01 ;
    
    if ( IDK_GetKey(s_key) ) ypos -= 0.01 ;
    
    if ( IDK_GetKey(a_key) ) xpos -= 0.01 ;
    
    if ( IDK_GetKey(d_key) ) xpos += 0.01 ;
    
    IDK_Rect(Area, 0.05, 0.08, xpos, ypos, 0, 1, 0) ;
        
    static float xpos2, ypos2 = 0.5 ;
    
    if ( IDK_GetKey(up_key) ) ypos2 += 0.01 ;
    
    if ( IDK_GetKey(down_key) ) ypos2 -= 0.01 ;
    
    if ( IDK_GetKey(left_key) ) xpos2 -= 0.01 ;
    
    if ( IDK_GetKey(right_key) ) xpos2 += 0.01 ;
    
    IDK_Rect(Area, 0.08, 0.05, xpos2, ypos2, 0, 0, 1) ;
    
    } else {
        
    void pirates_teststuff(IDKDrawArea Area) ;
     
    void GameRun( IDKDrawArea Area ) ;
        
    IDK_DisableRefresh() ;
    
    IDK_SpawnThreads(Area) ;
        
    //pirates_teststuff(Area) ;
    
    GameRun(Area) ;
        
    }
    
    IDK_DisplayFrameRate(Area, 0.25, 0.005, 0.95, 0, 0, 1) ;
}
