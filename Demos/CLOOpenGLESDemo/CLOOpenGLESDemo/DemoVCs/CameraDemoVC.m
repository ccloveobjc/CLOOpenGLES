//
//  CameraDemoVC.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/18.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CameraDemoVC.h"
#import "PGCameraManager.h"
#import "CLOOpenGLCtr.h"
#import "CLOOpenGLView.h"
#import "CLOOpenGLGlobal.h"

@interface CameraDemoVC ()
<
    PGCameraManagerDelegate
>

    @property (weak, nonatomic) IBOutlet UIView *mPreviewView;
    @property (strong, nonatomic) CLOOpenGLView *mGLView;

@end

@implementation CameraDemoVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self initCameraDevice];
    
    [self initGLView];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [DefaultCamera start];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [DefaultCamera stop];
}

- (IBAction)onChangedEffectClick:(UIButton *)sender
{
    
}

#pragma mark - GLView
- (void)initGLView
{
    CGFloat w = self.mPreviewView.bounds.size.width;
    CGFloat h = w / ( 3.0 / 4.0);
    CGFloat y = (self.mPreviewView.bounds.size.height - h) / 2.0;
    CGRect pFrame = CGRectMake(0, y, w, h);
    CLOOpenGLCtr *glCtr = [[CLOOpenGLCtr alloc] init];
    self.mGLView = [[CLOOpenGLView alloc] initWithFrame:pFrame withGLCtr:glCtr];
    [self.view addSubview:self.mGLView];
}

#pragma mark - 渲染 buffer
- (void)initCameraDevice
{
    DefaultCamera.delegate = self;
    [DefaultCamera initForPreview];
    [DefaultCamera initForCaputre];
    [DefaultCamera initForRender];
    [DefaultCamera pSetupNewSDK];
    
    [DefaultCamera setResolution:AVCaptureSessionPresetPhoto];
    
    [DefaultCamera setCameraPosition:PGCameraPositonBack];
    [DefaultCamera setSupportsMirroring:NO];
}
- (void)cameraManager:(PGCameraManager *)cameraManager withPreviewImage:(CMSampleBufferRef)sampleBuffer
{
    if ( ! [self.mGLView fRenderBuffer:sampleBuffer]) {
        
        CLONSLog(@"renderBuffer 错误 !");
    }
}

- (void)cameraManager:(PGCameraManager *)cameraManager captureData:(NSData *)sourceImageData
{
    
}
- (UIView *)cameraPrevView { return nil; }
- (BOOL)isSystemPreview { return NO; }
- (void)cameraManager:(PGCameraManager *)cameraManager withCaputreImageEtag:(NSString *)etag {}
- (void)cameraManager:(PGCameraManager *)cameraManager withError:(NSError *)error {}

@end
