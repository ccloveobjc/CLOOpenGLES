//
//  CLOOpenGLGlobal.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOOpenGLGlobal_h
#define CLOOpenGLGlobal_h

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
    #define CLOAssert(b, fmt, ...) if ((b) == false){ printf((fmt), ##__VA_ARGS__); assert(false); }
#else
    #define CLOAssert(b, fmt, ...)
#endif

#endif /* CLOOpenGLGlobal_h */
