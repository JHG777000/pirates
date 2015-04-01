//
//  codename.c -- 2D basic-vector graphics libary including some text support and graphing
//  
//
//  Created by Jacob Gordon on 6/2/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "codename.h"
#include "JHGPixelslib.h"

int codename_createcanvas(codename_pixelscene pixelscene, int (*canvas_funtion)(codename_pixelscene pixelscene)) {
    
    return canvas_funtion(pixelscene) ;
}

int codename_testfunc(codename_pixelscene pixelscene) {
    
    return 1 ;
    
}

codename_scene codename_NewSceneObject(codename_pixelscene pixelscene, int origin_x, int origin_y) {
    
    codename_scene newscene ;
    
    newscene = (codename_scene) malloc(sizeof(codename_scene_object));
    
    if ( newscene == NULL ) return NULL ;
    
    newscene->pixelscene = pixelscene ;
    
    newscene->origin.x = origin_x ;
    
    newscene->origin.y = origin_y ;
    
    return newscene ;

}

void codename_movescene(codename_scene pointscene, cn_point neworigin) {
    
    pointscene->origin = neworigin ;
}

void codename_ChangeBackGroundColor(codename_scene pointscene, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green ) {
        
    JHGPixels_SetBackGroundColor( (JHGPixels_scene) pointscene->pixelscene, (JHGsubpixel) red, (JHGsubpixel) blue, (JHGsubpixel) green ) ;
    
}

void codename_GetMainResolution(int *width, int *height) {
    
    JHGPixels_GetMainResolution(width, height) ;
    
}

void codename_GetWindowSize(int *width, int *height) {
    
    JHGPixels_GetWindowSize(width, height) ;
    
}

void codename_SetPixel(codename_scene pointscene, int x, int y, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
     codename_pixelcolor pixcolor = codename_NewColorObject( red, blue, green ) ;
    
    if ( pointscene->pixelscene->pixelarray_single == NULL ) {
        
        pointscene->pixelscene->pixelarray_double[x][y] = *pixcolor ;
        
    } else if ( pointscene->pixelscene->pixelarray_double == NULL ) {
        
        JHGPixels_SetPixel(pointscene->pixelscene, x, y, (JHGsubpixel)pixcolor->r, (JHGsubpixel)pixcolor->b, (JHGsubpixel)pixcolor->g) ;
    }
    
    free( pixcolor ) ;
}

void cn_SetPixel(codename_scene pointscene, int x, int y, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    if ( pointscene->pixelscene->pixelarray_single == NULL ) {
        
        JHGPixelcolor_Object pixcolor ;
        
        pixcolor.r = red ;
        
        pixcolor.b = blue ;
        
        pixcolor.g = green ;
        
        pointscene->pixelscene->pixelarray_double[x][y] = pixcolor ;
        
    } else if ( pointscene->pixelscene->pixelarray_double == NULL ) {
        
        JHGPixels_SetPixel(pointscene->pixelscene, x, y, (JHGsubpixel)red, (JHGsubpixel)blue, (JHGsubpixel)green) ;
        
    }

    
}


void cnpixel_SetColor(codename_scene pointscene, int x, int y, codename_pixelcolor pixcolor) {
    
    if ( pointscene->pixelscene->pixelarray_single == NULL ) {
        
    pointscene->pixelscene->pixelarray_double[x][y] = *pixcolor ;
        
    } else if ( pointscene->pixelscene->pixelarray_double == NULL ) {
        
    JHGPixels_SetPixel(pointscene->pixelscene, x, y, (JHGsubpixel)pixcolor->r, (JHGsubpixel)pixcolor->b, (JHGsubpixel)pixcolor->g) ;
    
    }
}


void cn_SetPoint(codename_scene pointscene, int x, int y, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    JHGPixelcolor_Object pixcolor ;
    
    int fin_x ;
    
    int fin_y ;
    
    fin_x = x + pointscene->origin.x ;
    
    fin_y = y + pointscene->origin.y ;
    
    if ( pointscene->pixelscene->pixelarray_single == NULL ) {
        
    pixcolor.r = red ;
        
    pixcolor.b = blue ;
        
    pixcolor.g = green ;
    
    if ( ((fin_x >= 0) && (fin_y >= 0)) && ((fin_x < pointscene->pixelscene->x) && (fin_y < pointscene->pixelscene->y)))
    pointscene->pixelscene->pixelarray_double[fin_x][fin_y] = pixcolor ;
        
    } else if ( pointscene->pixelscene->pixelarray_double == NULL ) {
        
    if ( ((fin_x >= 0) && (fin_y >= 0)) && ((fin_x < pointscene->pixelscene->x) && (fin_y < pointscene->pixelscene->y)))
    JHGPixels_SetPixel(pointscene->pixelscene, fin_x, fin_y, (JHGsubpixel)red, (JHGsubpixel)blue, (JHGsubpixel)green) ;
        
    }
    
}

