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
    IBOutlet NSColorWell* _gridWell;
    IBOutlet NSColorWell* _pathWell;
    IBOutlet NSColorWell* _wallWell;
    IBOutlet NSColorWell* _decayWell;
    IBOutlet NSButton* _fullPathCB;
    IBOutlet NSButton* _decayCB;
    IBOutlet NSButton* _rotateCB;
    
    MazeBuilder* _bldr;
}
-(IBAction)RefreshMaze:(id)sender;
-(IBAction)UpdateFromFullPathCB:(id)sender;
-(IBAction)UpdateFromDecayCB:(id)sender;
-(IBAction)UpdateFromRotateCB:(id)sender;

@property(readonly) MGOpenGLView* glView;
@end

