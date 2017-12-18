//
//  BIZCameraManager.m
//  Camera360
//
//  Created by pengpingguo plobol525@hotmail.com on 14-11-1.
//  Copyright (c) 2013年 Pinguo. All rights reserved.
//

#import "PGCameraManager.h"
#import "PGCameraManagerDelegate.h"
/**
 *  by plobol525@hotmail.com 2013-11-11
 */
struct
{
    unsigned int didResponseCaputreImage : 1;
    unsigned int didResponseCaputreImageEtag : 1;
    unsigned int didResponsePreviewImage : 1;
    unsigned int didResponseError : 1;
    unsigned int didResponseFocusState : 1;
    unsigned int didResponseExposureState : 1;
}_delegateFlag;

@interface PGCameraManager ()<AVCaptureVideoDataOutputSampleBufferDelegate>

@property (nonatomic, assign) AVCaptureVideoOrientation mVideoOrientation;
@property (nonatomic, strong) AVCaptureDeviceInput *mVideoInput;
@property (nonatomic, strong) AVCaptureDeviceInput *mAudioInput;
@property (nonatomic, strong) AVCaptureStillImageOutput *mStillImageOutput;
@property (nonatomic, strong) AVCaptureVideoDataOutput *mVideoDataOutput;
@property (nonatomic, strong) AVCaptureSession *mCameraCaptureSession;

@property (nonatomic, strong) id deviceConnectedObserver;
@property (nonatomic, strong) id deviceDisconnectedObserver;

@property (nonatomic, assign) BOOL videoMirrored;

@end


@implementation PGCameraManager

/**
 *  单例
 *
 *  @return 实例
 */
+ (instancetype)sharedCameraManager
{
    static PGCameraManager *shared = nil;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        shared = [[PGCameraManager alloc] init];
    });
    return shared;
}

#pragma mark - 相机控制
/**
 *  初始化相机参数
 *
 *  @return self
 */
- (instancetype)init
{
    self = [super init];
    if (self != nil)
    {
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(sessionRuntimeError:)
                                                     name:AVCaptureSessionRuntimeErrorNotification
                                                   object:nil];

        // 配置当前相机捕捉设备的会话状态
        _mCameraCaptureSession = [[AVCaptureSession alloc] init];

        [self setResolution:AVCaptureSessionPresetPhoto];//Cc   AVCaptureSessionPresetPhoto
                                                         //     AVCaptureSessionPreset1280x720
        _videoOutputQueue = dispatch_queue_create("com.vstudio.camera360.video.queue.context", DISPATCH_QUEUE_SERIAL);
        _defaultCameraPosiont = PGCameraPositonBack;

    }
    return self;
}

- (void)sessionRuntimeError:(NSNotification*)notification
{
    NSLog(@"SessionRuntimeError:%@",notification);
    self.isRunning = _mCameraCaptureSession.running;
}

- (void)initForPreview
{
    if(nil == self.mVideoInput)
    {
        NSError *error = nil;
        //获取当前相机捕捉设备
        AVCaptureDevice *cameraDevice = [self cameraWithPosition
                                         :(AVCaptureDevicePosition)self.defaultCameraPosiont];
        if (cameraDevice == nil
            && self.defaultCameraPosiont != AVCaptureDevicePositionFront)
        {
            //如果后置摄像头初始化失败，尝试用前置摄像头初始化
            cameraDevice = [self cameraWithPosition:AVCaptureDevicePositionFront];
        }
        //初始化摄像头设备输入对象
        self.mVideoInput = [AVCaptureDeviceInput deviceInputWithDevice:cameraDevice error:&error];
        if (error)
        {
            NSLog(@"init capture input fail, %@", error);
        }

        if ([_mCameraCaptureSession canAddInput:_mVideoInput])
        {
            [_mCameraCaptureSession addInput:_mVideoInput];
        }

        [self addObserver];
        if ([self.delegate respondsToSelector:@selector(isSystemPreview)])
        {
            BOOL bRet = [self.delegate isSystemPreview];
            if (bRet)
            {
                AVCaptureVideoPreviewLayer *_previeLayer = [[AVCaptureVideoPreviewLayer alloc]
                                                            initWithSession:_mCameraCaptureSession];
                if (_previeLayer)
                {
                    UIView *prevView = [self.delegate cameraPrevView];
                    prevView.frame = prevView.bounds;
                    _previeLayer.frame = prevView.bounds;
                    [prevView.layer addSublayer:_previeLayer];
                }
            }

        }

    }
}

