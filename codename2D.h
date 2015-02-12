//
//  codename2D.h
//  pirates_stage1
//
//  Created by Jacob Gordon on 9/20/13.
//  Copyright (c) 2013 Jacob Gordon. All rights reserved.
//

#ifndef pirates_stage1_codename2D_h
#define pirates_stage1_codename2D_h
#include "codename.h"
#include "RKMem.h"
#include "RKMath.h"

typedef codename_pixelscene codename2D_Bitmap ;

typedef JHGRawData codename2D_RawPixels ;

typedef unsigned long int codename2D_Type64 ;

typedef const char* codename2D_String ;

typedef int codename2D_Texture ;

typedef struct { float x ; float y ; } cn2D_Cord ;

typedef struct { float X ; float x ; float Y ; float y ; } codename2D_BoundingBox ;

typedef struct { int GridX ; int GridY ; int GridSizeX ; int GridSizeY ; codename2D_BoundingBox** tile_array ; } codename2D_TileArray ;

typedef struct { codename_scene scene ; int res_x ; int res_y ; codename2D_TileArray TileArray ; } codename2D_Canvus_object ;

typedef codename2D_Canvus_object* codename2D_Canvus ;

typedef int (*MyDrawFuntionType)(codename2D_Canvus MainCanvus) ;

typedef struct { MyDrawFuntionType drawfunc ; codename2D_Canvus maincanvus ; } codename2D_drawfunc_object ;

typedef codename2D_drawfunc_object* codename2D_drawfunc ;

int codename2D_CreateDrawArea( MyDrawFuntionType MyDrawFuntion, int reswidth, int resheight, int winwidth, int winheight ) ;

codename2D_Canvus codename2D_NewCanvus( codename_scene scene, int res_x, int res_y ) ;

codename2D_Canvus codename2D_NewCanvusFromScratch( int width, int height ) ;

int codename2D_SysDraw( codename2D_Canvus Canvus, MyDrawFuntionType MyDrawFuntion, int winwidth, int winheight ) ;

void codename2D_FreeCanvus( codename2D_Canvus Canvus ) ;

void codename2D_DestroyCanvus( codename2D_Canvus Canvus ) ;

codename2D_Bitmap codename2D_GetBitmapFromCanvus( codename2D_Canvus Canvus ) ;

codename2D_RawPixels codename2D_GetRawPixelsFromCanvus( codename2D_Canvus Canvus ) ;

codename2D_RawPixels codename2D_GetRawPixelsFromBitmap( codename2D_Bitmap Bitmap ) ;

codename2D_Texture codename2D_GetTextureFromCanvus( codename2D_Canvus Canvus ) ;

codename2D_Texture codename2D_GetTextureFromBitmap( codename2D_Bitmap Bitmap ) ;

codename2D_Texture codename2D_GetTextureFromRawPixels( codename2D_RawPixels RawPixels, int x, int y ) ;

#endif
