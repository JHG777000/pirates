//
//  JHGPixelslib.c
//  MacGLEssentials
//
//  Created by Jacob Gordon on 6/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JHGPixelslib.h"

static JHGPixels_window current_window = NULL ;

void JHGPixels_createwindow(int res_width, int res_height, void (*GetWindowSize)(int *width, int *height), void (*SetWindowSize) (int width, int height) )
{
    JHGPixels_window new_window ;
    
    if ( current_window == NULL ) {
        
        new_window = (JHGPixels_window) malloc(sizeof(JHGPixels_window_object));
        
        current_window = new_window ;
        
    } else {
        
        free(current_window) ;
        
        new_window = (JHGPixels_window) malloc(sizeof(JHGPixels_window_object));
        
        current_window = new_window ;
        
    }
    
    if (current_window != NULL ) {
     
        current_window->res_width = res_width ;
        
        current_window->res_height = res_height ;
        
        current_window->GetWindowSize = GetWindowSize ;
        
        current_window->SetWindowSize = SetWindowSize ;
    }
    
}  

void JHGPixels_SetMainResolution(int width, int height) {
    
    if ( current_window != NULL ) {
        
        current_window->res_width = width ;
        
        current_window->res_height = height ;
    }
}

void JHGPixels_GetMainResolution(int *width, int *height) {
    
    if ( current_window != NULL ) {
        
      *width = current_window->res_width ;
        
      *height = current_window->res_height ;
    }
}

void JHGPixels_SetWindowSize(int width, int height) {
    
    if ( current_window != NULL ) {
        
        current_window->SetWindowSize(width, height) ;
    }
}

void JHGPixels_GetWindowSize(int *width, int *height) {
    
    if ( current_window != NULL ) {
        
        current_window->GetWindowSize(width, height) ;
    }
}

void JHGPixels_destroywindow(void) {
    
  free(current_window) ;
    
  current_window = NULL ;
    
}

void JHGPixels_FastMonocolorSet( JHGsubpixel pixelarray[], const JHGsubpixel color, const int size ) {
    
    int rk = 0 ;
    
    while ( rk < size ) {
        
        pixelarray[rk] = color ;
        
        rk++ ;
    }
}

void JHGPixels_Reset_To_Monocolor( JHGPixels_scene scene, JHGsubpixel color ) {
    
    int i = 0 ;
    
    int j = 0 ;
    
    if ( scene->pixelarray_single == NULL ) {
        
        while (i < scene->x) {
            j = 0 ;
            while (j < scene->y) {
                
                scene->pixelarray_double[i][j].r = color ;
                
                scene->pixelarray_double[i][j].g = color ;
                
                scene->pixelarray_double[i][j].b = color ;
                
                j++ ;
            }
            
            i++ ;
        }
        
    } else if ( scene->pixelarray_double == NULL  ) {
        
        JHGPixels_FastMonocolorSet( scene->pixelarray_single, color, ( scene->x * (scene->y * 3) ) ) ;
  }
}



void JHGPixels_Reset( JHGPixels_scene scene, JHGsubpixel red, JHGsubpixel blue, JHGsubpixel green ) {
    
    int i = 0 ;
    
    int j = 0 ;
    
    int rk = 0 ;

    if ( scene->pixelarray_single == NULL ) {
        
        while (i < scene->x) {
            j = 0 ;
            while (j < scene->y) {
                
                scene->pixelarray_double[i][j].r = red ;
                
                scene->pixelarray_double[i][j].g = green ;
                
                scene->pixelarray_double[i][j].b = blue ;
                
                j++ ;
            }
            
            i++ ;
        }
        
    } else if ( scene->pixelarray_double == NULL  ) {
        
        while ( i < (scene->x ) ) {
            j = 0 ;
            while ( j < (scene->y ) ) {
                
                scene->pixelarray_single[rk] = red ;
                scene->pixelarray_single[rk + 1] = green ;
                scene->pixelarray_single[rk + 2] = blue ;
                
                j++;
                rk += 3 ;
            }
            i++ ;
        }
    }
    
}

