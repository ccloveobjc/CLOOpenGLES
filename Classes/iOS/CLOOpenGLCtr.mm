//
//  CLOOpenGLCtr.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLCtr.h"
#import "CLOOpenGLContext.h"
#import "CLOOpenGLGlobal.h"
#import "CLOOpenGLImageUtility.h"

#include "CLOglCtr.hpp"
#include "CLOglFilter.hpp"

@interface CLOOpenGLCtr()

    @property (nonatomic,strong) CLOOpenGLContext *mGLContext;
    @property (nonatomic,assign) CLO::CLOglCtr *m_glCtr;

@end
@implementation CLOOpenGLCtr

- (instancetype)init
{
    self = [super init];
    if (self) {
        
        _mGLContext = [[CLOOpenGLContext alloc] init];
        _m_glCtr = new CLO::CLOglCtr();
        
        CLONSLog(@"CLOOpenGLCtr 初始化完成");
    }
    
    return self;
}

- (void)dealloc
{
    CLOCClassRelease(_m_glCtr);
    CLONSLog(@"CLOOpenGLCtr dealloc");
}

- (BOOL)fSetupImage:(UIImage *)img withIndex:(NSUInteger)index
{
    __block BOOL bRet = NO;
    if (img.size.width > 0 && img.size.height > 0) {
    
        uint32_t imgW = 0, imgH = 0;
        unsigned char *ucImg = [CLOOpenGLImageUtility sCreateRGBA:img withOutW:&imgW withOutH:&imgH withPremultiplied:NO];
        if (ucImg && imgW > 0 && imgH > 0) {
            CLOWS
            [self.mGLContext fRunSynchronouslyOnContextQueue:^{
                CLOSS
                bRet = self.m_glCtr->fSetupImage((uint32_t)index, ucImg, CLO::eImageType_RGBA, imgW, imgH);
            }];
        }
        CLOCArrayRelease(ucImg);
    }
    
    return bRet;
}

- (BOOL)fSetupEffect:(NSString *)string
{
    __block BOOL bRet = NO;
    if (string.length > 0) {
        CLOWS
        [self.mGLContext fRunSynchronouslyOnContextQueue:^{
            CLOSS
            bRet = self.m_glCtr->fSetupEffectString(string.UTF8String);
        }];
    }
    
    return bRet;
}

- (BOOL)fMake
{
    return NO;
}

- (UIImage *)fGetMakedImage
{
    return nil;
}
@end
