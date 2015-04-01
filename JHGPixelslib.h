//
//  JHGPixelslib.h
//  MacGLEssentials
//
//  Created by Jacob Gordon on 6/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MacGLEssentials_JHGPixelslib_h
#define MacGLEssentials_JHGPixelslib_h


typedef float JHGfloat ;
typedef unsigned char JHGsubpixel ;
typedef JHGsubpixel* JHGRawData ;
typedef enum { single_array , double_array  } JHGarraytype ;
typedef struct { JHGsubpixel r ; JHGsubpixel g ; JHGsubpixel b ; } JHGPixelcolor_Object ;
typedef struct { JHGPixelcolor_Object background ; JHGPixelcolor_Object** pixelarray_double ;  JHGRawData pixelarray_single ; int x ; int y ; } JHGPixels_scene_object ;
typedef JHGPixels_scene_object* JHGPixels_scene ;

JHGPixels_scene JHGPixels_newscene( int x, int y, JHGPixelcolor_Object background, JHGarraytype arraytype ) ;
void JHGPixels_SetBackGroundColor(JHGPixels_scene scene, JHGsubpixel red, JHGsubpixel blue, JHGsubpixel green) ;
void JHGPixels_SetPixel( JHGPixels_scene scene, int x, int y, JHGsubpixel red, JHGsubpixel blue, JHGsubpixel green ) ;
void JHGPixels_FastMonocolorSet( JHGsubpixel pixelarray[], const JHGsubpixel color, const int size ) ;
void JHGPixels_FastColorSet( JHGsubpixel pixelarray[], const JHGsubpixel red, const JHGsubpixel blue, const JHGsubpixel green, const int size ) ;
void JHGPixels_Reset_To_Monocolor( JHGPixels_scene scene, JHGsubpixel color ) ;
void JHGPixels_Reset( JHGPixels_scene scene, JHGsubpixel red, JHGsubpixel blue, JHGsubpixel green ) ;
void JHGPixels_ResetBackGround( JHGPixels_scene scene ) ;
void JHGPixels_scenefree( JHGPixels_scene scene ) ;

typedef struct 
{ 
    int res_width ; 
    
    int res_height ;
    
    void (*GetWindowSize)(int *width, int *height) ; 
    
    void (*SetWindowSize) (int width, int height) ; 
    
} JHGPixels_window_object ;

typedef JHGPixels_window_object* JHGPixels_window;

void JHGPixels_createwindow(int res_width, int res_height, void (*GetWindowSize)(int *width, int *height), void (*SetWindowSize) (int width, int height) ) ;

void JHGPixels_destroywindow(void) ;
void JHGPixels_SetMainResolution(int width, int height) ;
void JHGPixels_GetMainResolution(int *width, int *height) ;
void JHGPixels_SetWindowSize(int width, int height) ;
void JHGPixels_GetWindowSize(int *width, int *height) ;

JHGRawData JHG_DrawPixels(JHGPixels_scene pixelframe, int *x, int *y) ;

#endif