- (void)initForRender
{
    if(nil == self.mVideoDataOutput)
    {
        //2.
        self.mVideoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
        //总是丢弃前面的帧
        self.mVideoDataOutput.alwaysDiscardsLateVideoFrames = YES;
        [self.mVideoDataOutput setSampleBufferDelegate:self queue:self.videoOutputQueue];

        //图像设置
        NSString *key = (NSString *)kCVPixelBufferPixelFormatTypeKey;
        NSNumber *value = @(kCVPixelFormatType_32BGRA);
        NSDictionary *_videoSettings = @{key : value};
        [self.mVideoDataOutput setVideoSettings:_videoSettings];


        if ([_mCameraCaptureSession canAddOutput:_mVideoDataOutput])
        {
            [_mCameraCaptureSession addOutput:_mVideoDataOutput];
        }
    }
}

- (void)pSetupNewSDK
{
    AVCaptureConnection *pVideoConn = [self.mVideoDataOutput connectionWithMediaType:AVMediaTypeVideo];
    if (pVideoConn && [pVideoConn isVideoOrientationSupported])
    {
        [pVideoConn setVideoOrientation:AVCaptureVideoOrientationPortrait];
    }
}

- (void)initForCaputre
{
    if(nil == self.mStillImageOutput)
    {
        //3.
        //初始化静态数据输出
        self.mStillImageOutput = [[AVCaptureStillImageOutput alloc] init];
        NSDictionary *outputSettings = @{AVVideoCodecKey : AVVideoCodecJPEG};
        [self.mStillImageOutput setOutputSettings:outputSettings];

        // 加入静态图像输出设备
        if ([_mCameraCaptureSession canAddOutput:_mStillImageOutput])
        {
            [_mCameraCaptureSession addOutput:_mStillImageOutput];
        }
    }
}

- (void)setMDefaultCameraPosiont:(PGCameraPositon)defaultCameraPosiont
{
    defaultCameraPosiont = defaultCameraPosiont;
    if(self.mVideoInput)
    {
        [self setCameraPosition:defaultCameraPosiont];
    }
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:AVCaptureSessionRuntimeErrorNotification
                                                  object:nil];
    @try
    {
        [self removeObserver:self forKeyPath:@"mVideoInput.device.adjustingExposure"];
        [self removeObserver:self forKeyPath:@"mVideoInput.device.adjustingExposure"];
        [self removeObserver:self forKeyPath:AVCaptureSessionRuntimeErrorNotification];
    }
    @catch (NSException *exception)
    {

        NSLog(@"Exception occurred: %@, %@", exception, [exception userInfo]);
    }
}


/**
 *  查看是否拥有相机权限
 *
 *  @return 结果
 */
- (BOOL)hasAuthorization
{
    __block BOOL isAvalible = YES;
    if ([AVCaptureDevice respondsToSelector:@selector(authorizationStatusForMediaType:)])
    {
        isAvalible = NO;

        NSString *mediaType = AVMediaTypeVideo; // Or AVMediaTypeAudio

        AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:mediaType];
        
        // This status is normally not visible—the AVCaptureDevice class methods
        // for discovering devices do not return devices the user is restricted from accessing.
        if (authStatus == AVAuthorizationStatusRestricted)
        {
            //?
            NSLog(@"AVMediaTypeVideo auth Restricted");
        }

                // The user has explicitly denied permission for media capture.
        else if (authStatus == AVAuthorizationStatusDenied)
        {
            //?
            NSLog(@"AVMediaTypeVideo auth Deny");
        }

                // The user has explicitly granted permission for media capture,
                // or explicit user permission is not necessary for the media type in question.
        else if (authStatus == AVAuthorizationStatusAuthorized)
        {
            NSLog(@"AVMediaTypeVideo auth OK");

            isAvalible = YES;
        }

                // Explicit user permission is required for media capture,
                // but the user has not yet granted or denied such permission.
        else if (authStatus == AVAuthorizationStatusNotDetermined)
        {
            [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted) {
                if (granted)
                {
                    NSLog(@"AVMediaTypeVideo auth OK");
                    // PGLogDebug(@"Granted access to %@", mediaType);
                }
                else
                {
                    NSLog(@"AVMediaTypeVideo auth not granted");
                    // PGLogDebug(@"Not granted access to %@", mediaType);
                }
            }];
        }
        else
        {
            //PGLogDebug(@"Unknown authorization status");
        }
    }
    return isAvalible;
}


