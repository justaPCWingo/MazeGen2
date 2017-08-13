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
    _bldr=new MazeBuilder(16,16);
    
    
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
    try
    {
        _bldr->GenerateMaze();
        [_glView refreshWithMaze:_bldr];
    }
    catch(std::runtime_error & e)
    {
        VisMgr::DbgDumpMaze(_bldr);
        std::cout<<"Start: "<<_bldr->GetStart()<<std::endl;
        std::cout<<"Finish: "<<_bldr->GetFinish()<<std::endl;
        throw e;
    }
}

-(void)dealloc
{
    delete _bldr;
}
@end
