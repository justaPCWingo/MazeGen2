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

@interface MGOpenGLView : NSOpenGLView
{
    VisMgr* _visMgr;
}

-(void)newMaze:(MazeBuilder*)bldr;
-(void)refreshWithMaze:(MazeBuilder*)bldr;
@end
