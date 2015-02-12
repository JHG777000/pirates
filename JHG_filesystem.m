//
//  JHG_filesystem.m
//  JHG-GL
//
//  Created by Jacob Gordon on 6/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#import "JHG_filesystem.h"


const char* get_filePathName_mac(const char* filename,const char* filetype) {
   
    NSString* filePathName = nil;
    
    NSString* filename_objc = [[NSString alloc] initWithUTF8String:filename];
    
    NSString* filetype_objc = [[NSString alloc] initWithUTF8String:filetype];
    
    filePathName = [[NSBundle mainBundle] pathForResource:filename_objc ofType:filetype_objc];
    
    [filename_objc release];
    
    [filetype_objc release];
   
    return [filePathName cStringUsingEncoding:NSASCIIStringEncoding] ;
    
    
}