//
//  ViewController.m
//  MG2_Visualization
//
//  Created by Patrick Wingo on 7/16/17.
//  Copyright © 2017 Wingo. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController
@synthesize glView=_glView;

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    _bldr=new MazeBuilder(16,16);
    
    [_glView addObserver:self forKeyPath:@"drawFullPath" options:0 context:nil];
    [_glView addObserver:self forKeyPath:@"showPathDecayColor" options:0 context:nil];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
    }

- (void)viewDidLayout
{
    [super viewDidLayout];
    //[_glView newMaze:_bldr];
    //update check boxes
    
    
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

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary<NSKeyValueChangeKey, id> *)change
                       context:(void *)context
{
    if([keyPath isEqualToString:@"drawFullPath"])
        [_fullPathCB setState:_glView.drawFullPath ? NSOnState : NSOffState];
    else if ([keyPath isEqualToString:@"showPathDecayColor"])
        [_decayCB setState:_glView.showPathDecayColor ? NSOnState : NSOffState];
    
}

-(IBAction)UpdateFromFullPathCB:(id)sender
{
    _glView.drawFullPath=[sender state]==NSOnState;
}

-(IBAction)UpdateFromDecayCB:(id)sender
{
    _glView.showPathDecayColor=[sender state]==NSOnState;
}

-(void)dealloc
{
    [_glView removeObserver:self forKeyPath:@"drawFullPath"];
    [_glView removeObserver:self forKeyPath:@"showPathDecayColor"];
    delete _bldr;
}
@end