/**
 *  设置回调的代码
 *
 *  @param delegate 相机管理代理
 */
- (void)setDelegate:(id<PGCameraManagerDelegate>)delegate
{
    if(_delegate != delegate)
    {
        _delegate = delegate;
        _delegateFlag.didResponseCaputreImageEtag = _delegate
                                                            && [_delegate respondsToSelector:
                                                                @selector(cameraManager:withCaputreImageEtag:)];
        _delegateFlag.didResponseCaputreImage = _delegate && [_delegate respondsToSelector:
                                                              @selector(cameraManager:captureData:)];
        _delegateFlag.didResponsePreviewImage = _delegate && [_delegate respondsToSelector:
                                                              @selector(cameraManager:withPreviewImage:)];
        _delegateFlag.didResponseError = _delegate && [_delegate respondsToSelector:
                                                       @selector(cameraManager:withError:)];
        _delegateFlag.didResponseFocusState = _delegate && [_delegate respondsToSelector:
                                                            @selector(cameraManager:withFocusState:)];
        _delegateFlag.didResponseExposureState = _delegate && [_delegate respondsToSelector:
                                                               @selector(cameraManager:withExposureState:)];
    }
}


/**
 *  启动相机
 *
 *  @return 是否启动成功
 */
- (BOOL)start
{
    BOOL bRet = NO;

    if (self.mCameraCaptureSession)
    {
        if (![self.mCameraCaptureSession isRunning])
        {
            [self.mCameraCaptureSession startRunning];
            self.isRunning = self.mCameraCaptureSession.running;
        }

        bRet = YES;
    }
    return bRet;
}

/**
 *  停止相机
 *
 *  @return 是否停止成功
 */
- (BOOL)stop
{
    BOOL bRet = NO;
    if (self.mCameraCaptureSession)
    {
        if ([self.mCameraCaptureSession isRunning])
        {
            [self.mCameraCaptureSession stopRunning];
            self.isRunning = NO;
        }
        bRet = YES;
    }
    return bRet;
}


/**
 *  关闭视频帧
 */
- (void)stopVideo
{
    if (self.mVideoDataOutput.sampleBufferDelegate)
    {
        [self.mVideoDataOutput setSampleBufferDelegate:nil queue:self.videoOutputQueue];
    }
}


/**
 *  开启视频帧
 */
- (void)startVideo
{
    if (!self.mVideoDataOutput.sampleBufferDelegate)
    {
        [self.mVideoDataOutput setSampleBufferDelegate:self queue:self.videoOutputQueue];
    }
}

/**
 *  拍照
 */
- (void)captureImageIsNeedEtag:(BOOL)isNeedEtag
{
    // REV @hc method line > 100

    NSArray *connections = [[self mStillImageOutput] connections];
    AVCaptureConnection *videoConn = [PGCameraManager connectionWithMediaType:AVMediaTypeVideo fromConnections:connections];

    if (videoConn == nil)
    {
        return;
    }

    if ([videoConn isVideoOrientationSupported])
    {
        AVCaptureVideoOrientation videoDeviceOrient = AVCaptureVideoOrientationLandscapeLeft;
        if ([self.delegate respondsToSelector:@selector(deviceOrientatiron)])
        {
            UIDeviceOrientation deviceOriention = [self.delegate deviceOrientatiron];
            if (deviceOriention == UIDeviceOrientationPortrait)
            {
                videoDeviceOrient = AVCaptureVideoOrientationPortrait;
            }
            else if (deviceOriention == UIDeviceOrientationLandscapeLeft)
            {
                videoDeviceOrient = AVCaptureVideoOrientationLandscapeRight;
            }
            else if (deviceOriention == UIDeviceOrientationLandscapeRight)
            {
                videoDeviceOrient = AVCaptureVideoOrientationLandscapeLeft;
            }
            else if (deviceOriention == UIDeviceOrientationPortraitUpsideDown)
            {
                videoDeviceOrient = AVCaptureVideoOrientationPortraitUpsideDown;
            }

        }
        [videoConn setVideoOrientation:videoDeviceOrient];
    }

//    dispatch_async(dispatch_get_main_queue(), ^{

        if([[videoConn inputPorts] count] > 0 && [videoConn output])
        {
            [[self mStillImageOutput] captureStillImageAsynchronouslyFromConnection:videoConn completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
                
                if (imageDataSampleBuffer && !error)
                {
                    NSData *tmpImageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];   
                    [self.delegate cameraManager:self captureData:tmpImageData];
                }
                else
                {
                    if (_delegateFlag.didResponseError)
                    {
                        [self.delegate cameraManager:self withError:error];
                    }
                }
            }];
        }