void cnpoint_SetColor_and_free(codename_scene pointscene, int x, int y, codename_pixelcolor pixcolor) {
    
    int fin_x ;
    
    int fin_y ;

    fin_x = x + pointscene->origin.x ;
    
    fin_y = y + pointscene->origin.y ;
    
    if ( pointscene->pixelscene->pixelarray_single == NULL ) {
        
        if ( ((fin_x >= 0) && (fin_y >= 0)) && ((fin_x < pointscene->pixelscene->x) && (fin_y < pointscene->pixelscene->y)))
            pointscene->pixelscene->pixelarray_double[fin_x][fin_y] = *pixcolor ;
        
    } else if ( pointscene->pixelscene->pixelarray_double == NULL ) {
        
        if ( ((fin_x >= 0) && (fin_y >= 0)) && ((fin_x < pointscene->pixelscene->x) && (fin_y < pointscene->pixelscene->y)))
            JHGPixels_SetPixel(pointscene->pixelscene, fin_x, fin_y, (JHGsubpixel)(pixcolor->r), (JHGsubpixel)(pixcolor->b), (JHGsubpixel)(pixcolor->g)) ;
    }

    
    free(pixcolor) ;
    
}

codename_pixelcolor cn_GetPoint(codename_scene pointscene, int x, int y) {
  
    JHGPixelcolor_Object pixcolor ;
    
    int fin_x = x ; //Get points from screen cords, not real cords
    
    int fin_y = y ;
    
    if ( ((fin_x >= 0) && (fin_y >= 0)) && ((fin_x < pointscene->pixelscene->x) && (fin_y < pointscene->pixelscene->y))) {
    pixcolor = pointscene->pixelscene->pixelarray_double[fin_x][fin_y];    
    return codename_NewColorObject(pixcolor.r, pixcolor.b, pixcolor.g) ;
    }
    
    return codename_NewColorObject(0, 0, 0) ;
}

void cn_SetArcPoint(codename_scene pointscene, int x, int y, int a, int b, int r, double alpha, double beta, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    JHGPixelcolor_Object pixcolor ;
    
    int fin_x ;
    
    int fin_y ;
    
    double angle ;
    
    pixcolor.r = red ;
    
    pixcolor.b = blue ;
    
    pixcolor.g = green ;
    
    fin_x = x + pointscene->origin.x ;
    
    fin_y = y + pointscene->origin.y ;
    
    angle = ((fin_y-b) >= 0) ? acos(MIN_JHG(1.0,MAX_JHG(-1.0,((fin_x-a)/r)))) : (2 * M_PI) - acos(MIN_JHG(1.0,MAX_JHG(-1.0,((fin_x-a)/r)))) ;
    
    if ( (angle > (alpha)) && (angle < (beta)) ) {
    
        if ( ((fin_x >= 0) && (fin_y >= 0)) && ((fin_x < pointscene->pixelscene->x) && (fin_y < pointscene->pixelscene->y))) {
        
          if ( pointscene->pixelscene->pixelarray_single == NULL ) {
        
         pointscene->pixelscene->pixelarray_double[fin_x][fin_y] = pixcolor ;
            
          } else if ( pointscene->pixelscene->pixelarray_double == NULL ) {
             
         JHGPixels_SetPixel(pointscene->pixelscene, fin_x, fin_y, (JHGsubpixel)red, (JHGsubpixel)blue, (JHGsubpixel)green) ;
             
          }
            
        }
     }
}

codename_pixelcolor codename_NewColorObject( cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green ) {
    
    codename_pixelcolor pixcolor ;
    
    pixcolor = (codename_pixelcolor) malloc(sizeof(JHGPixelcolor_Object)) ;
    
    if ( pixcolor == NULL ) return NULL ;
    
    pixcolor->r = red ;
    
    pixcolor->b = blue ;

    pixcolor->g = green ;
    
    return pixcolor ;
    
}

void cnpixel_vline(codename_scene pointscene, int v) {
    
    
    int y = 0 ;
    

        while (y < pointscene->pixelscene->y) {
            
            
            cn_SetPixel(pointscene,v,y,255,255,255) ;
            
            
            y++ ;
        }     
}

void cnpixel_colorvline(codename_scene pointscene, int v, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    
    int y = 0 ;
    
    
        while (y < pointscene->pixelscene->y) {
            
            cn_SetPixel(pointscene,v,y,red,blue,green) ;
            
                        
            y++ ;
        } 
           
    
}

void cnpixel_vlinewithcolor(codename_scene pointscene, int v, codename_pixelcolor pixcolor) {
    
    
    int y = 0 ;
    
        while (y < pointscene->pixelscene->y) {
                
            
             cnpixel_SetColor(pointscene,v,y,pixcolor) ;
            
            
               y++ ;
                
            }    
    
}

