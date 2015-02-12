//
//  AppDelegate.m
//  NumShock
//
//  Created by Jacob Gordon on 12/30/13.
//  Copyright (c) 2013 Jacob Gordon. All rights reserved.
//

#import "codename2D.h"
#import "PlaceToDrawStuff.h"
#import "AppDelegate.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    codename2D_CreateDrawArea(MyDrawFuntion, 1024, 1024, 900, 800) ;
}

@end
