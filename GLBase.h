//
//  GLBase.h
//  JHG-GL
//
//  Created by Jacob Gordon on 6/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "glUtil.h"


#ifndef JHG_GL_GLBase_h
#define JHG_GL_GLBase_h
#include "sourceUtil.h"
void startup(void);
void resize(GLuint width, GLuint height);
void drawstuff(void);
void cleanup(void);

enum {
	POS_ATTRIB_IDX,
	NORMAL_ATTRIB_IDX,
	TEXCOORD_ATTRIB_IDX
};

#endif