//    });
}



#pragma mark -功能支持
/**
 *  是否支持闪光灯
 *
 *  @return 结果
 */
- (BOOL)hasFlash
{
    return [[[self mVideoInput] device] hasFlash];
}


/**
 *  是否支持手电筒
 *
 *  @return 结果
 */
- (BOOL)hasTorch
{
    return [[[self mVideoInput] device] hasTorch];;
}


/**
 *  是不中支持聚焦
 *
 *  @return 结果
 */
- (BOOL)hasFocus
{
    AVCaptureDevice *_device = [[self mVideoInput] device];
    return [_device isFocusModeSupported:AVCaptureFocusModeLocked] ||
           [_device isFocusModeSupported:AVCaptureFocusModeAutoFocus] ||
           [_device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus];
}


/**
 *  是否支持曝光
 *
 *  @return 结果
 */
- (BOOL)hasExposure
{
    AVCaptureDevice *_device = [[self mVideoInput] device];
    return [_device isExposureModeSupported:AVCaptureExposureModeLocked] ||
           [_device isExposureModeSupported:AVCaptureExposureModeAutoExpose] ||
           [_device isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure];
}


/**
 *  是否支持白平衡
 *
 *  @return 结果
 */
- (BOOL)hasWhiteBalance
{
    AVCaptureDevice *device = [[self mVideoInput] device];
    return [device isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeLocked] ||
           [device isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeAutoWhiteBalance];
}


/**
 *  是否支持镜像
 *
 *  @return 结果
 */
- (BOOL)supportsMirroring
{
    AVCaptureConnection *pVideoConn = [self.mVideoDataOutput connectionWithMediaType:AVMediaTypeVideo];
    return (pVideoConn && [pVideoConn isVideoMirroringSupported]);
}

- (void)setSupportsMirroring:(BOOL)isMirroring
{
    AVCaptureConnection *pVideoConn = [self.mVideoDataOutput connectionWithMediaType:AVMediaTypeVideo];
    if (pVideoConn && [pVideoConn isVideoMirroringSupported] && pVideoConn.videoMirrored != isMirroring) {
        
        self.videoMirrored = isMirroring;
        [pVideoConn setVideoMirrored:self.videoMirrored];
    }
}

/**
 *  是否支持切换摄像头
 *
 *  @return 结果
 */
- (BOOL)supportsToggleCamera
{
    BOOL bRet = NO;
    if ([self getCameraCount] > 1)
    {
        bRet = YES;
    }
    return bRet;
}

#pragma mark -相机参数设置

//- (void)checkPermission
// {
//    if (![self hasAuthorization])
//    {
//        // 如果没有进行授权,则需要提示给外部调用接口
//        if (_delegateFlag.didResponseError)
//        {
//            NSError *theError = [NSError errorWithDomain:@"BIZCameraManager"
//                                                    code:AVErrorApplicationIsNotAuthorizedToUseDevice
//                                                userInfo:@{@"haveNoAuthorization" : @"error"}];
//            [self.delegate cameraManager:self withError:theError];
//        }
//    }
// }

/**
 *  设置分辩率
 *
 *  @param preset 系统定义的分辩率
 *  @param error  错误输出
 *
 *  @return 设置是否成功
 */
- (BOOL)setResolution:(NSString *)preset
{
    BOOL bRet = NO;
    if (![_mCameraCaptureSession.sessionPreset isEqualToString:preset]
        && [_mCameraCaptureSession canSetSessionPreset:preset])
    {
        [_mCameraCaptureSession beginConfiguration];
        [_mCameraCaptureSession setSessionPreset:preset];
        [_mCameraCaptureSession commitConfiguration];
    }

    NSLog(@"当前质量:%@", preset);
    return bRet;
}


