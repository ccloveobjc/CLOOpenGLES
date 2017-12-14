//
//  CLOglGlobal.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglGlobal_h
#define CLOglGlobal_h

#include <stdio.h>

// 释放宏
#define CLOClassRelease(cls) if (cls) { \
    delete cls; \
    cls = nullptr; \
}
#define CLOArrayRelease(arr) if (arr) { \
    delete [] arr; \
    arr = nullptr; \
}

// 命名空间宏
#define CLONamespaceS namespace CLO {
#define CLONamespaceE }
#define CLONamespaceUse using namespace CLO;

// 断言宏
#include <cassert>
#ifdef DEBUG
    #define CLOAssert(b, fmt, ...) if ((b) == false){ CLOLog(fmt, ##__VA_ARGS__); assert(false); }
#else
    #define CLOAssert(b, fmt, ...)
#endif

// 日志
#ifdef DEBUG
    #define CLOLog(fm, ...) printf(fm, ##__VA_ARGS__); printf("\n")
#else
    #define CLOLog(fm, ...)
#endif


#endif /* CLOglGlobal_h */
