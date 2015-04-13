//
//  pirates3d.h
//  IDKApp
//
//  Created by Jacob Gordon on 4/6/15.
//  Copyright (c) 2015 Jacob Gordon. All rights reserved.
//

#ifndef __IDKApp__pirates3d__
#define __IDKApp__pirates3d__

#include <stdio.h>
#include "pirates.h"

typedef enum { pirates3d_copy_data = 1, pirates3d_dont_copy_data = 0, pirates3d_dont_copy_but_delete_data = 1 } pirates3d_copy_mode ;

typedef enum { Triangles } pirates3d_primitive_type ;

typedef int pirates3d_material ;

typedef RKList pirates3d_transform ;

typedef struct pirates3d_primitive_array_s* pirates3d_primitive_array ;

typedef struct pirates3d_camera_s* pirates3d_camera ;

typedef struct pirates3d_scene_s* pirates3d_scene ;

#endif /* defined(__IDKApp__pirates3d__) */
