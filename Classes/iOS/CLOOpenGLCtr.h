//
//  CLOOpenGLCtr.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

    @class CLOOpenGLContext;

@interface CLOOpenGLCtr : NSObject

    @property (nonatomic,strong,readonly) CLOOpenGLContext *mGLContext;

- (instancetype)init;

// CLO::CLOglCtr 格式
- (void *)fGetCLOglCtr;

- (BOOL)fSetupIndex:(NSUInteger)index withUIImage:(UIImage *)img;
- (BOOL)fSetupIndex:(NSUInteger)index withTextureID:(int)textureID withWidth:(int)width withHeight:(int)height withNeedFree:(bool)needFree;

- (BOOL)fSetupEffect:(NSString *)string;

- (BOOL)fMake;

- (UIImage *)fGetMakedImage;

@end
