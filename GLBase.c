//
//  GLBase.c
//  JHG-GL
//
//  Created by Jacob Gordon on 6/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "GLBase.h"
#include "JHG_filesystem.h"
#include "matrixUtil.h"
#include "modelUtil.h"
#include "sourceUtil.h"
#include "shaders.h"
#include "GLTextures.h"
#include "VAO.h"
#include "JHGPixelslib.h"
#include "codename.h"
#include "PlaceToDrawStuff.h"

static JHGPixels_scene testframe ;

GLenum _surfacePrimType;
GLenum _surfaceElementType;
GLuint _surfaceNumElements;
GLuint _defaultFBOName;
GLuint _surfacePrgName;
GLint _surfaceMvpUniformIdx;
GLint _surfaceProjectionUniformIdx;
GLuint _surfaceTexName;
demoModel* _surfaceModel;

GLuint _surfaceVAOName;

GLuint _viewWidth;
GLuint _viewHeight;

GLuint JHGFramecount = 0 ;

static JHGarraytype arraytype = single_array ;

static JHGRawData PixelFrame = NULL ;

static RKPixels_DrawCallBack DrawCallBackFunc ;

static int RKPixels_x = 0 ;

static int RKPixels_y = 0 ;

void RKPixels_SetDrawCallBack( RKPixels_DrawCallBack callbackfunc ) {
    
    DrawCallBackFunc = callbackfunc ;
}

void RKPixels_DrawPixels( unsigned char* Pixels, int x, int y ) {
    
    RKPixels_x = x ;
    
    RKPixels_y = y ;
    
    PixelFrame = Pixels ;
}

JHGRawData RKPixels_MainDrawFunc( void ) {
    
    DrawCallBackFunc.func( DrawCallBackFunc.arguments ) ;
    
    return PixelFrame ;
    
}

void resize(GLuint width, GLuint height) {
    
    glViewport(0, 0, width, height) ;
	
	_viewWidth = width ;
	_viewHeight = height ;
    
    printf("%d, and %d\n",_viewWidth,_viewHeight) ;

}

void startup(void) {
    
    printf("Jacob Harrison Gordon Proudly Presents --JHG-GL A C-Based Basic OpenGl Base for All Your 3D Needs-- ") ;
    
    printf("%s %s\n",glGetString(GL_RENDERER),glGetString(GL_VERSION)) ;
    
    GLint n, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++) {
        printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
               }
    
    _surfaceModel = mdlLoadQuadModel() ;
    
    // Build Vertex Array Object (VAOs) and VBOs with our model data
    _surfaceVAOName = buildVAO(_surfaceModel) ;
    
    // Cache the number of element and primType to use later in our glDrawElements calls
    _surfaceNumElements = _surfaceModel->numElements ;
    _surfacePrimType    = _surfaceModel->primType ;
    _surfaceElementType = _surfaceModel->elementType ;
    
    //If we're using VBOs we can destroy all this memory since buffers are
    // loaded into GL and we've saved anything else we need 
    mdlDestroyModel(_surfaceModel) ;
    _surfaceModel = NULL ;

    
    demoSource *vtxSource = NULL ;
    demoSource *frgSource = NULL ;

    vtxSource = srcLoadSource(GetFilePath("surface","vsh","surface.vsh")) ;
    
    frgSource = srcLoadSource(GetFilePath("surface","fsh","surface.fsh")) ;
    
    _surfacePrgName = BuildSahder(vtxSource,frgSource) ;
    
    srcDestroySource(vtxSource) ;
    srcDestroySource(frgSource) ;
    
    
   _surfaceMvpUniformIdx = glGetUniformLocation(_surfacePrgName, "modelViewProjectionMatrix") ;
    
    //OpenGL start
    
    // We will always cull back faces for better performance
    glEnable(GL_CULL_FACE) ;
    
    //Gray is cool!
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f) ;
    
}

void drawstuff(void) {
    
    // Set up the modelview and projection matricies
	GLfloat modelView[16];
	GLfloat projection[16];
	GLfloat mvp[16];
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _viewWidth, _viewHeight);
    
    glUseProgram(_surfacePrgName);
    
    mtxLoadOrthographic(projection, -1,1,-1,1,1,1000);
    
    mtxLoadTranslate(modelView, 0, -0, -250);
    
	// Multiply the modelview and projection matrix and set it in the shader
	mtxMultiply(mvp, projection, modelView);
	
	// Set the projection matrix that we calculated above
	// in our vertex shader
	glUniformMatrix4fv(_surfaceMvpUniformIdx, 1, GL_FALSE, mvp) ;
    
    JHGRawData rawdata_2 = RKPixels_MainDrawFunc() ;
    
    // Build a texture object with our image data
    _surfaceTexName = LoadTextureToOpenGL(rawdata_2, RKPixels_x, RKPixels_y, GL_RGB, GL_UNSIGNED_BYTE) ;
    
    //if ( testframe->pixelarray_single == NULL ) free(rawdata) ;
    
    if (GetdoDisplayNeedUpdate()) {
        
        //JHGPixels_Reset_To_Monocolor(testframe, 0) ;
        JHGPixels_FastMonocolorSet( rawdata_2, 0, ( RKPixels_x * (RKPixels_y * 3) ) ) ;
    }

    // Bind the texture we rendered-to above
	glBindTexture(GL_TEXTURE_2D, _surfaceTexName);
    
    glCullFace(GL_BACK);
    
    // Bind our vertex array object
	glBindVertexArray(_surfaceVAOName) ;
    
    glDrawElements(GL_TRIANGLES, _surfaceNumElements, _surfaceElementType, 0) ;
    
    glDeleteTextures(1, &_surfaceTexName) ;
    
}

void cleanup(void) {
    
 JHGPixels_scenefree(testframe) ;
    
 glDeleteTextures(1, &_surfaceTexName) ;
   
 destroyVAO(_surfaceVAOName) ;
    
 mdlDestroyModel(_surfaceModel) ;
    
 EndOfSahders(_surfacePrgName) ;    
    
}