/**
 *  得到分辩率
 *
 *  @return 系统定义的分辩率
 */
- (NSString *)getResolution
{
    return _mCameraCaptureSession.sessionPreset;
}


/**
 *  得到摄像头数目，用于判断是否可以进行摄像头的切换
 *
 *  @return 个数
 */
- (NSInteger)getCameraCount
{
    return [[AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo] count];
}


/**
 *  是否有指定相机
 *
 *  @param position BIZCameraPosition 枚举
 *
 *  @return 结果
 */
- (BOOL)hasCamera:(PGCameraPositon)cameraPosition
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == (AVCaptureDevicePosition)cameraPosition)
        {
            return YES;
        }
    }
    return NO;
}

/**
 *  设置相机位置
 *
 *  @param position BIZCameraPosition 枚举
 *
 *  @return 结果
 */
- (BOOL)setCameraPosition:(PGCameraPositon)position
{
    BOOL bRet = NO;

    if ([self supportsToggleCamera])
    {
        if ([self getCameraPosition] != position)
        {
            [self removeObserver];
            NSError *error = nil;
            AVCaptureSession *_session = self.mCameraCaptureSession;
            AVCaptureDeviceInput *newVideoInput = nil;

            if ((AVCaptureDevicePosition)position == AVCaptureDevicePositionBack)
            {
                AVCaptureDevice *const device = [self cameraWithPosition:AVCaptureDevicePositionBack];
                newVideoInput = [[AVCaptureDeviceInput alloc] initWithDevice:device
                                                                       error:&error];
            }
            else if ((AVCaptureDevicePosition)position == AVCaptureDevicePositionFront)
            {
                AVCaptureDevice *const device = [self cameraWithPosition:AVCaptureDevicePositionFront];
                newVideoInput = [[AVCaptureDeviceInput alloc] initWithDevice:device
                                                                       error:&error];
            }
            if (newVideoInput != nil)
            {
                [_session beginConfiguration];
                [_session removeInput:_mVideoInput];
                NSString *currentPreset = [_session sessionPreset];
                if (![[newVideoInput device] supportsAVCaptureSessionPreset:currentPreset])
                {
                    [_session setSessionPreset:currentPreset];
                }

                if ([_session canAddInput:newVideoInput])
                {
                    [_session addInput:newVideoInput];
                    _mVideoInput = newVideoInput;
                }
                else
                {
                    [_session setSessionPreset:currentPreset];
                    [_session addInput:_mVideoInput];
                }
                [_session commitConfiguration];
                bRet = YES;
            }
            else
            {
                if (error)
                {
                    NSLog(@"init AVCaptureDeviceInput fail, %@", error);
                }

            }

            [self addObserver];
        }


    }
    return bRet;
}


/**
 *  得到摄像头位置
 *
 *  @return 是前还是后
 */
- (PGCameraPositon)getCameraPosition
{
    return (PGCameraPositon)[[[self mVideoInput] device] position];
}


/**
 *  设置高频帧训练场
 *
 *  @param min 最小
 *  @param max 最大
 *
 *  @return 是否设置成功
 */

