//
//  MGOpenGLView.h
//  MazeGen2
//
//  Created by Patrick Wingo on 7/16/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

#import <vector>
#import "MazeBuilder.h"
#import "../ShaderMgr.h"
#import "../VisMgr.h"

@class ViewController;

@interface MGOpenGLView : NSOpenGLView
{
    VisMgr* _visMgr;
    NSTimer* _pathTimer;
    ViewController* _vCtrl;
}

-(void)newMaze:(MazeBuilder*)bldr;
-(void)refreshWithMaze:(MazeBuilder*)bldr;

@property(getter=getGridColor,setter=setGridColor:) NSColor* gridColor;
@property(getter=getWallColor,setter=setWallColor:) NSColor* wallColor;
@property(getter=getPathColor,setter=setPathColor:) NSColor* pathColor;
@property(getter=getDecayColor,setter=setDecayColor:) NSColor* decayColor;
@property(getter=getDrawPath,setter=setDrawPath:) BOOL drawFullPath;
@property(getter=getDrawDecay,setter=setDrawDecay:) BOOL showPathDecayColor;
@property(getter=getDecayDelay,setter=setDecayDelay:) float decayDelay;
@property(readwrite) ViewController* controller;
@property(getter=getApplyRot,setter=setApplyRot:) BOOL applyRotation;
@end
