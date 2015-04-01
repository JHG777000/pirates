//
//  GLBase.c
//  JHG-GL
//
//  Created by Jacob Gordon on 6/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "GLBase.h"
#include "matrixUtil.h"
#include "modelUtil.h"
#include "sourceUtil.h"
#include "shaders.h"
#include "GLTextures.h"
#include "VAO.h"
#include "JHGPixelslib.h"
#include "codename.h"
#include "IDK.h"
#include "PlaceToDrawStuff.h"

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

static IDKDrawArea drawarea = NULL ;

void resize(GLuint width, GLuint height) {
    
    glViewport(0, 0, width, height);
	
	_viewWidth = width;
	_viewHeight = height;
    
    printf("%d, and %d\n",_viewWidth,_viewHeight);

}

void startup(void) {
    
    int ResWidth = 0 ;
    int ResHeight = 0 ;
    
    printf("Jacob Harrison Gordon Proudly Presents --JHG-GL A C-Based Basic OpenGl Base for All Your 3D Needs-- ");
    
    printf("%s %s\n",glGetString(GL_RENDERER),glGetString(GL_VERSION));
    
    ///APPLE SAMPLE CODE///
    
    _surfaceModel = mdlLoadQuadModel();
    
    // Build Vertex Array Object (VAOs) and VBOs with our model data
    _surfaceVAOName = buildVAO(_surfaceModel);
    
    // Cache the number of element and primType to use later in our glDrawElements calls
    _surfaceNumElements = _surfaceModel->numElements;
    _surfacePrimType    = _surfaceModel->primType;
    _surfaceElementType = _surfaceModel->elementType;
    
    //If we're using VBOs we can destroy all this memory since buffers are
    // loaded into GL and we've saved anything else we need 
    mdlDestroyModel(_surfaceModel);
    _surfaceModel = NULL;

    
    demoSource *vtxSource = NULL;
    demoSource *frgSource = NULL;

    vtxSource = srcLoadSource("surface.vsh") ;
    
    frgSource = srcLoadSource("surface.fsh") ;
    
    _surfacePrgName = BuildSahder(vtxSource,frgSource) ;
    
    srcDestroySource(vtxSource);
    srcDestroySource(frgSource);
    
    
   _surfaceMvpUniformIdx = glGetUniformLocation(_surfacePrgName, "modelViewProjectionMatrix");
    
    // JHGPixels/IDK
    JHGPixels_GetMainResolution(&ResWidth, &ResHeight) ;
    
    drawarea = IDK_NewDrawArea(MyDrawArea, ResWidth, ResHeight) ;
    
    //OpenGL start
    
    // We will always cull back faces for better performance
    glEnable(GL_CULL_FACE);
    
    //Gray is cool!
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void drawstuff(void) {
    
    int x = 0 ;
    int y = 0 ;
    
    ///APPLE SAMPLE CODE///
    
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
	glUniformMatrix4fv(_surfaceMvpUniformIdx, 1, GL_FALSE, mvp);
    
    // JHGPixels/IDK
    
    if (IDK_GetdoDisplayNeedUpdate())JHGPixels_ResetBackGround(drawarea->r_scene->pixelscene) ;
    
    if ( drawarea->r_scene->pixelscene == NULL ) printf("JHGPixels_scene is NULL!!!!") ;
    
    IDKRawData rawdata = IDK_Draw(drawarea, &x, &y) ;
    
    // Build a texture object with our image data
     _surfaceTexName = LoadTextureToOpenGL(rawdata, x, y, GL_RGB, GL_UNSIGNED_BYTE) ;

    ///APPLE SAMPLE CODE///
    
    // Bind the texture we rendered-to above
	glBindTexture(GL_TEXTURE_2D, _surfaceTexName);
    
    glCullFace(GL_BACK);
    
    // Bind our vertex array object
	glBindVertexArray(_surfaceVAOName);
    
    glDrawElements(GL_TRIANGLES, _surfaceNumElements, _surfaceElementType, 0);
    
    glDeleteTextures(1,&_surfaceTexName) ;
    
}

void cleanup(void) {
    
 glDeleteTextures(1,&_surfaceTexName) ;

 IDK_DestroyDrawArea(drawarea) ;
    
 destroyVAO(_surfaceVAOName);
    
 mdlDestroyModel(_surfaceModel);
    
 EndOfSahders(_surfacePrgName);    
    
}