- (BOOL)setFrameRate:(NSInteger)minFrame max:(NSInteger)maxFrame
{
//    return YES;
    /// 设置帧率
//    if (![gBIZDeviceMgr isBeforeIPHONE4Device])
    {
        NSArray *connections = [[self mStillImageOutput] connections];
        AVCaptureConnection *videoConn = [PGCameraManager connectionWithMediaType:AVMediaTypeVideo
                                                                  fromConnections:connections];

        if ([videoConn respondsToSelector:@selector(isVideoMaxFrameDurationSupported)])
        {
            if ([videoConn isVideoMaxFrameDurationSupported])
            {
                if([[videoConn inputPorts] count] > 0 && [videoConn output])
                {
                    [videoConn setVideoMaxFrameDuration:CMTimeMake(1, (int32_t)minFrame)];
                    
                }
                else
                {
                    NSLog(@"videoConn error:%@",videoConn);
                }
            }

        }
//        else
//        {
//            if ([[[self videoInput] device] respondsToSelector:@selector(setActiveVideoMaxFrameDuration:)])
//            {
//                NSError* error;
//                if([[_videoInput device] lockForConfiguration:&error])
//                {
//                    [[[self videoInput] device] setActiveVideoMaxFrameDuration:CMTimeMake(1, (int32_t)minFrame)];
//                    [[_videoInput device] unlockForConfiguration];
//                }
//                else
//                {
//                    PGLogError(@"lockForConfiguration error:%@",error);
//                }
//
//            }
//        }

        if ([videoConn respondsToSelector:@selector(isVideoMinFrameDurationSupported)])
        {
            if ([videoConn isVideoMinFrameDurationSupported])
            {
                if([[videoConn inputPorts] count] > 0 && [videoConn output])
                {
                    [videoConn setVideoMinFrameDuration:CMTimeMake(1, (int32_t)maxFrame)];
                }
                else
                {
                    NSLog(@"videoConn error:%@",videoConn);
                }
            }
        }
//        else
//        {
//            if ([[[self videoInput] device] respondsToSelector:@selector(setActiveVideoMinFrameDuration:)])
//            {
//                NSError* error;
//                if([[_videoInput device] lockForConfiguration:&error])
//                {
//                    [[[self videoInput] device] setActiveVideoMinFrameDuration:CMTimeMake(1, (int32_t)maxFrame)];
//                    [[_videoInput device] unlockForConfiguration];
//                }
//                else
//                {
//                    PGLogError(@"lockForConfiguration error:%@",error);
//                }
//            }
//        }
    }
    return YES;
//    return YES;
}


/**
 *  设置帧率
 *
 *  @param minFrame 最小帧
 *  @param maxFrame 最大帧
 *
 *  @return 是否成功
 */
- (BOOL)setFocusAtPoint:(CGPoint)point
{
    if (point.x > 1.0)
    {
        point.x = 1.0;
    }

    if (point.y > 1.0)
    {
        point.y = 1.0;
    }


    BOOL bRet = NO;
    AVCaptureDevice *_device = [[self mVideoInput] device];
    if ([_device isFocusPointOfInterestSupported] &&
        [_device isFocusModeSupported:AVCaptureFocusModeAutoFocus])
    {
        if (![_device isAdjustingFocus])
        {
            NSError *error = nil;
            if ([_device lockForConfiguration:&error])
            {
                [_device setFocusPointOfInterest:point];
                [_device setFocusMode:AVCaptureFocusModeAutoFocus];
                [_device unlockForConfiguration];
                bRet = YES;
            }
            else
            {
                bRet = NO;
                if (error)
                {
                    NSLog(@"lockForConfiguration fail, %@", error);
                }
            }
        }
    }
    return bRet;
}


/**
 *  设置爆光
 *
 *  @param point 位置 0-----1
 */
