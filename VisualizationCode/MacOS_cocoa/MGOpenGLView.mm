//
//  MGOpenGLView.m
//  MazeGen2
//
//  Created by Patrick Wingo on 7/16/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#import "MGOpenGLView.h"
//#import "ViewController.h"
//#import "GLErrStream.hpp"

#import "GLErrStream.hpp"
#import<random>
#import<chrono>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

@implementation MGOpenGLView
@synthesize controller=_vCtrl;

- (id)initWithFrame:(NSRect)frame
{
    _shadeDir=@"Shaders";
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    [self awakeFromNib];
    
    return self;
}

- (id)initWithFrame:(NSRect)frame shadeDir:(NSString*)shdrs
{
    self=[self initWithFrame:frame];
    _shadeDir=shdrs;
    return self;
}

-(void)awakeFromNib
{
    
    //initialize the shader manager
    if (!_shadeDir)
        _shadeDir=@"Shaders";
        //_shadeDir=[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/Shaders/"];
    
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
        if(_visMgr->GetPathTime()<_visMgr->GetPathLength()+8)
        {
            _visMgr->SetPathTime(_visMgr->GetPathTime()+0.001);
            [self setNeedsDisplay:YES];
        }
        else
            [_vCtrl performSelector:@selector(RefreshMaze:) withObject:nil];
    }];
    
    
    
    ASSERT_GL("Postawake");
}

-(void)prepareOpenGL
{
    NSLog(@"Preparing Opengl");
    //initialization code here
    [super prepareOpenGL];
    NSRect bounds = self.bounds;
    glEnable (GL_BLEND);
    //glEnable( GL_MULTISAMPLE );
    
    [self willChangeValueForKey:@"gridColor"];
    [self willChangeValueForKey:@"wallColor"];
    [self willChangeValueForKey:@"pathColor"];
    [self willChangeValueForKey:@"decayColor"];
    [self willChangeValueForKey:@"drawFullPath"];
    [self willChangeValueForKey:@"showPathDecayColor"];
    [self willChangeValueForKey:@"decayDelay"];
    [self willChangeValueForKey:@"applyRot"];
    _visMgr=new VisMgr([_shadeDir cStringUsingEncoding:NSASCIIStringEncoding],bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
    _visMgr->InitForOpenGL();
    [self didChangeValueForKey:@"gridColor"];
    [self didChangeValueForKey:@"wallColor"];
    [self didChangeValueForKey:@"pathColor"];
    [self didChangeValueForKey:@"decayColor"];
    [self didChangeValueForKey:@"drawFullPath"];
    [self didChangeValueForKey:@"showPathDecayColor"];
    [self didChangeValueForKey:@"decayDelay"];
    [self didChangeValueForKey:@"applyRot"];
    //_visMgr->InitTest();
    ASSERT_GL("Postawake");
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
    if(_visMgr->ReadyToDraw())
        _visMgr->Draw();
    else
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    [[self openGLContext] flushBuffer];
    ASSERT_GL("InitFail");
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
    //random stuff
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distribution(-0.1,0.1);
    NSLog(@"randoms initialized");
    _visMgr->RefreshWithMaze(bldr);
    NSLog(@"VisMgr refreshed");
    _visMgr->SetPathTime(0.0);
    _visMgr->SetRotAxis(distribution(generator),distribution(generator),distribution(generator));
    [self setNeedsDisplay:YES];
    ASSERT_GL("Refresh Posted");
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
    if(_visMgr)
        return _visMgr->GetShowFullPath();
    return NO;
}

-(void)setDrawPath:(BOOL)drawPath
{
    if(_visMgr)
        _visMgr->SetShowFullPath(drawPath);
}

-(BOOL)getDrawDecay
{
    if(_visMgr)
        return _visMgr->GetShowPathDecay();
    return NO;
}

-(void)setDrawDecay:(BOOL)showDecay
{
    if(_visMgr)
        _visMgr->SetShowPathDecay(showDecay);
}

-(float)getDecayDelay
{
    if(_visMgr)
        return _visMgr->GetDecayDelay();
    return 0.1;
}

-(void)setDecayDelay:(float)delay
{
    if(_visMgr)
        _visMgr->SetDecayDelay(delay);
}

-(BOOL)getApplyRot
{
    if(_visMgr)
        return _visMgr->GetApplyRot();
    return YES;
}

-(void)setApplyRot:(BOOL)apply
{
    if(_visMgr)
        _visMgr->SetApplyRot(apply);
}

-(void)dealloc
{
    delete _visMgr;
}
@end
