//
//  ViewController.h
//  MG2_Visualization
//
//  Created by Patrick Wingo on 7/16/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>
#import "MGOpenGLView.h"

//mazegen stuff
#import "MazeBuilder.h"

@interface ViewController : NSViewController
{
    IBOutlet MGOpenGLView* _glView;
    
    MazeBuilder* _bldr;
}
-(IBAction)RefreshMaze:(id)sender;

@end

