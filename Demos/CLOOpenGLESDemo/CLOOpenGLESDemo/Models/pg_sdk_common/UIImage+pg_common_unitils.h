//
//  UIImage+pg_unitils.h
//  pg_sdk_common
//
//  Created by Cc on 14/12/11.
//  Copyright (c) 2014年 PinguoSDK. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreMedia/CoreMedia.h>


    // C++
    double c_common_DegreesToRadians(double degrees);
    double c_common_RadiansToDegrees(double radians);

NS_ASSUME_NONNULL_BEGIN
@interface UIImage (pg_common_unitils)

// kCGInterpolationDefault 比较好的压缩
- (nonnull UIImage *)c_common_ResizedImage:(CGSize)newSize interpolationQuality:(CGInterpolationQuality)quality;

- (nonnull UIImage *)c_common_FixImageByOrientation;

- (nonnull UIImage *)c_common_Rotation:(UIImageOrientation)oOrientation;

- (nonnull UIImage *)c_common_GotScameImageWithMaxPixels:(long)maxPixel;

- (nonnull UIImage *)c_common_RotatedByDegrees:(CGFloat)degrees;


/**
 *  获取sdk最大支持像素
 *  如果参数 limitPixel ＝ 0 那么这个参数不起作用
 */
- (unsigned long)c_common_GotSDKMakeMaxPixelsWithLimit:(unsigned long)limitPixel;

/**
 *  获取至少大于size 的新size
 */
- (CGSize)c_common_GotMoreThanLimit:(CGSize)size;


    + (_Nullable CMSampleBufferRef)c_common_SampleBufferFromCGImage:(CGImageRef)image;

    + (_Nullable CGImageRef)c_common_CGImageFromSampleBuffer:(CMSampleBufferRef)buffer;

    + (_Nullable CGContextRef)c_common_CGContextFromSampleBuffer:(CMSampleBufferRef)sampleBuffer;


/**
 *  获取UIImage 对应的 char *
 */
- (nullable NSData *)c_common_GotBytesFromCGImageWithOutWidth:(NSUInteger *)widthPtr
                                       withOutHeight:(NSUInteger *)heightPtr;


    + (nullable UIImage *)c_common_ImageWithColor:(UIColor *)color
                                             size:(CGSize)size
                                     cornerRadius:(CGFloat)cornerRadius;


///**
// 获取路径的图片格式转成char *
//
// @param imgPath 路径
// @param sw 输出W
// @param sh 输出H
// @return char* 需要自行delete []
// */
//+ (unsigned char *_Nullable)c_common_GotImageData:(nonnull NSString *)imgPath withW:(int&)sw withH:(int&)sh;
//+ (BOOL)c_common_GotImageInData:(nonnull UIImage *)img withData:(unsigned char *_Nonnull)pData;

    + (_Nullable CMSampleBufferRef)c_common_DrawSampleBufferFacePoints:(CMSampleBufferRef)sampleBuffer withPoints:(NSArray<NSArray<NSValue *> *> *)faces;
    + (nullable UIImage *)c_common_DrawUIImageFacePoints:(UIImage *)oriImage withPoints:(NSArray<NSArray<NSValue *> *> *)faces;


/**
 图片上写字

 @param strText 文字
 @return 图片
 */
- (nullable UIImage *)c_common_WriteWords:(NSString *)strText;


/**
 获取buffer 的 RGBA 格式的Data

 @param sampleBuffer 预览帧
 @return NSData
 */
+ (void)c_common_GotBytesFromSampleBuffer:(CMSampleBufferRef)sampleBuffer withBlock:(void (^)(unsigned char *pData, int w, int h))block;


/**
 把dictionary 上的信息写到图片上

 @param params 支持 NSNumber ,NSArry , NSString
 @return 图片
 */
+ (UIImage *)c_common_CreateImageByDictionary:(NSDictionary *)params;


/**
 裁剪图片

 @param rect 位置
 @return 新图
 */
- (UIImage *)c_common_CropImage:(CGRect)rect;

/**
 等比缩放

 @param size 最大
 @return 图
 */
- (UIImage *)c_common_EqualRatioToSize:(CGSize)size withUsingSizeDrawContext:(BOOL)isUsing;
@end
NS_ASSUME_NONNULL_END
