//
//  CLOOpenGLContext.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class EAGLContext;

@interface CLOOpenGLContext : NSObject

    @property (nonatomic,strong,readonly) EAGLContext *mEAGLContext;

@end
