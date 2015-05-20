//
//  IDK.c
//  IDKApp
//
//  Created by Jacob Gordon on 3/22/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IDK.h"

static int num_of_keys = 16 ;

static int* key_array = NULL ;

int left = 0 ;

int middle = 0 ;

int right = 0 ;

static int forceupdate = 0 ;

static int idk_refresh = 1 ;

static int active = 0 ;

static float MouseX = 0 ;

static float MouseY = 0 ;

static float oldX = 0 ;

static float oldY = 0 ;

int IDK_Timer( long* time_count, long seconds ) {
    
    long _time_ = ((unsigned)time(NULL) - (*time_count)) ;
    
    (*time_count) = (unsigned)time(NULL) ;
    
    return ( _time_ >= seconds ) ;
}

int IDK_Flip( int* flip ) {
    
    *flip = !(*flip) ;
    
    return *flip ;
}

void IDK_AllocKeyboard( void ) {
    
    int i = 0 ;
    
    key_array = RKMem_CArray(num_of_keys, int) ;
    
    while ( i < num_of_keys ) {
        
        key_array[i] = 0 ;
        
        i++ ;
    }
}

void IDK_DeallocKeyboard( void ) {
    
    free(key_array) ;
}

void IDK_SetKey( int key ) {
    
    key_array[key] = 1 ;
}

int IDK_GetKey( int key ) {
    
    return key_array[key] ;
}

void IDK_ResetKey( int key ) {

     key_array[key] = 0 ;
}

void IDK_SetLeftMouseButton(void) {
    
    left = 1 ;
}

void IDK_ResetLeftMouseButton(void) {
    
    left = 0 ;
}

int IDK_GetLeftMouseButton(void) {
    
    return left ;
}

void IDK_SetRightMouseButton(void) {
    
    right = 1 ;
}

void IDK_ResetRightMouseButton(void) {
    
    right = 0 ;
}

int IDK_GetRightMouseButton(void) {
    
    return right ;
}

void IDK_ResetMiddleMouseButton(void) {
    
    middle = 0 ;
}

void IDK_SetMiddleMouseButton(void) {
    
    middle = 1 ;
}

int IDK_GetMiddleMouseButton(void) {
    
    return middle ;
}

float IDK_GetMouseX( void ) {
    
    return MouseX ;
}

float IDK_GetMouseY( void ) {
    
    return MouseY ;
}

void IDK_theMouse( float x, float y ) {
    
    if (active) {
        
        MouseX = x ;
        
        MouseY = y ;
        
    }
}

void IDK_setMouseActive( void ) {
    
    active = 1 ;
}

void IDK_setMouseInactive( void ) {
    
    active = 0 ;
}

void IDK_SetDisplayNeedsToUpdate( void ) {
    
    forceupdate++ ;
}

int IDK_GetdoDisplayNeedUpdate(void) {
    
    if ( idk_refresh ) {
    
    if ( forceupdate ) {
        
        forceupdate = 0 ;
        
        return 1 ;
    }
    
    if ((MouseX != oldX) || (MouseY != oldY)) {
        
        oldX = MouseX ;
        
        oldY = MouseY ;
        
        return 1 ;
        
    }
    
    }
    
    return 0 ;
}

void IDK_EnableRefresh(void) {
    
    idk_refresh = 1 ;
}

void IDK_DisableRefresh(void) {
    
    idk_refresh = 0 ;
}

static float GetFPSFunc( void ) {
    
    static int init = 0 ;
    
    static float numofframes = 0 ;
    
    static long time_count = 0 ;
    
    long seconds = 0 ;
    
    static float fps = 0 ;
    
    if (!init) {
        
        time_count = (unsigned)time(NULL) ;
        
        init++ ;
    }
    
    numofframes++ ;
    
    seconds = ((unsigned)time(NULL) - time_count) ;
    
    if ( seconds >= 1 ) {
        
        fps = (numofframes / seconds) ;
        
        seconds = 0 ;
        
        numofframes = 0 ;
        
        time_count = (unsigned)time(NULL) ;
        
    }
    
    return fps ;
}

static const char* myitoa( int val, char* string ) {
    
    snprintf(string, sizeof(string), "%d", val) ;
    
    return string ;
    
}

static const char* myitoa_for_floats( float val, char* string ) {
    
    snprintf(string, sizeof(string), "%f", val) ;
    
    return string ;
    
}

static int from_point_to_pixel( float f, int max ) {
    
    return (int)(f * max) ;
}

