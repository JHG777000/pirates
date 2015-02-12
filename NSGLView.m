
#import "NSGLView.h"
#import "GLBase.h"
#import "JHGPixelslib.h"
#import "PlaceToDrawStuff.h"
#import "RKMem.h"

@interface NSGLView (PrivateMethods)
- (void) initGL;
- (void) drawView;


@end

@implementation NSGLView
static id selfAwareness ;
static NSSize windowSize ;
static void SetWindowSize(int width, int height) ;
static void GetWindowSize(int *width, int *height) ;
static void mac_theMouse(float x, float y) ;
static  NSTrackingArea* trackingArea ;
static float old_x = 0 ;
static float old_y = 0 ;
- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
	// There is no autorelease pool when this method is called 
	// because it will be called from a background thread
	// It's important to create one or you will leak objects
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	[self drawView];
	
	[pool release];
	return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(NSGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (id) initWithFrame:(NSRect)frameRect
{
	NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile,
		NSOpenGLProfileVersion3_2Core,
		0
	};
	
	NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	
	if (!pf)
	{
		NSLog(@"No OpenGL pixel format");
	}
    
	self = [super initWithFrame:frameRect pixelFormat:[pf autorelease]];
	
    //[self  setWantsBestResolutionOpenGLSurface:YES];
    
    trackingArea = [[NSTrackingArea alloc] initWithRect:frameRect
                                                options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow )
                                                  owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
    
	return self;
}

- (void) prepareOpenGL
{
	[super prepareOpenGL];

	// Make all the OpenGL calls to setup rendering  
	//  and build the necessary rendering objects
	[self initGL];
	
	// Create a display link capable of being used with all active displays
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	
	// Set the renderer output callback function
	CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
	
	// Set the display link for the current renderer
	CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
	
	// Activate the display link
	CVDisplayLinkStart(displayLink);
}

- (void) Windowresizewidth:(CGFloat)width Andheight: (CGFloat)height {
    
    NSSize size ;
    
    size.width = width ;
    
    size.height = height ;
    
    [[self window] setContentSize:size] ; 
    
}

void SetWindowSize(int width, int height) {
    
[selfAwareness Windowresizewidth:(((CGFloat) width) - 42) Andheight:(((CGFloat) height) - 33)] ;
    
}

void GetWindowSize(int *width, int *height) {
    
    *width = (int) windowSize.width ;
    
    *height = (int) windowSize.height ;
    
}

- (void) initGL
{
	// Make this openGL context current to the thread
	// (i.e. all openGL on this thread calls will go to this context)
	[[self openGLContext] makeCurrentContext];
	
	// Synchronize buffer swaps with vertical refresh rate
	GLint swapInt = 1;
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
     //Window Init.
     selfAwareness = self ;
     JHGPixels_createwindow(1024,1024, GetWindowSize, SetWindowSize) ;
    
	//renderer Init.
    startup();
}

- (void) reshape
{	
	[super reshape];
	
	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously when resizing
	CGLLockContext([[self openGLContext] CGLContextObj]);
	
	NSRect rect = [self convertRectToBacking:[self bounds]];
	
	resize(rect.size.width, rect.size.height);
    
    windowSize.width = rect.size.width ;
    
    windowSize.height = rect.size.height ;
    
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void) drawView
{	 
	[[self openGLContext] makeCurrentContext];

	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously	when resizing
	CGLLockContext([[self openGLContext] CGLContextObj]);
	
	drawstuff();
	
	CGLFlushDrawable([[self openGLContext] CGLContextObj]);
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)updateTrackingAreas {
    NSRect eyeBox = [self bounds];
    [self removeTrackingArea:trackingArea];
    [trackingArea release];
    trackingArea = [[NSTrackingArea alloc] initWithRect:eyeBox
                                                options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow)
                                                  owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
}

void mac_theMouse( float x, float y ) {
    
    int ResWidth ;
    int ResHeight ;
    int WinWidth ;
    int WinHeight ;
    int sign_x ;
    int sign_y ;
    float diff_x;
    float diff_y;
    float scale_x ;
    float scale_y ;
    float count_x = 0 ;
    float count_y = 0 ;
    
    
    JHGPixels_GetMainResolution(&ResWidth, &ResHeight) ;
    JHGPixels_GetWindowSize(&WinWidth, &WinHeight) ;
    
    sign_x = (WinWidth > ResWidth) ? -1 : 1 ;
    diff_x = (ResWidth - WinWidth) * sign_x ;
    
    scale_x = (diff_x < 1) ? 0 : WinWidth / diff_x ;
    
    sign_y = (WinHeight > ResHeight) ? -1 : 1 ;
    diff_y = (ResHeight - WinHeight) * sign_y ;
    
    scale_y = (diff_y < 1) ? 0 : WinHeight / diff_y ;
    
    count_x = (x/diff_x) * 1 ;
    
    if ( count_x < 0 ) count_x = 0 ;
    
    if ( count_x > diff_x ) count_x = diff_x ;
    
    count_y = (y/diff_y) * 15  ;
    
    if ( count_y < 0 ) count_y = 0 ;
    
    if ( count_y > diff_y ) count_y = diff_y ;
    
    theMouse((x + (  count_x * scale_x)), (y + (  count_y * scale_y))) ;
    
    old_x = x ;
    old_y = y ;
    
}

- (void)mouseEntered:(NSEvent *)theEvent {
    [NSCursor hide];
    NSPoint eyeCenter = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    NSRect eyeBox = NSMakeRect((eyeCenter.x-10.0), (eyeCenter.y-10.0), 20.0, 20.0);
    
    NSPoint mousepoint ;
    mousepoint = [theEvent locationInWindow] ;
    mac_theMouse(mousepoint.x, mousepoint.y ) ;
    
    [self setNeedsDisplayInRect:eyeBox];
    [self displayIfNeeded];
}

- (void)mouseMoved:(NSEvent *)theEvent {
    NSPoint eyeCenter = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    NSRect eyeBox = NSMakeRect((eyeCenter.x-10.0), (eyeCenter.y-10.0), 20.0, 20.0);
    
    NSPoint mousepoint ;
    mousepoint = [theEvent locationInWindow] ;
    mac_theMouse(mousepoint.x, mousepoint.y ) ;
    
    [self setNeedsDisplayInRect:eyeBox];
    [self displayIfNeeded];
}

- (void)mouseExited:(NSEvent *)theEvent {
    [NSCursor unhide];
    NSPoint mousepoint ;
    mousepoint = [theEvent locationInWindow] ;
    mac_theMouse(mousepoint.x, mousepoint.y ) ;
    //JHGPixels_SetWindowSize(1024, 1024);
    [self displayIfNeeded];
}

- (void) dealloc
{	
    JHGPixels_destroywindow();
    
	cleanup() ;
	
	// Release the display link
	CVDisplayLinkRelease(displayLink);
	
	[super dealloc];
}
@end
