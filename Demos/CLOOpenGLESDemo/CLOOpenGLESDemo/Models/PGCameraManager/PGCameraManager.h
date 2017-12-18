//
//  BIZCameraManager.m
//  Camera360
//
//  Created by pengpingguo plobol525@hotmail.com on 14-11-1.
//  Copyright (c) 2013年 Pinguo. All rights reserved.
//

/**
 *  by plobol525@hotmail.com 2013-11-11
 */
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "PGCameraDefine.h"
#import "PGCameraManagerDelegate.h"


#define DefaultCamera       [PGCameraManager sharedCameraManager]
#define kPGCameraManager       DefaultCamera

@class PGAVCamPreviewView;
@class PGCameraManager;
@protocol PGCameraManagerDelegate;

@interface PGCameraManager : NSObject

@property (nonatomic, assign) PGCameraPositon defaultCameraPosiont;
@property (nonatomic, weak) id<PGCameraManagerDelegate> delegate;
@property (nonatomic, assign) BOOL isRunning;
@property (nonatomic, strong) dispatch_queue_t videoOutputQueue;
/**
 *  单例
 *
 *  @return 实例
 */
+ (instancetype)sharedCameraManager;

#pragma mark -
#pragma mark 初始化相机参数

/**
 *  初始化相机
 *
 *  @return self
 */
- (instancetype)init;

/**
 *  初始化相机预览
 *
 */
- (void)initForPreview;

/**
 *  初始化相机SDK渲染
 *
 */
- (void)initForRender;

/**
 *  初始化相机拍照
 *
 */
- (void)initForCaputre;

- (void)pSetupNewSDK;

#pragma mark - 相机控制
/**
 *  启动相机
 *
 *  @return 是否启动成功
 */
- (BOOL)start;

/**
 *  停止相机
 *
 *  @return 是否停止成功
 */
- (BOOL)stop;

#pragma mark -
#pragma mark 相机视频预览控制

/**
 *  关闭视频帧
 */
- (void)stopVideo;

/**
 *  开启视频帧
 */
- (void)startVideo;


#pragma mark -
#pragma mark 相机拍照方面
/**
 *  拍照 通过回调：
 - (void)cameraManager:(BIZCameraManager *)cameraManager withCaputreImage:(NSData *)sourceImageData
 sourceImageData 返回实际的拍照数据
 */
- (void)captureImageIsNeedEtag:(BOOL)isNeedEtag;


#pragma mark -
#pragma mark 相机参数
/**
 *  是否支持闪光灯
 *
 *  @return 结果
 */
- (BOOL)hasFlash;

/**
 *  是否支持手电筒
 *
 *  @return 结果
 */
- (BOOL)hasTorch;

/**
 *  是不中支持聚焦
 *
 *  @return 结果
 */
- (BOOL)hasFocus;

/**
 *  是否支持曝光
 *
 *  @return 结果
 */
- (BOOL)hasExposure;

/**
 *  是否支持白平衡
 *
 *  @return 结果
 */
- (BOOL)hasWhiteBalance;

/**
 *  是否支持镜像
 *
 *  @return 结果
 */
- (BOOL)supportsMirroring;

- (void)setSupportsMirroring:(BOOL)isMirroring;

/**
 *  是否支持切换摄像头
 *
 *  @return 结果
 */
- (BOOL)supportsToggleCamera;

/**
 *  获取焦距最大压缩率
 *
 *  @return 结果
 */
- (CGFloat)getCameraMaxScaleAndCropFactor;

/**
 *  检查相机权限
 *
 *  @return false if no permission
 */
- (BOOL)hasAuthorization;

#pragma mark -
#pragma mark 设置相机相关参数
/**
 *  设置分辩率
 *
 *  @param preset 系统定义的分辩率
 *  @param error  错误输出
 *
 *  @return 设置是否成功
 */
- (BOOL)setResolution:(NSString *)preset;

/**
 *  得到分辩率
 *
 *  @return 系统定义的分辩率
 */
- (NSString *)getResolution;

/**
 *  是否有指定相机
 *
 *  @param position BIZCameraPosition 枚举
 *
 *  @return 结果
 */
- (BOOL)hasCamera:(PGCameraPositon)cameraPosition;

/**
 *  设置相机位置
 *
 *  @param position BIZCameraPosition 枚举
 *
 *  @return 结果
 */
- (BOOL)setCameraPosition:(PGCameraPositon)position;

/**
 *  得到摄像头位置
 *
 *  @return 是前还是后
 */
- (PGCameraPositon)getCameraPosition;

/**
 *  设置帧率
 *
 *  @param minFrame 最小帧
 *  @param maxFrame 最大帧
 *
 *  @return 是否成功
 */
- (BOOL)setFrameRate:(NSInteger)minFrame
                 max:(NSInteger)maxFrame;


/**
 *  设置聚焦
 *
 *  @param point 位置 0----1
 *
 *  @return 结果
 */
- (BOOL)setFocusAtPoint:(CGPoint)point;

/**
 *  设置爆光
 *
 *  @param point 位置 0-----1
 */
- (BOOL)setExposureAtPoint:(CGPoint)point;


/**
 *  设置爆光类型
 *
 *  @param mode BIZCameraExposureMode 枚举
 *
 *  @return 结果
 */
- (BOOL)setExposureMode:(PGCameraExposureMode)mode;

/**
 *  得到爆光类型
 *
 *  @return BIZCameraExposureMode 枚举
 */
- (PGCameraExposureMode)getExposureMode;

/**
 *  设置聚焦类型
 *
 *  @param mode BIZCameraFocusMode 枚举
 *
 *  @return 结果
 */
- (BOOL)setFocusMode:(PGCameraFocusMode)mode;

/**
 *  得到聚焦类型
 *
 *  @return BIZCameraFocusMode 枚举
 */
- (PGCameraFocusMode)getFocusMode;

/**
 *  设置闪光灯类型
 *
 *  @param mode BIZCameraFlashMode 枚举
 *
 *  @return 结果
 */
- (BOOL)setFlashMode:(PGCameraFlashMode)mode;

/**
 *  得到闪光灯类型
 *
 *  @return BIZCameraFlashMode 枚举
 */
- (PGCameraFlashMode)getFlashMode;

/**
 *  设置手电筒类型
 *
 *  @param mode BIZCameraTorchMode
 *
 *  @return 结果
 */
- (BOOL)setTorchMode:(PGCameraTorchMode)mode;

/**
 *  得到手电筒类型
 *
 *  @return BIZCameraTorchMode 枚举
 */
- (PGCameraTorchMode)getTorchMode;



#pragma mark -
#pragma mark 其他功能参数
/**
 *  转换坐标
 *
 *  @param viewCoordinates 视图坐标
 *
 *  @return 0-----1
 */
- (CGPoint)convertToPointOfInterestFromViewCoordinates:(CGPoint)viewCoordinates
                                         touchViewSize:(CGSize)size;

@end
