//
//  IDK.h
//  IDKApp
//
//  Created by Jacob Gordon on 3/22/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#ifndef __IDKApp__IDK__
#define __IDKApp__IDK__

#include "RKMem.h"
#include "RKTasks.h"
#include "codename.h"

typedef codename_scene raster_scene ;

typedef JHGRawData IDKRawData ;

typedef struct { float red ; float blue ; float green  ; } IDKColor ;

struct IDKDrawArea_s ;

typedef void (*IDKDrawFunc)(struct IDKDrawArea_s* area) ;

typedef struct IDKDrawArea_s { RKTasks_ThreadGroup Threads ; raster_scene r_scene ; IDKDrawFunc drawfunc ; int raster_size ;
    
int rect_x ; int rect_y ; } IDKDrawArea_object ;

typedef IDKDrawArea_object* IDKDrawArea ;

typedef enum { w_key = 0, a_key = 1, s_key = 2, d_key = 3, right_key = 4, left_key = 5, up_key = 6, down_key = 7 } IDK_Key ;

void IDK_theMouse( float x, float y ) ;
void IDK_setMouseActive( void ) ;
void IDK_setMouseInactive( void ) ;
int IDK_GetdoDisplayNeedUpdate(void) ;
void IDK_SetDisplayNeedsToUpdate(void) ;
float IDK_GetMouseX( void ) ;
float IDK_GetMouseY( void ) ;
void IDK_SetLeftMouseButton(void) ;
int IDK_GetLeftMouseButton(void) ;
void IDK_SetRightMouseButton(void) ;
int IDK_GetRightMouseButton(void) ;
void IDK_SetMiddleMouseButton(void) ;
int IDK_GetMiddleMouseButton(void) ;
void IDK_SetKey( int key ) ;
int IDK_GetKey( int key ) ;
void IDK_ResetKey( int key ) ;
void IDK_ResetLeftMouseButton(void) ;
void IDK_ResetRightMouseButton(void) ;
void IDK_ResetMiddleMouseButton(void) ;
void IDK_EnableRefresh(void) ;
void IDK_DisableRefresh(void) ;

#define IDK_KeyboardMacro(action,press,release,key) \
if ( action == press ) { \
IDK_SetKey(key) ; \
} else if ( action == release ) {\
IDK_ResetKey(key) ;\
}

#define IDK_MouseMacro(action,press,release,button) \
if ( action == press ) { \
IDK_Set ## button ## MouseButton() ; \
} else if ( action == release ) {\
IDK_Reset ## button ## MouseButton() ;\
}

IDKDrawArea IDK_NewDrawArea( IDKDrawFunc drawfunc, int width, int height ) ;

void IDK_DestroyDrawArea( IDKDrawArea area ) ;

IDKRawData IDK_Draw( IDKDrawArea area, int *x, int * y ) ;

void IDK_SpawnThreads( IDKDrawArea area ) ;

void IDK_KillThreads( IDKDrawArea area ) ;

RKTasks_ThreadGroup IDK_GetThreads( IDKDrawArea area ) ;

void IDK_Progressbar( IDKDrawArea drawarea, float percent, float size_x, float size_y, float x, float y, float red, float blue, float green ) ;

void IDK_Rect( IDKDrawArea drawarea, float size_x, float size_y, float x, float y, float red, float blue, float green ) ;

void IDK_SetPoint( IDKDrawArea drawarea, int x, int y, float red, float blue, float green ) ;

void IDK_SetDot( IDKDrawArea drawarea, float x, float y, float red, float blue, float green ) ;

void IDK_SetColor( IDKDrawArea drawarea, int x, int y, IDKColor color ) ;

void IDK_SetDotColor( IDKDrawArea drawarea, float x, float y, IDKColor color ) ;

void IDK_DisplayFrameRate( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) ;

float IDK_GetFPS( void ) ;

void IDK_DisplayInt( IDKDrawArea drawarea, int val, float size, float x, float y, float red, float blue, float green ) ;

void IDK_String(  IDKDrawArea drawarea, const char* string, float size, float x, float y, float red, float blue, float green ) ;

#endif /* defined(__IDKApp__IDK__) */
