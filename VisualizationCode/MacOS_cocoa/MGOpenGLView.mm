//
//  MGOpenGLView.m
//  MazeGen2
//
//  Created by Patrick Wingo on 7/16/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#import "MGOpenGLView.h"
//#import "GLErrStream.hpp"

#ifndef SHADERS_DIR
#define SHADERS_DIR "shaders"
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

@implementation MGOpenGLView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(void)awakeFromNib
{
    
    //initialize the shader manager
    //NSString* shaderPath=[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/Shaders/"];
    
    //set up initial rendering info.
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
    {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFAColorSize,32,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        0
    };
    
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes];
    
    NSOpenGLContext* openGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil] ;
    
    [self setPixelFormat:pixelFormat];
    [self setOpenGLContext:openGLContext];
    [openGLContext makeCurrentContext];
    

}

-(void)prepareOpenGL
{
    //initialization code here
    [super prepareOpenGL];
    
    NSRect bounds=[self bounds];
    glViewport(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
    glEnable (GL_BLEND);
    _visMgr=new VisMgr(SHADERS_DIR);
    _visMgr->InitForOpenGL();
    //_visMgr->InitTest();
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
    _visMgr->Draw();
    [[self openGLContext] flushBuffer];
}

-(void)newMaze:(MazeBuilder*)bldr
{
    _visMgr->NewMaze(bldr);
    
}

-(void)refreshWithMaze:(MazeBuilder*)bldr
{
    _visMgr->RefreshWithMaze(bldr);
    [self setNeedsDisplay:YES];
}

-(void)dealloc
{
    delete _visMgr;
}
@end