static raster_scene new_r_scene( int width, int height ) {
    
    int raster_size = MAX_JHG(width, height) ;
    
    JHGPixels_scene pixelscene = JHGPixels_newscene(raster_size, raster_size, *(codename_NewColorObject(0,0,0)), single_array,JHGINT8888REVBGRA) ;
    
    codename_scene codenamescene = codename_NewSceneObject(pixelscene, 0, 0) ;
    
    return (raster_scene)codenamescene ;
    
}

IDKDrawArea IDK_NewDrawArea( IDKDrawFunc drawfunc, int width, int height ) {
    
    IDKDrawArea NewDrawArea = RKMem_NewMemOfType(IDKDrawArea_object) ;
    
    NewDrawArea->r_scene = new_r_scene(width,height) ;
    
    NewDrawArea->raster_size = MAX_JHG(width, height) ;
    
    NewDrawArea->drawfunc = drawfunc ;
    
    NewDrawArea->Threads = NULL ;
    
    NewDrawArea->rect_x = width ;
    
    NewDrawArea->rect_y = height ;
    
    IDK_AllocKeyboard() ;
    
    return NewDrawArea ;
}

void IDK_DestroyDrawArea( IDKDrawArea area ) {
    
    JHGPixels_scenefree(area->r_scene->pixelscene) ;
    
    IDK_DeallocKeyboard() ;
    
    free(area->r_scene) ;
    
    free(area) ;
}

IDKRawData IDK_Draw( IDKDrawArea area, int *x, int * y ) {
    
    area->drawfunc(area) ;
    
    return (IDKRawData)JHG_DrawPixels(area->r_scene->pixelscene, x, y) ;
}

void IDK_SpawnThreads( IDKDrawArea area ) {
    
    if ( area->Threads == NULL ) area->Threads = RKTasks_NewThreadGroup(1, 8, 4, 1, 6) ;
}

void IDK_KillThreads( IDKDrawArea area ) {
    
    if ( area->Threads != NULL ) {
        
        RKTasks_KillThreadGroup(area->Threads)  ;
        
        area->Threads = NULL ;
    }
}

RKTasks_ThreadGroup IDK_GetThreads( IDKDrawArea area ) {
    
    return area->Threads ;
}

void IDK_Progressbar( IDKDrawArea drawarea, float percent, float size_x, float size_y, float x, float y, float red, float blue, float green ) {
    
    IDK_Rect(drawarea,size_x*percent,size_y,x,y,red,blue,green) ;
}

void IDK_Rect( IDKDrawArea drawarea, float size_x, float size_y, float x, float y, float red, float blue, float green ) {
    
    cnpoint_Rect(drawarea->r_scene, from_point_to_pixel(size_x,drawarea->rect_x), from_point_to_pixel(size_y,drawarea->rect_y), from_point_to_pixel(x,drawarea->raster_size), from_point_to_pixel(y,drawarea->raster_size), from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_SetPoint( IDKDrawArea drawarea, int x, int y, float red, float blue, float green ) {
    
    cn_SetPoint(drawarea->r_scene, x, y, from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_SetDot( IDKDrawArea drawarea, float x, float y, float red, float blue, float green ) {
    
    cn_SetPoint(drawarea->r_scene, from_point_to_pixel(x,drawarea->raster_size), from_point_to_pixel(y,drawarea->raster_size), from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_SetColor( IDKDrawArea drawarea, int x, int y, IDKColor color ) {
    
    IDK_SetPoint(drawarea, x, y, color.red, color.blue, color.green) ;
}

void IDK_SetDotColor( IDKDrawArea drawarea, float x, float y, IDKColor color ) {
    
    IDK_SetDot(drawarea, x, y, color.red, color.blue, color.green) ;
}

void IDK_DisplayFrameRate( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) {
    
    char framestring[100] ;
    
    float fps = GetFPSFunc() ;
    
    myitoa_for_floats(fps,framestring) ;
    
    IDK_String(drawarea, framestring, size, x, y, red, blue, green) ;
    
    IDK_SetDisplayNeedsToUpdate() ;
}

float IDK_GetFPS( void ) {
    
    return GetFPSFunc() ;
}

void IDK_DisplayInt( IDKDrawArea drawarea, int val, float size, float x, float y, float red, float blue, float green ) {
    
    char string[100] ;
    
    myitoa(val, string) ;
    
    IDK_String(drawarea, string, size, x, y, red, blue, green) ;
}

void IDK_String( IDKDrawArea drawarea, const char* string, float size, float x, float y, float red, float blue, float green ) {
    
    cnpoint_String(drawarea->r_scene, string, from_point_to_pixel(size*00.1, (drawarea->raster_size*size)), from_point_to_pixel(x,drawarea->raster_size),  from_point_to_pixel(y,drawarea->raster_size),  from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}