//
//  GLBase.h
//  JHG-GL
//
//  Created by Jacob Gordon on 6/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef JHG_GL_GLBase_h
#define JHG_GL_GLBase_h

#include "glUtil.h"

void startup(void) ;
void resize(GLuint width, GLuint height) ;
void drawstuff(void) ;
void cleanup(void) ;

typedef struct { int (*func)(void *) ; void *arguments ; } RKPixels_DrawCallBack ;

void RKPixels_SetDrawCallBack( RKPixels_DrawCallBack callbackfunc ) ;

void RKPixels_DrawPixels( unsigned char* Pixels, int x, int y ) ;

unsigned char* RKPixels_RetPixels( void ) ;

unsigned char* RKPixels_MainDrawFunc( void ) ;

enum {
	POS_ATTRIB_IDX,
	NORMAL_ATTRIB_IDX,
	TEXCOORD_ATTRIB_IDX
};

#endif
