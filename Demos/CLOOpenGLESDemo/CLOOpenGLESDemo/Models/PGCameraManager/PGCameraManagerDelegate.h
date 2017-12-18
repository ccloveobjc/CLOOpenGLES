//
//  PGCameraManagerDelegate.h
//  PGCameraDemo
//
//  Created by penpingguo on 14-11-1.
//  Copyright (c) 2014年 penpingguo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreMedia/CoreMedia.h>
#import "PGCameraDefine.h"

@class NSObject;
@class PGCameraManager;

@protocol PGCameraManagerDelegate<NSObject>

#pragma mark 相机视频输出
@required

/**
 *  相机预览图片输出
 *
 *  @param cameraManager 相机管理对象
 *  @param sampleBuffer  相机预览采样数据
 */
- (void)cameraManager:(PGCameraManager *)cameraManager
     withPreviewImage:(CMSampleBufferRef)sampleBuffer;

/**
 *  拍照图片的数据
 *
 *  @param cameraManager   相机管理对象
 *  @param sourceImageData 拍照后的数据
 */
- (void)cameraManager:(PGCameraManager *)cameraManager
          captureData:(NSData *)sourceImageData;


/*
 * 获取需要渲染的视图
 */
- (UIView *)cameraPrevView;

/*
 * 是否是系统渲染
 */
- (BOOL)isSystemPreview;


/**
 *  拍照图片的数据Etag
 *
 *  @param cameraManager 相机管理对象
 *  @param etag          拍照后的图片cache对应的key
 */
- (void)cameraManager:(PGCameraManager *)cameraManager
 withCaputreImageEtag:(NSString *)etag;

/**
 *  如果发生错误，此回调被调用
 *
 *  @param cameraManager 自身
 *  @param error         错误输出
 */
- (void)cameraManager:(PGCameraManager *)cameraManager
            withError:(NSError *)error;


@optional

/*
 * 开始初始化拍摄参数的处理过程
 */
- (void)shouldStartInitCameraParams;

/*
 * 加载完成后拍摄的参数处理过程
 */
- (void)didStartInitCameraParams;

/*
 * 获取当前的设备方向
 */
- (UIDeviceOrientation)deviceOrientatiron;


/**
 *  聚焦状态变化时回调
 */
- (void)cameraManager:(PGCameraManager *)cameraManager
       withFocusState:(PGCameraFocusState)focusState;

/**
 *  曝光状态变化时回调
 */
- (void)cameraManager:(PGCameraManager *)cameraManager
    withExposureState:(PGCameraExposureState)exposureState;

/*
 * 处理自定聚焦和测光的问题
 */
- (void)autoFocusOrExposureHandleOfManager:(PGCameraManager *)manager;
@end

