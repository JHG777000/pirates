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

typedef enum { Triangles } pirates3d_primitive_type ;

typedef enum { Geometry, Material, Transform, Camera } pirates3d_entity_type ;

typedef struct pirates3d_scene_s* pirates3d_scene ;

typedef RKList_node pirates3d_node ;

typedef RKList pirates3d_graph ;

typedef struct pirates3d_entity_s* pirates3d_entity ;

#endif /* defined(__IDKApp__pirates3d__) */
