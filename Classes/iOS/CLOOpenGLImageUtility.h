//
//  CLOOpenGLImageUtility.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/8/21.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface CLOOpenGLImageUtility : NSObject

/**
 获取 UIImage 的 RGBA 数据，解决了预乘
 如果返回不为空，需要 delete[]

 @param img 图片
 @param outW 宽
 @param outH 高
 @param isPremultiplied 是否开启预乘
 @return 成功返回数据
 */
+ (unsigned char *)sCreateRGBA:(UIImage *)img withOutW:(uint32_t *)outW withOutH:(uint32_t *)outH withPremultiplied:(BOOL)isPremultiplied;

/**
 获取 UIImage

 @param pRGBA rgba 数据
 @param w 宽
 @param h 高
 @param isPremultiplied 是否有预乘
 @return 成功返回对象
 */
+ (UIImage *)sGetUIImage:(unsigned char *)pRGBA withW:(uint32_t)w withH:(uint32_t)h withPremultiplied:(BOOL)isPremultiplied;

//+ (void)sText:(UIImage *)img;

/**
 RGBA to UIImage
 
 @param buffer RGBA
 @param width 宽
 @param height 高
 @return 成功=UIImage
 */
+ (UIImage *)sConvertBitmapRGBA8ToUIImage:(unsigned char *)buffer withWidth:(uint32_t)width withHeight:(uint32_t)height;

@end
