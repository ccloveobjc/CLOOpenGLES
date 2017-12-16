//
//  CLOglPixel.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/16.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglPixel_hpp
#define CLOglPixel_hpp

#include <stdio.h>
#include "CLOglGlobal.h"

CLOCNamespaceS

class CLOglPixel {
public:
    /// datas 会被接管，不要释放
    CLOglPixel(unsigned char *datas, int w, int h, int rowstep = 0, bool needFree = true);
    ~CLOglPixel();
    
    unsigned char * fGetRAW();
    int fGetWidth();
    int fGetHeight();
    int fGetRowStep();
    
    void fExchange(CLOglPixel *raw);
    
    void fCopyData(CLOglPixel *raw);
    
    void fUpdateSize(int w, int h, int rowstep = 0);
    
    int fGetSize();
    
private:
    unsigned char * mDatas = nullptr;
    int mW = 0;
    int mH = 0;
    bool mNeedFree = true;
    int mRowstep = 0;
    
    void fRelease();
    
    CLOglPixel();
};

CLOCNamespaceE
#endif /* CLOglPixel_hpp */
