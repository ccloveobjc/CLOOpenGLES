//
//  ViewController.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "ViewController.h"
#import "CLOOpenGLCtr.h"
#import "CLOOpenGLView.h"


@interface ViewController ()

    @property (nonatomic,strong) CLOOpenGLCtr *mOpenGLCtr;
    @property (nonatomic,strong) CLOOpenGLView *mOpenGLView;

@end

@implementation ViewController

- (CLOOpenGLCtr *)mOpenGLCtr
{
    if (!_mOpenGLCtr) {
        
        _mOpenGLCtr = [[CLOOpenGLCtr alloc] init];
    }
    
    return _mOpenGLCtr;
}

- (CLOOpenGLView *)mOpenGLView
{
    if (!_mOpenGLView) {
        
        _mOpenGLView = [[CLOOpenGLView alloc] initWithFrame:CGRectZero withGLCtr:self.mOpenGLCtr];
    }
    
    return _mOpenGLView;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    CGFloat w = self.view.bounds.size.width;
    CGFloat h = 4.0 * w / 3.0;
    self.mOpenGLView.frame = CGRectMake(0, 0, w, h);
    [self.view addSubview:self.mOpenGLView];
}

- (void)dealloc
{
    
}

@end
