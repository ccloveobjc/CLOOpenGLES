//
//  CLOOpenGLGlobal.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/14.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOOpenGLGlobal_h
#define CLOOpenGLGlobal_h

// block
#define CLOWS __weak __typeof__(self) self_weak_ = (self);
#define CLOSS __strong __typeof__(self) self = self_weak_;

// 日志
#ifdef DEBUG
    #define CLONSLog(fm, ...) NSLog(fm, ##__VA_ARGS__)
#else
    #define CLONSLog(fm, ...)
#endif

#endif /* CLOOpenGLGlobal_h */
