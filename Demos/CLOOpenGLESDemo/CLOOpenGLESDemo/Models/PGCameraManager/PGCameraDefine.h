//
//  PGCameraDefine.h
//  PGCameraDemo
//
//  Created by penpingguo on 14-11-1.
//  Copyright (c) 2014年 penpingguo. All rights reserved.
//

#import <Foundation/Foundation.h>


/*
 * 相机聚焦的状态 表示是否完成
 */
typedef NS_ENUM(NSInteger, PGCameraFocusState)
{
    PGCameraFocusStateAdjusting = 0,
    PGCameraFocusStateAdjusted
};

/**
 *   表示相机的测光状态
 */
typedef NS_ENUM(NSInteger, PGCameraExposureState)
{
    PGCameraExposureStateAdjusting = 0,
    PGCameraExposureStateAdjusted
};

/**
 *  表示相机的位置 枚举
 */
typedef NS_ENUM(NSInteger, PGCameraPositon)
{
    PGCameraPositonBack = 1,
    PGCameraPositonFront = 2
};

/**
 *   表示相机的测光模式
 */
typedef NS_ENUM(NSInteger, PGCameraExposureMode)
{
    PGCameraExposureModeLocked = 0,
    PGCameraExposureModeAuto ,
    PGCameraExposureModeContinueAndAuto
};


/**
 *  表示相机聚焦模式
 */
typedef NS_ENUM(NSInteger, PGCameraFocusMode)
{
    PGCameraFocusModeLocked = 0,
    PGCameraFocusModeAutoFocus,
    PGCameraFocusModeContinueAndAutoFocus
};

/*
 * 表示相机的闪光灯状态
 */
typedef NS_ENUM(NSInteger, PGCameraFlashMode)
{
    PGCameraFlashModeOff = 0,
    PGCameraFlashModeOn,
    PGCameraFlashModeAuto
};

/*
 * 表示相机灯光模式
 */
typedef NS_ENUM(NSInteger, PGCameraTorchMode)
{
    PGCameraTorchModeOff = 0,
    PGCameraTorchModeOn,
    PGCameraTorchModeAuto
};

/*
 * 相机的状态 
 */
typedef NS_ENUM(NSInteger, PGCameraInitState)
{
    PGCameraInitStateUnknow = 0,
    PGCameraInitStateRunningOk,
    PGCameraInitStatePreLayerOk,
    PGCameraInitStateRenderOk,
    PGCameraInitStateCaptureOk
};

typedef NS_ENUM(NSInteger, PGCameraProcessState)
{
    PGCameraProcessStateInitParams = 0,
    PGCameraProcessStateInitPreview,
    PGCameraProcessStateInitRender,
    PGCameraProcessStateInitCaptureDevice,
};



@interface PGCameraDefine : NSObject




@end
