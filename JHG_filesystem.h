//
//  JHG_filesystem.h
//  JHG-GL
//
//  Created by Jacob Gordon on 6/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef JHG_GL_JHG_filesystem_h
#define JHG_GL_JHG_filesystem_h
#ifdef __APPLE__
const char* get_filePathName_mac(const char* filename,const char* filetype) ;
#define GetFilePath(name,type,filename) get_filePathName_mac(name,type)
#else
#define GetFilePath(name,type,filename) filename
#endif
#endif