- (BOOL)setExposureAtPoint:(CGPoint)point
{
    if (point.x > 1.0)
    {
        point.x = 1.0;
    }

    if (point.y > 1.0)
    {
        point.y = 1.0;
    }

    BOOL bRet = NO;
    AVCaptureDevice *_device = [[self mVideoInput] device];
    if ([_device isExposurePointOfInterestSupported]
        && [_device isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure])
    {
        NSError *error = nil;
        if ([_device lockForConfiguration:&error])
        {
            [_device setExposurePointOfInterest:point];
            [_device setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
            [_device unlockForConfiguration];
            bRet = YES;
        }
        else
        {
            bRet = NO;
            if (error)
            {
                NSLog(@"lockForConfiguration fail, %@", error);
            }
        }
    }
    return bRet;
}


/**
 *  转换坐标
 *
 *  @param viewCoordinates 视图坐标
 *
 *  @return 0-----1
 */
- (CGPoint)convertToPointOfInterestFromViewCoordinates:(CGPoint)viewCoordinates touchViewSize:(CGSize)size
{
    if ([self getCameraPosition] == PGCameraPositonFront)
    {
        viewCoordinates.x = size.width - viewCoordinates.x;
    }
    CGPoint pointOfInterest = CGPointMake(.5f, .5f);
    CGSize frameSize = size;
    CGRect cleanAperture;
    for (AVCaptureInputPort *port in [_mVideoInput ports])
    {
        if ([port mediaType] == AVMediaTypeVideo)
        {
            cleanAperture = CMVideoFormatDescriptionGetCleanAperture([port formatDescription], YES);
            CGSize apertureSize = cleanAperture.size;
            CGPoint point = viewCoordinates;
            CGFloat apertureRatio = apertureSize.height / apertureSize.width;
            CGFloat viewRatio = frameSize.width / frameSize.height;
            CGFloat xc = .5f;
            CGFloat yc = .5f;
            if (viewRatio > apertureRatio)
            {
                CGFloat y2 = apertureSize.width * (frameSize.width / apertureSize.height);
                xc = (point.y + ((y2 - frameSize.height) / 2.f)) / y2;
                yc = (frameSize.width - point.x) / frameSize.width;
            }
            else
            {
                CGFloat x2 = apertureSize.height * (frameSize.height / apertureSize.width);
                yc = 1.f - ((point.x + ((x2 - frameSize.width) / 2)) / x2);
                xc = point.y / frameSize.height;
            }
            pointOfInterest = CGPointMake(xc, yc);
            break;
        }
    }
    return pointOfInterest;
}


/**
 *  设置爆光类型
 *
 *  @param mode BIZCameraExposureMode 枚举
 *
 *  @return 结果
 */
- (BOOL)setExposureMode:(PGCameraExposureMode)mode
{
    AVCaptureDevice *_device = [[self mVideoInput] device];
    BOOL bRet = NO;
    if ([_device isExposureModeSupported:(AVCaptureExposureMode)mode] &&
        [_device exposureMode] != (AVCaptureExposureMode)mode)
    {
        NSError *error = nil;
        if ([_device lockForConfiguration:&error])
        {
            [_device setExposureMode:(AVCaptureExposureMode)mode];
            [_device unlockForConfiguration];
            bRet = NO;
        }
        else
        {
            bRet = YES;
            if (error)
            {
                NSLog(@"lockForConfiguration fail, %@", error);
            }
        }
    }
    return bRet;
}


/**
 *  得到爆光类型
 *
 *  @return BIZCameraExposureMode 枚举
 */
- (PGCameraExposureMode)getExposureMode
{
    return (PGCameraExposureMode)[[[self mVideoInput] device] exposureMode];
}


/**
 *  设置聚焦类型
 *
 *  @param mode BIZCameraFocusMode 枚举
 *
 *  @return 结果
 */
- (BOOL)setFocusMode:(PGCameraFocusMode)mode
{
    AVCaptureDevice *_device = [[self mVideoInput] device];
    BOOL bRet = NO;
    if ([_device isFocusModeSupported:(AVCaptureFocusMode)mode] &&
        [_device focusMode] != (AVCaptureFocusMode)mode)
    {
        NSError *error = nil;
        if ([_device lockForConfiguration:&error])
        {
            [_device setFocusMode:(AVCaptureFocusMode)mode];
            [_device unlockForConfiguration];
            bRet = YES;
        }
        else
        {
            bRet = NO;
            if (error)
            {
                NSLog(@"lockForConfiguration fail, %@", error);
            }
        }
    }
    return bRet;
}


/**
 *  得到聚焦类型
 *
 *  @return BIZCameraFocusMode 枚举
 */
- (PGCameraFocusMode)getFocusMode
{
    return (PGCameraFocusMode)[[[self mVideoInput] device] focusMode];
}


/**
 *  设置闪光灯类型
 *
 *  @param mode BIZCameraFlashMode 枚举
 *
 *  @return 结果
 */
- (BOOL)setFlashMode:(PGCameraFlashMode)mode
{
    AVCaptureDevice *_device = [[self mVideoInput] device];
    AVCaptureFlashMode flashMode = (AVCaptureFlashMode)mode;
    BOOL bRet = NO;
    if ([_device isFlashModeSupported:flashMode])
    {
        if (flashMode != [_device flashMode])
        {
            NSError *error = nil;
            if ([_device lockForConfiguration:&error])
            {
                [_device setFlashMode:flashMode];
                [_device unlockForConfiguration];
                bRet = YES;
            }
            else
            {
                bRet = NO;
                if (error)
                {
                    NSLog(@"lockForConfiguration fail, %@", error);
                }
            }
        }
    }
    return bRet;
}


/**
 *  得到闪光灯类型
 *
 *  @return BIZCameraFlashMode 枚举
 */
- (PGCameraFlashMode)getFlashMode
{
    return (PGCameraFlashMode)[[[self mVideoInput] device] flashMode];
}


/**
 *  设置手电筒类型
 *
 *  @param mode BIZCameraTorchMode
 *
 *  @return 结果
 */
- (BOOL)setTorchMode:(PGCameraTorchMode)mode
{
    AVCaptureDevice *_device = [[self mVideoInput] device];
    AVCaptureTorchMode torchMode = (AVCaptureTorchMode)mode;
    BOOL bRet = NO;
    if ([_device isTorchModeSupported:torchMode] && torchMode != [_device torchMode])
    {
        NSError *error = nil;
        if ([_device lockForConfiguration:&error])
        {
            [_device setTorchMode:(AVCaptureTorchMode)mode];
            [_device unlockForConfiguration];
            bRet = YES;
        }
        else
        {
            bRet = NO;
            if (error)
            {
                NSLog(@"lockForConfiguration fail, %@", error);
            }
        }
    }
    return bRet;
}

/**
 *  获取焦距最大压缩率
 *
 *  @return 结果
 */
- (CGFloat)getCameraMaxScaleAndCropFactor
{
    return [[self.mStillImageOutput connectionWithMediaType:AVMediaTypeVideo] videoMaxScaleAndCropFactor];
}


/**
 *  得到手电筒类型
 *
 *  @return BIZCameraTorchMode 枚举
 */
- (PGCameraTorchMode)getTorchMode
{
    return (PGCameraTorchMode)[[[self mVideoInput] device] torchMode];
}

#pragma mark 相机预览输出代理
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection
{
    if (_delegateFlag.didResponsePreviewImage)
    {
        [_delegate cameraManager:self withPreviewImage:sampleBuffer];
    }
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didDropSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
//    NSLog(@"Drop sampleBuffer ");
        CFTypeRef typeRef = CMGetAttachment(sampleBuffer, kCMSampleBufferAttachmentKey_DroppedFrameReason, NULL);
    
        CFStringRef myString = CFCopyDescription (typeRef);
    
        NSLog(@"Drop sampleBuffer  %@", (NSString *)CFBridgingRelease(myString));
}

#pragma mark 私有方法

- (AVCaptureDevice *)cameraWithPosition:(AVCaptureDevicePosition)position
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == position)
        {
            return device;
        }
    }
    return nil;
}


