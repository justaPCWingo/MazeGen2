//
//  ViewController.m
//  MG2_Visualization
//
//  Created by Patrick Wingo on 7/16/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    _bldr=new MazeBuilder(8,8);
    
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
    }

- (void)viewWillAppear
{
    [_glView newMaze:_bldr];
}

-(IBAction)RefreshMaze:(id)sender
{
    _bldr->GenerateMaze();
    [_glView refreshWithMaze:_bldr];
}

-(void)dealloc
{
    delete _bldr;
}
@end