void cnpixel_line(codename_scene pointscene, float m, float b, float scale, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green ) {
    
    
    float x = 0 ;
    
    float y = 0 ;
    
    while (x < pointscene->pixelscene->x) {
        y = 0 ;
        while (y < pointscene->pixelscene->y) {
            
            if ( y == (m*x)+b ) cn_SetPixel(pointscene,x,y,red,blue,green) ;
            
            y += scale ;
        }
        
        x += scale ;
    }
}

void cnpixel_linewithcolor(codename_scene pointscene, float m, float b, float scale, codename_pixelcolor pixcolor ) {
    
    float x = 0 ;
    
    float y = 0 ;
    
    while (x < pointscene->pixelscene->x) {
        y = 0 ;
        while (y < pointscene->pixelscene->y) {
            
            if ( y == (m*x)+b ) cnpixel_SetColor(pointscene,x,y,pixcolor) ;
            
            y += scale ;
        }
        
        x += scale ;
    }
}

void cnpixel_line_lowp(codename_scene pointscene, int m, int b, int scale, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green ) {
    
    
    int x = 0 ;
    
    int y = 0 ;
    
    while (x < pointscene->pixelscene->x) {
        y = 0 ;
        while (y < pointscene->pixelscene->y) {
            
            if ( y == (m*x)+b ) cn_SetPixel(pointscene,x,y,red,blue,green) ;
            
            y += scale ;
        }
        
        x += scale ;
    }
}

void cnpixel_linewithcolor_lowp(codename_scene pointscene, int m, int b, int scale, codename_pixelcolor pixcolor ) {
    
    int x = 0 ;
    
    int y = 0 ;
    
    while (x < pointscene->pixelscene->x) {
        y = 0 ;
        while (y < pointscene->pixelscene->y) {
            
            if ( y == (m*x)+b ) cnpixel_SetColor(pointscene,x,y,pixcolor) ;
            
            y += scale ;
        }
        
        x += scale ;
    }
}

void cnpoint_dotline(codename_scene pointscene, int line[], int x, int y, int size, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    int i = 0 ;
    int spaces = 0 ;
    
    if (line != NULL) {
        
        while (line[i] != -1) {
            
            if (line[i] == 1) {
                
                cnpoint_Rect(pointscene, size, size, x + (spaces * size), y, red, blue, green) ;
            }
            
            i++ ;
            spaces++ ;
        }
    }
}

void cnpoint_linesegment(codename_scene pointscene, int x1, int y1, int x2, int y2, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    cnpoint_line3d(pointscene,x1,y1,x2,y2,0,0,red,blue,green) ;
    
}

/*
 * line3d was dervied from DigitalLine.c published as "Digital Line Drawing"
 * by Paul Heckbert from "Graphics Gems", Academic Press, 1990
 * 
 * 3D modifications by Bob Pendleton. The original source code was in the public
 * domain, the author of the 3D version places his modifications in the
 * public domain as well.
 * 
 * line3d uses Bresenham's algorithm to generate the 3 dimensional points on a
 * line from (x1, y1, z1) to (x2, y2, z2)
 * 
 */

void cnpoint_line3d(codename_scene pointscene, int x1, int y1, int x2, int y2, int z1, int z2, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    int xd, yd, zd;
    int x, y, z;
    int ax, ay, az;
    int sx, sy, sz;
    int dx, dy, dz;
    
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    
    ax = ABS_JHG(dx) << 1;
    ay = ABS_JHG(dy) << 1;
    az = ABS_JHG(dz) << 1;
    
    sx = ZSGN(dx);
    sy = ZSGN(dy);
    sz = ZSGN(dz);
    
    x = x1;
    y = y1;
    z = z1;
    
    if (ax >= MAX_JHG(ay, az))            /* x dominant */
    {
        yd = ay - (ax >> 1);
        zd = az - (ax >> 1);
        for (;;)
        {
            cn_SetPoint(pointscene, x, y, red, blue, green);
            if (x == x2)
            {
                break;
            }
            
            if (yd >= 0)
            {
                y += sy;
                yd -= ax;
            }
            
            if (zd >= 0)
            {
                z += sz;
                zd -= ax;
            }
            
            x += sx;
            yd += ay;
            zd += az;
        }
    }
    else if (ay >= MAX_JHG(ax, az))            /* y dominant */
    {
        xd = ax - (ay >> 1);
        zd = az - (ay >> 1);
        for (;;)
        {
           cn_SetPoint(pointscene, x, y, red, blue, green);
            if (y == y2)
            {
                break;
            }
            
            if (xd >= 0)
            {
                x += sx;
                xd -= ay;
            }
            
            if (zd >= 0)
            {
                z += sz;
                zd -= ay;
            }
            
            y += sy;
            xd += ax;
            zd += az;
        }
    }
    else if (az >= MAX_JHG(ax, ay))            /* z dominant */
    {
        xd = ax - (az >> 1);
        yd = ay - (az >> 1);
        for (;;)
        {
            cn_SetPoint(pointscene, x, y, red, blue, green);
            if (z == z2)
            {
                break;
            }
            
            if (xd >= 0)
            {
                x += sx;
                xd -= az;
            }
            
            if (yd >= 0)
            {
                y += sy;
                yd -= az;
            }
            
            z += sz;
            xd += ax;
            yd += ay;
        }
    }
    
}


