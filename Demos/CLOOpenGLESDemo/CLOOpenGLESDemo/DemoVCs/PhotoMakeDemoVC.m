//
//  ViewController.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "PhotoMakeDemoVC.h"
#import "CLOOpenGLCtr.h"
#import "CLOOpenGLView.h"
#import "CLOOpenGLGlobal.h"

@interface PhotoMakeDemoVC ()

    @property (nonatomic,strong) CLOOpenGLCtr *mOpenGLCtr;
    @property (nonatomic,strong) CLOOpenGLView *mOpenGLView;

    @property (weak, nonatomic) IBOutlet UIImageView *mImgPreview;

@end

@implementation PhotoMakeDemoVC

- (CLOOpenGLCtr *)mOpenGLCtr
{
    if (!_mOpenGLCtr) {
        
        _mOpenGLCtr = [[CLOOpenGLCtr alloc] init];
    }
    
    return _mOpenGLCtr;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
//    CGFloat w = self.view.bounds.size.width;
//    CGFloat h = 4.0 * w / 3.0;
//    self.mOpenGLView.frame = CGRectMake(0, 0, w, h);
//    [self.view addSubview:self.mOpenGLView];
    UIImage *img = [UIImage imageNamed:@"Lambeau"];
    self.mImgPreview.image = img;
}

- (void)dealloc
{
    
}

- (IBAction)onStartMakeImage:(UIButton *)sender
{
    if (self.mImgPreview.image) {
        
        CLONSLog(@"开始做图 img: %@", self.mImgPreview.image);
        
        [self.mOpenGLCtr fSetupImage:self.mImgPreview.image withIndex:0];
        [self.mOpenGLCtr fSetupEffect:@"Effect=Normal"];
        [self.mOpenGLCtr fMake];
        UIImage *outImg = [self.mOpenGLCtr fGetMakedImage];
        
        CLONSLog(@"做图完成  img: %@", outImg);
    }
}


@end