void JHGPixels_init( JHGPixels_scene scene )  {
    
    int i = 0 ;
    
    int j = 0 ;
    
    int rk = 0 ;
    
    if ( scene->pixelarray_single == NULL ) {
    
    while (i < scene->x) {
        j = 0 ;
        while (j < scene->y) {
            
            scene->pixelarray_double[i][j] = scene->background ;
            
            j++ ;
        }
        
        i++ ;
    }
        
    } else if ( scene->pixelarray_double == NULL  ) {
        
        while ( i < (scene->x ) ) {
            j = 0 ;
            while ( j < (scene->y ) ) {
                
                scene->pixelarray_single[rk] = scene->background.r ;
                scene->pixelarray_single[rk + 1] = scene->background.g ;
                scene->pixelarray_single[rk + 2] = scene->background.b ;
                
                j++;
                rk += 3 ;
            }
            i++ ;
        }
    }
}

JHGPixels_scene JHGPixels_newscene( int x, int y, JHGPixelcolor_Object background, JHGarraytype arraytype ) {
    
    int i = 0 ;
    
    JHGPixels_scene newscene ;
    
    newscene = (JHGPixels_scene) malloc(sizeof(JHGPixels_scene_object));
    
    if ( newscene == NULL ) return NULL ;
    
    newscene->x = x ;
    
    newscene->y = y ;
    
    newscene->background = background ;
    
    if ( arraytype == double_array ) {
        
    newscene->pixelarray_single = NULL ;
    
    newscene->pixelarray_double = ( JHGPixelcolor_Object  ** ) malloc( sizeof( JHGPixelcolor_Object* ) * x ) ;
    
    if ( newscene->pixelarray_double == NULL ) return NULL ;
    
    while ( i < x ) {
        
       newscene->pixelarray_double[i] = ( JHGPixelcolor_Object  * ) malloc( sizeof( JHGPixelcolor_Object ) * y ) ;
        
        if ( newscene->pixelarray_double[i] == NULL ) return NULL ;
        
        i++ ;
        
    }
        
    } else if ( arraytype == single_array ) {
        
        newscene->pixelarray_double = NULL ;
        
        newscene->pixelarray_single = (JHGRawData) malloc(newscene->x * (newscene->y * 3) ) ;
        
        if ( newscene->pixelarray_single == NULL ) return NULL ;
        
    }
    
    JHGPixels_init(newscene) ;
    
    return newscene ;
    
}

void JHGPixels_SetBackGroundColor(JHGPixels_scene scene, JHGsubpixel red, JHGsubpixel blue, JHGsubpixel green) {
    
    scene->background.r = red ;
    
    scene->background.b = blue ;
    
    scene->background.g = green ;
}

void JHGPixels_SetPixel( JHGPixels_scene scene, int x, int y, JHGsubpixel red, JHGsubpixel blue, JHGsubpixel green ) {
    
    int rk = ( x * 3 ) + ( y * scene->y * 3 ) ;
    
    scene->pixelarray_single[rk] = red ;
    
    scene->pixelarray_single[rk + 1] = green ;
    
    scene->pixelarray_single[rk + 2] = blue ;
    
}

void JHGPixels_scenefree( JHGPixels_scene scene )  {
    
    int i = 0 ;
    
    if ( scene->pixelarray_single == NULL ) {
    
    while (i < scene->x) {
        
            free(scene->pixelarray_double[i]) ;
            
        i++ ;
    }

    free(scene->pixelarray_double) ;
    
    } else if ( scene->pixelarray_double == NULL ) {
        
        free(scene->pixelarray_single) ;
    }
    
    free(scene) ;
}

JHGRawData JHG_DrawPixels(JHGPixels_scene pixelframe, int *x, int *y) {
    
    *x = pixelframe->x ;
    
    *y = pixelframe->y ;
    
    if ( pixelframe->pixelarray_single == NULL ) {
    
        JHGRawData rawdata = (JHGRawData) malloc( pixelframe->x * (pixelframe->y * 3) );
        
        if ( rawdata == NULL ) {
            
            free(rawdata) ;
            
            return NULL ;
        }
        
        int i = 0 ;
        int j = 0 ;
        int rk = 0 ;
        
        while ( i < ( pixelframe->x ) ) {
            j = 0 ;
            while ( j < ( pixelframe->y ) ) {
                
                rawdata[rk] = pixelframe->pixelarray_double[j][i].r ;
                rawdata[rk + 1] = pixelframe->pixelarray_double[j][i].g ;
                rawdata[rk + 2] = pixelframe->pixelarray_double[j][i].b ;
                
                j++ ;
                rk += 3 ;
            }
            i++ ;
        }
        
        return rawdata ;
     
    } else if ( pixelframe->pixelarray_double == NULL ) {
        
        return pixelframe->pixelarray_single ;
    }
    
    return NULL ;
}