void cnpoint_Rect(codename_scene pointscene, int size_x, int size_y, int x, int y, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green ) {
    
    int i = 0 ;
    int j = 0 ;
    
    while (i < size_x) {
        j = 0 ;
        while (j < size_y) {
            cn_SetPoint(pointscene, (i + x), (j + y), red, blue, green) ;
           j++ ;
        }
     i++ ;
    }
}

void cnpixel_circle(codename_scene pointscene, float a, float b, float r, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green ) {
    
    float x = 0 ;
    
    float y = 0 ;
    
    while (x < pointscene->pixelscene->x) {
        y = 0 ;
        while (y < pointscene->pixelscene->y) {
            
            if ( powf(x-a,2) + powf(y-b,2) == powf(r,2) ) cn_SetPixel(pointscene,x,y,red,blue,green) ;
            
            y++ ;
        }
        
        x++ ;
    }   
    
}


// From Wikipedia -- Midpoint circle algorithm
void cnpoint_wikiCircle(codename_scene pointscene, int a, int b, int r, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green)
{
    int x0 = a ;
    int y0 = b ;
    int radius = r ;
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
    
    cn_SetPoint(pointscene,x0, y0 + radius,red,blue,green);
    cn_SetPoint(pointscene,x0, y0 - radius,red,blue,green);
    cn_SetPoint(pointscene,x0 + radius, y0,red,blue,green);
    cn_SetPoint(pointscene,x0 - radius, y0,red,blue,green);
    
    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;    
        cn_SetPoint(pointscene,x0 + x, y0 + y,red,blue,green);
        cn_SetPoint(pointscene,x0 - x, y0 + y,red,blue,green);
        cn_SetPoint(pointscene,x0 + x, y0 - y,red,blue,green);
        cn_SetPoint(pointscene,x0 - x, y0 - y,red,blue,green);
        cn_SetPoint(pointscene,x0 + y, y0 + x,red,blue,green);
        cn_SetPoint(pointscene,x0 - y, y0 + x,red,blue,green);
        cn_SetPoint(pointscene,x0 + y, y0 - x,red,blue,green);
        cn_SetPoint(pointscene,x0 - y, y0 - x,red,blue,green);
    }
}

void cnpoint_wikiArc(codename_scene pointscene, int a, int b, int r, double alpha, double beta, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green)
{
    double degrees_a = (alpha / 180) * M_PI;
    double degrees_b = (beta / 180) * M_PI;
    int x0 = a ;
    int y0 = b ;
    int radius = r ;
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
    
    cn_SetArcPoint(pointscene,x0, y0 + radius,a,b,radius,degrees_a,degrees_b,red,blue,green);
    cn_SetArcPoint(pointscene,x0, y0 - radius,a,b,radius,degrees_a,degrees_b,red,blue,green);
    cn_SetArcPoint(pointscene,x0 + radius,y0,a,b,radius,degrees_a,degrees_b, red,blue,green);
    cn_SetArcPoint(pointscene,x0 - radius,y0,a,b,radius,degrees_a,degrees_b, red,blue,green);
    
    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;    
        cn_SetArcPoint(pointscene,x0 + x, y0 + y,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 - x, y0 + y,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 + x, y0 - y,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 - x, y0 - y,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 + y, y0 + x,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 - y, y0 + x,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 + y, y0 - x,a,b,radius,degrees_a,degrees_b,red,blue,green);
        cn_SetArcPoint(pointscene,x0 - y, y0 - x,a,b,radius,degrees_a,degrees_b,red,blue,green);
    }
}

// From http://free.pages.at/easyfilter/bresenham.html -- plotBasicBezier
/********************************************************************
 *                                                                   *
 *                    Curve Rasterizing Algorithm                    *
 *                                                                   *
 ********************************************************************/

/**
 * @author Zingl Alois
 * @date 16.5.2012
 * @version 1.0
 */

