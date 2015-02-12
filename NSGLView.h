#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

#import "GLBase.h"

@interface NSGLView : NSOpenGLView {
	CVDisplayLinkRef displayLink;

}

@end