//
//  VAO.h
//  JHG-GL
//
//  Created by Jacob Gordon on 6/9/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef JHG_GL_VAO_h
#define JHG_GL_VAO_h
#include "GLBase.h"
#include "modelUtil.h"

GLuint buildVAO(demoModel* model);
void destroyVAO(GLuint vaoName);

#endif
