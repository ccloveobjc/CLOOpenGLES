//
//  CLOglTexture.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglTexture_hpp
#define CLOglTexture_hpp

#include "CLOglGlobal.h"
CLONamespaceS

class CLOglTexture {
public:
    CLOglTexture();
    CLOglTexture(uint32_t textureID, uint32_t w, uint32_t h, bool needFree);
    ~CLOglTexture();
    
    
private:
    uint32_t mTextureID = 0;
    uint32_t mW = 0;
    uint32_t mH = 0;
    bool mNeedFree = false;
    
    bool fDeleteTexture();
};

CLONamespaceE
#endif /* CLOglTexture_hpp */
