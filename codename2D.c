//
//  codename2D.c
//  pirates_stage1
//
//  Created by Jacob Gordon on 9/20/13.
//  Copyright (c) 2013 Jacob Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "RKMem.h"
#include "RKMath.h"
#include "GLBase.h"
#include "RKWindow.h"
#include "JHGPixelslib.h"
#include "codename.h"
#include "codename2D.h"

static int codename2D_RKPixelsDrawCallbackFunc( void* MyDrawFuntionObj ) {
    
    codename2D_drawfunc drawcallback = (codename2D_drawfunc) MyDrawFuntionObj ;
    
    MyDrawFuntionType MyDrawFuntion = (MyDrawFuntionType) drawcallback->drawfunc ;
    
    codename2D_Canvus MainCanvus = (codename2D_Canvus) drawcallback->maincanvus ;
    
    int retval = MyDrawFuntion( MainCanvus ) ;
    
    if ( retval ) return retval ;
    
    return retval ;
    
}

int codename2D_CreateDrawArea( MyDrawFuntionType MyDrawFuntion, int reswidth, int resheight, int winwidth, int winheight ) {
    
    int retval = 0 ;
    
    codename2D_Canvus maincanvus = codename2D_NewCanvusFromScratch(reswidth, resheight) ;
    
    retval = codename2D_SysDraw(maincanvus, MyDrawFuntion, winwidth, winheight) ;
    
    return retval ;
}

int codename2D_SysDraw( codename2D_Canvus Canvus, MyDrawFuntionType MyDrawFuntion, int winwidth, int winheight ) {
    
    int retval = 0 ;
    
    RKPixels_DrawCallBack callbackfunc ;
    
    codename2D_drawfunc drawfunc = RKMem_CArray(1, codename2D_drawfunc_object) ;
    
    drawfunc->drawfunc = MyDrawFuntion ;
    
    drawfunc->maincanvus = Canvus ;
    
    callbackfunc.func = codename2D_RKPixelsDrawCallbackFunc ;
    
    callbackfunc.arguments = (void*) drawfunc ;
    
    RKPixels_SetDrawCallBack( callbackfunc ) ;
    
    RKPixels_DrawPixels(Canvus->scene->pixelscene->pixelarray_single, Canvus->scene->pixelscene->x, Canvus->scene->pixelscene->y) ;
    
    RKWindow_CreateWindow( winwidth, winheight ) ;
    
    return retval ;
}

codename2D_TileArray codename2D_NewTileArray(int grid_x, int grid_y, int res_x, int res_y) {
   
     codename2D_TileArray TileArray ;
    
     codename2D_BoundingBox** tile_array = NULL ;
    
     int GridSizeX = 0 ;
    
     int GridSizeY = 0 ;
    
     int GridX = 0 ;
    
     int GridY = 0 ;
    
     int ResX = 0 ;
    
     int ResY = 0 ;
    
    if ((grid_x != GridX) || (grid_y != GridY) || (res_x != ResX) || (res_y != ResY)) {
        
        GridX = grid_x ;
        
        GridY = grid_y ;
        
        ResX = res_x ;
        
        ResY = res_y ;
        
        if ( GridX > ResX ) GridX = ResX ;
        
        GridSizeX = ResX / GridX ;
        
        if ( GridSizeY < 1 ) GridSizeY = 3 ;
        
        if ( GridY > ResY ) GridY = ResY ;
        
        GridSizeY = ResY / GridY ;
        
        if ( GridSizeY < 1 ) GridSizeY = 3 ;
        
        TileArray.GridX = GridX ;
        
        TileArray.GridY = GridY ;
        
        TileArray.GridSizeX = GridSizeX ;
        
        TileArray.GridSizeY = GridSizeY ;
    }
    
    tile_array = RKMem_CArray(GridX, codename2D_BoundingBox*) ;
    
    int x = 0 ;
    
    int z = 0 ;
    
    while ( x < GridX ) {
        
        tile_array[x] = RKMem_CArray(GridY, codename2D_BoundingBox) ;
        
        x++ ;
    }
    
    x = 0 ;
    
    z = 0 ;
    
    while ( x < GridX ) {
        
        z = 0 ;
        
        while ( z < GridY ) {
            
            tile_array[x][z].X = (x + 1) * GridSizeX ;
            
            tile_array[x][z].x = x * GridSizeX ;
            
            tile_array[x][z].Y = (z + 1) * GridSizeY ;
            
            tile_array[x][z].y = z * GridSizeY ;
            
            z++ ;
        }
        
        x++ ;
    }
    
    TileArray.tile_array = tile_array ;
    
    return TileArray ;
}

codename2D_Canvus codename2D_NewCanvus( codename_scene scene, int res_x, int res_y ) {
    
    codename2D_Canvus newcanvus = RKMem_CArray(1, codename2D_Canvus_object) ;
    
    newcanvus->scene = scene ;
    
    newcanvus->res_x = res_x ;
    
    newcanvus->res_y = res_y ;
    
    newcanvus->TileArray = codename2D_NewTileArray(10, 10, newcanvus->res_x, newcanvus->res_y) ;
    
    return newcanvus ;
}

codename2D_Canvus codename2D_NewCanvusFromScratch( int width, int height ) {
    
    JHGPixels_scene pixelscene = JHGPixels_newscene(width, height, *(codename_NewColorObject(0,0,0)), single_array) ;
    
    codename_scene codenamescene = codename_NewSceneObject(pixelscene, 1, 0, 0) ;
    
    codename2D_Canvus newcanvus = codename2D_NewCanvus(codenamescene,width,height) ;
    
    return newcanvus ;
}

void codename2D_FreeCanvus( codename2D_Canvus Canvus ) {
    
    int i = 0 ;
    
    
    
    free( Canvus ) ;

}

void codename2D_DestroyCanvus( codename2D_Canvus Canvus ) {
    
    JHGPixels_scenefree( Canvus->scene->pixelscene ) ;
    
    free( Canvus->scene ) ;
    
    codename2D_FreeCanvus( Canvus ) ;

}

void codename2D_ConvertToTileRes(codename2D_Canvus MainCanvus, int* x_inout, int* y_inout ) {
    
    int x = *x_inout ;
    
    int y = *y_inout ;
    
    if ( x < MainCanvus->TileArray.GridSizeX ) x = 0 ;
    
    if ( x == MainCanvus->TileArray.GridSizeX ) x = 1 ;
    
    if ( x > MainCanvus->TileArray.GridSizeX ) x = x/MainCanvus->TileArray.GridSizeX ;
    
    if ( y < MainCanvus->TileArray.GridSizeY ) y = 0 ;
    
    if ( y == MainCanvus->TileArray.GridSizeY ) y = 1 ;
    
    if ( y > MainCanvus->TileArray.GridSizeY ) y = y/MainCanvus->TileArray.GridSizeY ;
    
    *x_inout = x ;
    
    *y_inout = y ;
}