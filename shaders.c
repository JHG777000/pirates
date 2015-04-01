//
//  shaders.c
//  JHG-GL
//
//  Created by Jacob Gordon on 6/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
/*
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2010~2011 Apple Inc. All Rights Reserved.
 
 */

#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "GLBase.h"
#include "sourceUtil.h"

GLuint BuildSahder(demoSource* vertexSource, demoSource* fragmentSource) {
    
    GLuint prgName;
	
	GLint logLength, status;
	
	// String to pass to glShaderSource
	GLchar* sourceString = NULL;  
	
	// Determine if GLSL version 140 is supported by this context.
	//  We'll use this info to generate a GLSL shader source string  
	//  with the proper version preprocessor string prepended
	float  glLanguageVersion;
    
    sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &glLanguageVersion);
    
    // GL_SHADING_LANGUAGE_VERSION returns the version standard version form 
	//  with decimals, but the GLSL version preprocessor directive simply
	//  uses integers (thus 1.10 should 110 and 1.40 should be 140, etc.)
	//  We multiply the floating point number by 100 to get a proper
	//  number for the GLSL preprocessor directive
	GLuint version = 100 * glLanguageVersion;
    
    // Get the size of the version preprocessor string info so we know 
	//  how much memory to allocate for our sourceString
	const GLsizei versionStringSize = sizeof("#version 123\n");
    
    // Create a program object
	prgName = glCreateProgram();
	
	// Indicate the attribute indicies on which vertex arrays will be
	//  set with glVertexAttribPointer
	//  See buildVAO to see where vertex arrays are actually set
	glBindAttribLocation(prgName, POS_ATTRIB_IDX, "inPosition");
    
    glBindAttribLocation(prgName, TEXCOORD_ATTRIB_IDX, "inTexcoord");
    
    //////////////////////////////////////
	// Specify and compile VertexShader //
	//////////////////////////////////////
	
	// Allocate memory for the source string including the version preprocessor information
	sourceString = malloc(vertexSource->byteSize + versionStringSize);
	
	// Prepend our vertex shader source string with the supported GLSL version so
	//  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
	sprintf(sourceString, "#version %d\n%s", version, vertexSource->string);
    
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);	
	glShaderSource(vertexShader, 1, (const GLchar **)&(sourceString), NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	
	if (logLength > 0) 
	{
		GLchar *log = (GLchar*) malloc(logLength);
		glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
		printf("Vtx Shader compile log:%s\n", log);
		free(log);
	}
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		printf("Failed to compile vtx shader:\n%s\n", sourceString);
		return 0;
	}
	
	free(sourceString);
	sourceString = NULL;
	
	// Attach the vertex shader to our program
	glAttachShader(prgName, vertexShader);
	
	
	/////////////////////////////////////////
	// Specify and compile Fragment Shader //
	/////////////////////////////////////////
	
	// Allocate memory for the source string including the version preprocessor	 information
	sourceString = malloc(fragmentSource->byteSize + versionStringSize);
	
	// Prepend our fragment shader source string with the supported GLSL version so
	//  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
	sprintf(sourceString, "#version %d\n%s", version, fragmentSource->string);
	
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);	
	glShaderSource(fragShader, 1, (const GLchar **)&(sourceString), NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) 
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(fragShader, logLength, &logLength, log);
		printf("Frag Shader compile log:\n%s\n", log);
		free(log);
	}
	
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		printf("Failed to compile frag shader:\n%s\n", sourceString);
		return 0;
	}
	
	free(sourceString);
	sourceString = NULL;
	
	// Attach the fragment shader to our program
	glAttachShader(prgName, fragShader);
	
	
	//////////////////////
	// Link the program //
	//////////////////////
	
	glLinkProgram(prgName);
	glGetProgramiv(prgName, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(prgName, logLength, &logLength, log);
		printf("Program link log:\n%s\n", log);
		free(log);
	}
	
	glGetProgramiv(prgName, GL_LINK_STATUS, &status);
	if (status == 0)
	{
		printf("Failed to link program");
		return 0;
	}
	
	glValidateProgram(prgName);
	glGetProgramiv(prgName, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(prgName, logLength, &logLength, log);
		printf("Program validate log:\n%s\n", log);
		free(log);
	}
	
	glGetProgramiv(prgName, GL_VALIDATE_STATUS, &status);
	if (status == 0)
	{
		printf("Failed to validate program");
		return 0;
	}
	
	
	glUseProgram(prgName);
	
	///////////////////////////////////////
	// Setup common program input points //
	///////////////////////////////////////
    
	
	GLint samplerLoc = glGetUniformLocation(prgName, "diffuseTexture");
	
	// Indicate that the diffuse texture will be bound to texture unit 0
	GLint unit = 0;
	glUniform1i(samplerLoc, unit);
    
    return prgName;
    
    
}
void EndOfSahders(GLuint prgName) {
    
    if(0 == prgName)
	{
		return;
	}
	
	GLsizei shaderNum;
	GLsizei shaderCount;
	
	// Get the number of attached shaders
	glGetProgramiv(prgName, GL_ATTACHED_SHADERS, &shaderCount);
	
	GLuint* shaders = (GLuint*)malloc(shaderCount * sizeof(GLuint));
	
	// Get the names of the shaders attached to the program
	glGetAttachedShaders(prgName,
						 shaderCount,
						 &shaderCount,
						 shaders);
	
	// Delete the shaders attached to the program
	for(shaderNum = 0; shaderNum < shaderCount; shaderNum++)
	{
		glDeleteShader(shaders[shaderNum]);
	}
	
	free(shaders);
	
	// Delete the program
	glDeleteProgram(prgName);
	glUseProgram(0);
    
    
}