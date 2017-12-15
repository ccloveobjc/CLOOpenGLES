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
#define CLOCClassRelease(cls) if (cls) { \
    delete cls; \
    cls = nullptr; \
}
#define CLOCArrayRelease(arr) if (arr) { \
    delete [] arr; \
    arr = nullptr; \
}

// 命名空间宏
#define CLOCNamespaceS namespace CLO {
#define CLOCNamespaceE }
#define CLOCNamespaceUse using namespace CLO;

// 断言宏
#include <cassert>
#ifdef DEBUG
    #define CLOCAssert(b, fmt, ...) if ((b) == false){ CLOCLog(fmt, ##__VA_ARGS__); assert(false); }
#else
    #define CLOCAssert(b, fmt, ...)
#endif

// 日志
#ifdef DEBUG
    #define CLOCLog(fm, ...) printf(fm, ##__VA_ARGS__); printf("\n")
#else
    #define CLOCLog(fm, ...)
#endif


#endif /* CLOglGlobal_h */
