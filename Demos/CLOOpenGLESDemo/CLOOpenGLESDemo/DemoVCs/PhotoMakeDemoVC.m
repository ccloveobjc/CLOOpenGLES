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
#import "UICDisplayImageView.h"

@interface PhotoMakeDemoVC ()

    @property (nonatomic,strong) CLOOpenGLCtr *mOpenGLCtr;
    @property (nonatomic,strong) CLOOpenGLView *mOpenGLView;

    @property (weak, nonatomic) IBOutlet UICDisplayImageView *mImgPreview;

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
    [self.mImgPreview pSetupOrigImage:img];
}

- (void)dealloc
{
    
}

- (IBAction)onStartMakeImage:(UIButton *)sender
{
    UIImage *oriImg = self.mImgPreview.mOrigImage;
    if (oriImg) {
        
        CLONSLog(@"开始做图 img: %@", oriImg);
        
        [self.mOpenGLCtr fSetupImage:oriImg withIndex:0];
        [self.mOpenGLCtr fSetupEffect:@"Effect=Normal"];
        [self.mOpenGLCtr fMake];
        UIImage *outImg = [self.mOpenGLCtr fGetMakedImage];
        
        [self.mImgPreview pSetupPreviewImage:outImg];
        CLONSLog(@"做图完成  img: %@", outImg);
    }
}


@end
