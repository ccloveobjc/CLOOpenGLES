//
//  CLOOpenGLImageUtility.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/8/21.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLImageUtility.h"

@implementation CLOOpenGLImageUtility

+ (unsigned char *)sCreateRGBA:(UIImage *)img withOutW:(uint32_t *)outW withOutH:(uint32_t *)outH withPremultiplied:(BOOL)isPremultiplied
{
    CGImageRef imgRef = img.CGImage;
    
    size_t pixWidth, pixHeight;
    pixWidth = CGImageGetWidth(imgRef);
    pixHeight = CGImageGetHeight(imgRef);
    NSAssert(pixWidth > 0 && pixHeight > 0, @"");
    
    *outW = (uint32_t)pixWidth;
    *outH = (uint32_t)pixHeight;
    
    size_t bytesPerPixel = 4;
    size_t bytesPerRow = bytesPerPixel * pixWidth;
    
    NSUInteger bitsPerComponent = CGImageGetBitsPerComponent(imgRef);
    //    int info = CGImageGetBitmapInfo(imgRef);
    unsigned char *orgPixel = new unsigned char[bytesPerRow * pixHeight];
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    {
        CGContextRef context = CGBitmapContextCreate(orgPixel, pixWidth, pixHeight,
                                                     bitsPerComponent, bytesPerRow, colorSpace,
                                                     kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        {
            CGContextDrawImage(context, CGRectMake(0, 0, pixWidth, pixHeight), imgRef);
//             预乘了alpha，需要反解
            if (!isPremultiplied) {
                
                for(int i = 0 ; i < pixWidth * pixHeight * 4 ; i += 4) {
                    
                    unsigned char alpha = orgPixel[i+3];
                    if(alpha != 0) {
                        
                        float fAlpha = 255.0/(float)alpha;
                        orgPixel[i+0] *= fAlpha;
                        orgPixel[i+1] *= fAlpha;
                        orgPixel[i+2] *= fAlpha;
                        
                        if (orgPixel[i+0] > 255) { orgPixel[i+0] = 255; }
                        if (orgPixel[i+1] > 255) { orgPixel[i+1] = 255; }
                        if (orgPixel[i+2] > 255) { orgPixel[i+2] = 255; }
                    }
                }
            }
        }
        CGContextRelease(context);
    }
    CGColorSpaceRelease(colorSpace);
    
    return orgPixel;
}

+ (UIImage *)sGetUIImage:(unsigned char *)pRGBA withW:(uint32_t)w withH:(uint32_t)h withPremultiplied:(BOOL)isPremultiplied
{
    UIImage *imgRet = nil;
    CGBitmapInfo bitmapInfo =  kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    {
        int rowLength = w * 4;
        if (!isPremultiplied) {
         
            for(int i = 0 ; i < w * h * 4 ; i += 4)
            {
                unsigned char alpha = pRGBA[i+3];
                if(alpha != 0)
                {
                    float fAlpha = 255.0/(float)alpha;
                    pRGBA[i+0] = pRGBA[i+0] / fAlpha;
                    pRGBA[i+1] = pRGBA[i+1] / fAlpha;
                    pRGBA[i+2] = pRGBA[i+2] / fAlpha;
                    
                    if (pRGBA[i+0] > 255) { pRGBA[i+0] = 255; }
                    if (pRGBA[i+1] > 255) { pRGBA[i+1] = 255; }
                    if (pRGBA[i+2] > 255) { pRGBA[i+2] = 255; }
                }
            }
        }
        //RGBA
        CGContextRef context = CGBitmapContextCreate(pRGBA, w, h, 8, rowLength, space,bitmapInfo);
        {
            CGImageRef cgimg = CGBitmapContextCreateImage(context);
            {
                UIImageOrientation imgOrient = UIImageOrientationUp;
                imgRet = [[UIImage alloc]initWithCGImage:cgimg scale:1.0 orientation:imgOrient];
            }
            CGImageRelease(cgimg);
        }
        CGContextRelease(context);
    }
    CGColorSpaceRelease(space);
    
    return imgRet;
}

+ (UIImage *)sConvertBitmapRGBA8ToUIImage:(unsigned char *)buffer withWidth:(uint32_t)width withHeight:(uint32_t)height
{
    return [self.class sGetUIImage:buffer withW:width withH:height withPremultiplied:NO];
}

@end
