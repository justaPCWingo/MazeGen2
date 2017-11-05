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
    
    //set up path time
    _pathTimer=[NSTimer scheduledTimerWithTimeInterval:0.001 repeats:YES block:^(NSTimer* theTimer)
    {
        _visMgr->SetPathTime(_visMgr->GetPathTime()+0.001);
        [self setNeedsDisplay:YES];
    }];
}

-(void)prepareOpenGL
{
    //initialization code here
    [super prepareOpenGL];
    
    NSRect bounds=[self bounds];
    
    glEnable (GL_BLEND);
    
    [self willChangeValueForKey:@"gridColor"];
    [self willChangeValueForKey:@"wallColor"];
    [self willChangeValueForKey:@"pathColor"];
    [self willChangeValueForKey:@"decayColor"];
    [self willChangeValueForKey:@"drawFullPath"];
    [self willChangeValueForKey:@"showPathDecayColor"];
    _visMgr=new VisMgr(SHADERS_DIR,bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
    _visMgr->InitForOpenGL();
    [self didChangeValueForKey:@"gridColor"];
    [self didChangeValueForKey:@"wallColor"];
    [self didChangeValueForKey:@"pathColor"];
    [self didChangeValueForKey:@"decayColor"];
    [self didChangeValueForKey:@"drawFullPath"];
    [self didChangeValueForKey:@"showPathDecayColor"];
    //_visMgr->InitTest();
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
    _visMgr->Draw();
    [[self openGLContext] flushBuffer];
}

-(void)reshape
{
    NSRect bounds=[self bounds];
    _visMgr->SetViewport(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
}

-(void)newMaze:(MazeBuilder*)bldr
{
    _visMgr->NewMaze(bldr);
    //[[NSRunLoop currentRunLoop] addTimer:_pathTimer forMode:NSEventTrackingRunLoopMode];
}

-(void)refreshWithMaze:(MazeBuilder*)bldr
{
    _visMgr->RefreshWithMaze(bldr);
    _visMgr->SetPathTime(0.0);
    [self setNeedsDisplay:YES];
}

#define PW_VIS_COLOR(getProp,setProp,GetFn,SetFn) \
-(NSColor*)getProp {\
if (_visMgr) \
{ \
    glm::vec3 raw=_visMgr->GetFn(); \
    return [NSColor colorWithRed:raw[0] green:raw[1] blue:raw[2] alpha:1.0]; \
} \
return [NSColor blackColor]; \
} \
-(void)setProp:(NSColor*)c \
{ \
_visMgr->SetFn(glm::vec3(c.redComponent,c.greenComponent,c.blueComponent)); \
 \
} \
\
\

PW_VIS_COLOR(getGridColor,setGridColor, GetGridColor,SetGridColor)
PW_VIS_COLOR(getWallColor,setWallColor, GetWallColor,SetWallColor)
PW_VIS_COLOR(getPathColor,setPathColor, GetPathColor,SetPathColor)
PW_VIS_COLOR(getDecayColor,setDecayColor, GetDecayedPathColor,SetDecayedPathColor)

#undef PW_VIS_COLOR

-(BOOL)getDrawPath
{
    return _visMgr->GetShowFullPath();
}

-(void)setDrawPath:(BOOL)drawPath
{
    _visMgr->SetShowFullPath(drawPath);
}

-(BOOL)getDrawDecay
{
    return _visMgr->GetShowPathDecay();
}

-(void)setDrawDecay:(BOOL)showDecay
{
    _visMgr->SetShowPathDecay(showDecay);
}

-(void)dealloc
{
    delete _visMgr;
}
@end
