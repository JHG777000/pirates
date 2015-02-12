//
//  RKWindow_Mac.m
//  pirates_stage1
//
//  Created by Jacob Gordon on 12/2/13.
//  Copyright (c) 2013 Jacob Gordon. All rights reserved.
//

#import "RKWindow.h"
#import "RKWindow_Mac.h"

static NSUInteger stylemask = (NSTitledWindowMask | NSMiniaturizableWindowMask ) ;

@implementation RKWindow_Mac

void RKWindow_CreateWindow( int width, int height ) {
    
    [[RKWindow_Mac alloc] StartWindow:width Andheight:height] ;
    
}


- (void)StartWindow:(CGFloat)width Andheight: (CGFloat)height {
    
    NSRect rect ;
    
    rect.origin.x = 300 ;
    
    rect.origin.y = 100 ;
    
    rect.size.height = height ;
    
    rect.size.width = width ;
    
    gamewindow = [[NSWindow alloc] initWithContentRect:rect styleMask: stylemask backing:NSBackingStoreBuffered defer:YES] ;
    
    [ gamewindow setTitle:@"Game" ] ;
    
    nsglview = [[NSGLView alloc] initWithFrame:rect] ;
    
    [ gamewindow setContentView: nsglview ] ;
    
    [ gamewindow makeKeyAndOrderFront:self ] ;
    
    [ gamewindow setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary ] ;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(FullScreener:)
                                                 name:NSWindowWillEnterFullScreenNotification object:nil] ;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(Windower:)
                                                 name:NSWindowWillExitFullScreenNotification object:nil] ;
    
}

- (void)FullScreener:(NSNotification *)notification {
    
    //[[gamewindow contentView] setWantsBestResolutionOpenGLSurface:YES] ;
    
    NSRect mainDisplayRect = [[NSScreen mainScreen] frame] ;
    
    [ gamewindow setContentSize:mainDisplayRect.size ] ;
    [ gamewindow setStyleMask:NSBorderlessWindowMask] ;
    [ gamewindow setStyleMask:NSFullScreenWindowMask] ;
    
}

- (void)Windower:(NSNotification *)notification {
    
    [ gamewindow setStyleMask:stylemask ] ;
    
    [ gamewindow setTitle:@"Game" ] ;
    
}

@end