void cnpoint_Curve(codename_scene pointscene,int x0, int y0, int x1, int y1, int x2, int y2, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {  
    
    int sx = x2-x1, sy = y2-y1;
    long xx = x0-x1, yy = y0-y1, xy;         /* relative values for checks */
    double dx, dy, err, cur = xx*sy-yy*sx;                    /* curvature */
    
    if (xx*sx <= 0 && yy*sy <= 0) {  /* sign of gradient must not change */
    
    if (sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) { /* begin with longer part */ 
        x2 = x0; x0 = sx+x1; y2 = y0; y0 = sy+y1; cur = -cur;  /* swap P0 P2 */
    }  
    if (cur != 0) {                                    /* no straight line */
        xx += sx; xx *= sx = x0 < x2 ? 1 : -1;           /* x step direction */
        yy += sy; yy *= sy = y0 < y2 ? 1 : -1;           /* y step direction */
        xy = 2*xx*yy; xx *= xx; yy *= yy;          /* differences 2nd degree */
        if (cur*sx*sy < 0) {                           /* negated curvature? */
            xx = -xx; yy = -yy; xy = -xy; cur = -cur;
        }
        dx = 4.0*sy*cur*(x1-x0)+xx-xy;             /* differences 1st degree */
        dy = 4.0*sx*cur*(y0-y1)+yy-xy;
        xx += xx; yy += yy; err = dx+dy+xy;                /* error 1st step */    
        do {                              
            cn_SetPoint(pointscene,x0,y0,red,blue,green) ;                                     /* plot curve */
            if (x0 == x2 && y0 == y2) return;  /* last pixel -> curve finished */
            y1 = 2*err < dx;                  /* save value for test of y step */
            if (2*err > dy) { x0 += sx; dx -= xy; err += dy += yy; } /* x step */
            if (    y1    ) { y0 += sy; dy -= xy; err += dx += xx; } /* y step */
        } while (dy < 0 && dx > 0);   /* gradient negates -> algorithm fails */
    }
   cnpoint_linesegment(pointscene, x0, y0, x2, y2, red, blue, green) ;                  /* plot remaining part to end */
        
    }
}  

void cnpoint_String(codename_scene pointscene, const char* string, int size, int x, int y, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    int strilen  = 0 ;
    int i = 0 ;
    
    strilen = (int) strlen( string ) ;
    
    while (i < strilen) {
      
        cnpoint_printchar(pointscene, string[i], size, x + ( i * (size * 5) ), y, red, blue, green) ;
        
        i++ ;
    }
}

void cnpoint_printchar(codename_scene pointscene, char c, int size, int x, int y, cn_RGBcolor red, cn_RGBcolor blue, cn_RGBcolor green) {
    
    //A
    int A1[6] = {0,0,1,0,0,-1} ;
    int A2[6] = {0,1,0,1,0,-1} ;
    int A3[6] = {0,1,1,1,0,-1} ;
    int A4[6] = {0,1,0,1,0,-1} ;
    int A5[6] = {0,1,0,1,0,-1} ;
    //B
    int B1[6] = {0,1,1,0,0,-1} ;
    int B2[6] = {0,1,0,1,0,-1} ;
    int B3[6] = {0,1,1,1,0,-1} ;
    int B4[6] = {0,1,0,1,0,-1} ;
    int B5[6] = {0,1,1,0,0,-1} ;
    //C
    int C1[6] = {0,0,1,1,0,-1} ;
    int C2[6] = {0,1,0,0,0,-1} ;
    int C3[6] = {0,1,0,0,0,-1} ;
    int C4[6] = {0,1,0,0,0,-1} ;
    int C5[6] = {0,0,1,1,0,-1} ;
    //D
    int D1[6] = {0,1,1,0,0,-1} ;
    int D2[6] = {0,1,0,1,0,-1} ;
    int D3[6] = {0,1,0,1,0,-1} ;
    int D4[6] = {0,1,0,1,0,-1} ;
    int D5[6] = {0,1,1,0,0,-1} ;
    //E
    int E1[6] = {0,1,1,1,0,-1} ;
    int E2[6] = {0,1,0,0,0,-1} ;
    int E3[6] = {0,1,1,0,0,-1} ;
    int E4[6] = {0,1,0,0,0,-1} ;
    int E5[6] = {0,1,1,1,0,-1} ;
    //F
    int F1[6] = {0,1,1,1,0,-1} ;
    int F2[6] = {0,1,0,0,0,-1} ;
    int F3[6] = {0,1,1,1,0,-1} ;
    int F4[6] = {0,1,0,0,0,-1} ;
    int F5[6] = {0,1,0,0,0,-1} ;
    //G
    int G1[6] = {0,1,1,1,0,-1} ;
    int G2[6] = {0,1,0,0,0,-1} ;
    int G3[6] = {0,1,0,1,0,-1} ;
    int G4[6] = {0,1,0,1,0,-1} ;
    int G5[6] = {0,1,1,1,0,-1} ;
    //H
    int H1[6] = {0,1,0,1,0,-1} ;
    int H2[6] = {0,1,0,1,0,-1} ;
    int H3[6] = {0,1,1,1,0,-1} ;
    int H4[6] = {0,1,0,1,0,-1} ;
    int H5[6] = {0,1,0,1,0,-1} ;
    //I
    int I1[6] = {0,1,1,1,0,-1} ;
    int I2[6] = {0,0,1,0,0,-1} ;
    int I3[6] = {0,0,1,0,0,-1} ;
    int I4[6] = {0,0,1,0,0,-1} ;
    int I5[6] = {0,1,1,1,0,-1} ;
    //J
    int J1[6] = {0,1,1,1,0,-1} ;
    int J2[6] = {0,0,1,0,0,-1} ;
    int J3[6] = {0,0,1,0,0,-1} ;
    int J4[6] = {0,0,1,0,0,-1} ;
    int J5[6] = {0,1,1,0,0,-1} ;
    //K
    int K1[6] = {0,1,0,1,0,-1} ;
    int K2[6] = {0,1,1,0,0,-1} ;
    int K3[6] = {0,1,1,0,0,-1} ;
    int K4[6] = {0,1,1,0,0,-1} ;
    int K5[6] = {0,1,0,1,0,-1} ;
    //L
    int L1[6] = {0,1,0,0,0,-1} ;
    int L2[6] = {0,1,0,0,0,-1} ;
    int L3[6] = {0,1,0,0,0,-1} ;
    int L4[6] = {0,1,0,0,0,-1} ;
    int L5[6] = {0,1,1,1,0,-1} ;
    //M
    int M1[6] = {1,1,0,1,1,-1} ;
    int M2[6] = {1,0,1,0,1,-1} ;
    int M3[6] = {1,0,1,0,1,-1} ;
    int M4[6] = {1,0,1,0,1,-1} ;
    int M5[6] = {1,0,0,0,1,-1} ;
    //N
    int N1[6] = {0,1,0,0,1,-1} ;
    int N2[6] = {0,1,1,0,1,-1} ;
    int N3[6] = {0,1,0,1,1,-1} ;
    int N4[6] = {0,1,0,0,1,-1} ;
    int N5[6] = {0,1,0,0,1,-1} ;
    //O
    int O1[6] = {0,1,1,1,0,-1} ;
    int O2[6] = {0,1,0,1,0,-1} ;
    int O3[6] = {0,1,0,1,0,-1} ;
    int O4[6] = {0,1,0,1,0,-1} ;
    int O5[6] = {0,1,1,1,0,-1} ;
    //P
    int P1[6] = {0,1,1,1,0,-1} ;
    int P2[6] = {0,1,0,1,0,-1} ;
    int P3[6] = {0,1,1,1,0,-1} ;
    int P4[6] = {0,1,0,0,0,-1} ;
    int P5[6] = {0,1,0,0,0,-1} ;
    //Q
    int Q1[6] = {0,1,1,1,0,-1} ;
    int Q2[6] = {0,1,0,1,0,-1} ;
    int Q3[6] = {0,1,0,1,0,-1} ;
    int Q4[6] = {0,1,1,1,0,-1} ;
    int Q5[6] = {0,0,0,1,0,-1} ;
    //R
    int R1[6] = {0,1,1,1,0,-1} ;
    int R2[6] = {0,1,0,1,0,-1} ;
    int R3[6] = {0,1,1,1,0,-1} ;
    int R4[6] = {0,1,0,1,0,-1} ;
    int R5[6] = {0,1,0,1,0,-1} ;
    //S
    int S1[6] = {0,0,1,1,0,-1} ;
    int S2[6] = {0,0,1,0,0,-1} ;
    int S3[6] = {0,0,1,0,0,-1} ;
    int S4[6] = {0,0,0,1,0,-1} ;
    int S5[6] = {0,0,1,1,0,-1} ;
    //T
    int T1[6] = {0,1,1,1,0,-1} ;
    int T2[6] = {0,0,1,0,0,-1} ;
    int T3[6] = {0,0,1,0,0,-1} ;
    int T4[6] = {0,0,1,0,0,-1} ;
    int T5[6] = {0,0,1,0,0,-1} ;
    //U
    int U1[6] = {0,1,0,1,0,-1} ;
    int U2[6] = {0,1,0,1,0,-1} ;
    int U3[6] = {0,1,0,1,0,-1} ;
    int U4[6] = {0,1,0,1,0,-1} ;
    int U5[6] = {0,1,1,1,0,-1} ;
    //V
    int V1[6] = {0,1,0,1,0,-1} ;
    int V2[6] = {0,1,0,1,0,-1} ;
    int V3[6] = {0,1,0,1,0,-1} ;
    int V4[6] = {0,1,0,1,0,-1} ;
    int V5[6] = {0,0,1,0,0,-1} ;
    //W
    int W1[6] = {1,0,0,0,1,-1} ;
    int W2[6] = {1,0,1,0,1,-1} ;
    int W3[6] = {1,0,1,0,1,-1} ;
    int W4[6] = {1,0,1,0,1,-1} ;
    int W5[6] = {1,1,0,1,1,-1} ;
    //X
    int X1[6] = {0,1,0,1,0,-1} ;
    int X2[6] = {0,1,0,1,0,-1} ;
    int X3[6] = {0,0,1,0,0,-1} ;
    int X4[6] = {0,1,0,1,0,-1} ;
    int X5[6] = {0,1,0,1,0,-1} ;
    //Y
    int Y1[6] = {0,1,0,1,0,-1} ;
    int Y2[6] = {0,1,0,1,0,-1} ;
    int Y3[6] = {0,1,0,1,0,-1} ;
    int Y4[6] = {0,0,1,0,0,-1} ;
    int Y5[6] = {0,0,1,0,0,-1} ;
    //Z
    int Z1[6] = {0,1,1,1,0,-1} ;
    int Z2[6] = {0,0,0,1,0,-1} ;
    int Z3[6] = {0,0,1,0,0,-1} ;
    int Z4[6] = {0,1,0,0,0,-1} ;
    int Z5[6] = {0,1,1,1,0,-1} ;
    //.
    int PE1[6] = {0,0,0,0,0,-1} ;
    int PE2[6] = {0,0,0,0,0,-1} ;
    int PE3[6] = {0,0,0,0,0,-1} ;
    int PE4[6] = {0,0,0,0,0,-1} ;
    int PE5[6] = {0,0,1,0,0,-1} ;
    //!
    int EP1[6] = {0,0,1,0,0,-1} ;
    int EP2[6] = {0,0,1,0,0,-1} ;
    int EP3[6] = {0,0,1,0,0,-1} ;
    int EP4[6] = {0,0,0,0,0,-1} ;
    int EP5[6] = {0,0,1,0,0,-1} ;
    //?
    int QM1[6] = {0,1,1,0,0,-1} ;
    int QM2[6] = {0,0,1,0,0,-1} ;
    int QM3[6] = {0,0,1,0,0,-1} ;
    int QM4[6] = {0,0,0,0,0,-1} ;
    int QM5[6] = {0,0,1,0,0,-1} ;
    //1
    int ONE1[6] = {0,1,1,0,0,-1} ;
    int ONE2[6] = {0,0,1,0,0,-1} ;
    int ONE3[6] = {0,0,1,0,0,-1} ;
    int ONE4[6] = {0,0,1,0,0,-1} ;
    int ONE5[6] = {0,1,1,1,0,-1} ;
    //2
    int TWO1[6] = {0,0,1,1,0,-1} ;
    int TWO2[6] = {0,1,0,0,1,-1} ;
    int TWO3[6] = {0,0,0,1,0,-1} ;
    int TWO4[6] = {0,0,1,0,0,-1} ;
    int TWO5[6] = {0,1,1,1,1,-1} ;
    //3
    int THREE1[6] = {0,0,1,1,0,-1} ;
    int THREE2[6] = {0,1,0,0,1,-1} ;
    int THREE3[6] = {0,0,0,1,1,-1} ;
    int THREE4[6] = {0,1,0,0,1,-1} ;
    int THREE5[6] = {0,0,1,1,0,-1} ;
    //4
    int FOUR1[6] = {0,1,0,1,0,-1} ;
    int FOUR2[6] = {0,1,0,1,0,-1} ;
    int FOUR3[6] = {0,1,1,1,0,-1} ;
    int FOUR4[6] = {0,0,0,1,0,-1} ;
    int FOUR5[6] = {0,0,0,1,0,-1} ;
    //5
    int FIVE1[6] = {0,1,1,1,0,-1} ;
    int FIVE2[6] = {0,1,0,0,0,-1} ;
    int FIVE3[6] = {0,1,1,1,0,-1} ;
    int FIVE4[6] = {0,0,0,1,0,-1} ;
    int FIVE5[6] = {0,1,1,1,0,-1} ;
    //6
    int SIX1[6] = {0,1,1,1,0,-1} ;
    int SIX2[6] = {0,1,0,0,0,-1} ;
    int SIX3[6] = {0,1,1,1,0,-1} ;
    int SIX4[6] = {0,1,0,1,0,-1} ;
    int SIX5[6] = {0,1,1,1,0,-1} ;
    //7
    int SEVEN1[6] = {0,1,1,1,1,-1} ;
    int SEVEN2[6] = {0,0,0,0,1,-1} ;
    int SEVEN3[6] = {0,0,0,1,0,-1} ;
    int SEVEN4[6] = {0,0,1,0,0,-1} ;
    int SEVEN5[6] = {0,1,0,0,0,-1} ;
    //8
    int EIGHT1[6] = {0,1,1,1,0,-1} ;
    int EIGHT2[6] = {0,1,0,1,0,-1} ;
    int EIGHT3[6] = {0,1,1,1,0,-1} ;
    int EIGHT4[6] = {0,1,0,1,0,-1} ;
    int EIGHT5[6] = {0,1,1,1,0,-1} ;
    //9
    int NINE1[6] = {0,1,1,1,0,-1} ;
    int NINE2[6] = {0,1,0,1,0,-1} ;
    int NINE3[6] = {0,1,1,1,0,-1} ;
    int NINE4[6] = {0,0,0,1,0,-1} ;
    int NINE5[6] = {0,1,1,1,0,-1} ;
    //0
    int ZERO1[6] = {0,0,1,1,0,-1} ;
    int ZERO2[6] = {0,1,0,0,1,-1} ;
    int ZERO3[6] = {0,1,0,0,1,-1} ;
    int ZERO4[6] = {0,1,0,0,1,-1} ;
    int ZERO5[6] = {0,0,1,1,0,-1} ;

    switch (c) {
        case 'A':
            cnpoint_dotline(pointscene, A1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, A2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, A3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, A4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, A5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'B':
            cnpoint_dotline(pointscene, B1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, B2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, B3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, B4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, B5,x,(y + (0 * size)),size,red,blue,green) ;
            break; 
        case 'C':
            cnpoint_dotline(pointscene, C1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, C2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, C3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, C4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, C5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'D':
            cnpoint_dotline(pointscene, D1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, D2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, D3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, D4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, D5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'E':
            cnpoint_dotline(pointscene, E1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, E2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, E3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, E4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, E5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'F':
            cnpoint_dotline(pointscene, F1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, F2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, F3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, F4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, F5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'G':
            cnpoint_dotline(pointscene, G1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, G2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, G3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, G4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, G5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'H':
            cnpoint_dotline(pointscene, H1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, H2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, H3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, H4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, H5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'I':
            cnpoint_dotline(pointscene, I1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, I2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, I3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, I4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, I5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'J':
            cnpoint_dotline(pointscene, J1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, J2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, J3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, J4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, J5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'K':
            cnpoint_dotline(pointscene, K1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, K2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, K3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, K4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, K5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'L':
            cnpoint_dotline(pointscene, L1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, L2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, L3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, L4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, L5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'M':
            cnpoint_dotline(pointscene, M1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, M2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, M3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, M4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, M5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'N':
            cnpoint_dotline(pointscene, N1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, N2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, N3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, N4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, N5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'O':
            cnpoint_dotline(pointscene, O1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, O2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, O3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, O4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, O5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'P':
            cnpoint_dotline(pointscene, P1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, P2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, P3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, P4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, P5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'Q':
            cnpoint_dotline(pointscene, Q1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Q2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Q3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Q4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Q5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'R':
            cnpoint_dotline(pointscene, R1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, R2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, R3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, R4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, R5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'S':
            cnpoint_dotline(pointscene, S1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, S2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, S3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, S4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, S5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'T':
            cnpoint_dotline(pointscene, T1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, T2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, T3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, T4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, T5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'U':
            cnpoint_dotline(pointscene, U1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, U2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, U3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, U4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, U5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'V':
            cnpoint_dotline(pointscene, V1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, V2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, V3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, V4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, V5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'W':
            cnpoint_dotline(pointscene, W1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, W2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, W3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, W4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, W5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'X':
            cnpoint_dotline(pointscene, X1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, X2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, X3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, X4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, X5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'Y':
            cnpoint_dotline(pointscene, Y1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Y2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Y3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Y4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Y5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case 'Z':
            cnpoint_dotline(pointscene, Z1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Z2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Z3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Z4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, Z5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '.':
            cnpoint_dotline(pointscene, PE1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, PE2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, PE3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, PE4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, PE5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '!':
            cnpoint_dotline(pointscene, EP1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EP2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EP3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EP4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EP5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '?':
            cnpoint_dotline(pointscene, QM1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, QM2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, QM3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, QM4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, QM5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '1':
            cnpoint_dotline(pointscene, ONE1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ONE2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ONE3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ONE4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ONE5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '2':
            cnpoint_dotline(pointscene, TWO1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, TWO2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, TWO3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, TWO4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, TWO5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '3':
            cnpoint_dotline(pointscene, THREE1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, THREE2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, THREE3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, THREE4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, THREE5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '4':
            cnpoint_dotline(pointscene, FOUR1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FOUR2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FOUR3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FOUR4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FOUR5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '5':
            cnpoint_dotline(pointscene, FIVE1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FIVE2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FIVE3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FIVE4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, FIVE5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '6':
            cnpoint_dotline(pointscene, SIX1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SIX2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SIX3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SIX4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SIX5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '7':
            cnpoint_dotline(pointscene, SEVEN1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SEVEN2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SEVEN3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SEVEN4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, SEVEN5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '8':
            cnpoint_dotline(pointscene, EIGHT1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EIGHT2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EIGHT3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EIGHT4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, EIGHT5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '9':
            cnpoint_dotline(pointscene, NINE1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, NINE2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, NINE3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, NINE4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, NINE5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        case '0':
            cnpoint_dotline(pointscene, ZERO1,x,(y + (4 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ZERO2,x,(y + (3 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ZERO3,x,(y + (2 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ZERO4,x,(y + (1 * size)),size,red,blue,green) ;
            cnpoint_dotline(pointscene, ZERO5,x,(y + (0 * size)),size,red,blue,green) ;
            break;
        default:
            break;
    }
    
}