+ (AVCaptureConnection *)connectionWithMediaType:(NSString *)mediaType fromConnections:(NSArray *)connections
{
    for (AVCaptureConnection *connection in connections)
    {
        for (AVCaptureInputPort *port in [connection inputPorts])
        {
            if ([[port mediaType] isEqual:mediaType])
            {
                return connection;
            }
        }
    }
    return nil;
}

#pragma mark KVO事件监听回调方法
- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{

    if ([keyPath isEqualToString:@"self.mVideoInput.device.adjustingFocus"])
    {

        PGCameraFocusState state;
        if (self.mVideoInput.device.isAdjustingFocus)
        {
            state = PGCameraFocusStateAdjusting;
        }
        else
        {
            state = PGCameraFocusStateAdjusted;
        }

        if (_delegateFlag.didResponseFocusState)
        {
            
            [self.delegate cameraManager:self withFocusState:state];
        }

        if ([self.delegate respondsToSelector:@selector(autoFocusOrExposureHandleOfManager:)])
        {
            [self.delegate autoFocusOrExposureHandleOfManager:self];
        }
    }
    else if ([keyPath isEqualToString:@"self.mVideoInput.device.adjustingExposure"])
    {

        PGCameraExposureState state;
        if (self.mVideoInput.device.isAdjustingExposure)
        {
            state = PGCameraExposureStateAdjusting;
        }
        else
        {
            state = PGCameraExposureStateAdjusted;
        }

        if (_delegateFlag.didResponseExposureState)
        {
            [self.delegate cameraManager:self withExposureState:state];
        }
    }


}

#pragma mark  - 
#pragma mark 其他函数支持

- (void)addObserver
{
    [self addObserver:self
           forKeyPath:@"self.mVideoInput.device.adjustingFocus"
              options:NSKeyValueObservingOptionNew
              context:nil];
    [self addObserver:self
           forKeyPath:@"self.mVideoInput.device.adjustingExposure"
              options:NSKeyValueObservingOptionNew
              context:nil];
}

- (void)removeObserver
{
    [self removeObserver:self forKeyPath:@"self.mVideoInput.device.adjustingFocus"];
    [self removeObserver:self forKeyPath:@"self.mVideoInput.device.adjustingExposure"];
}

@end
