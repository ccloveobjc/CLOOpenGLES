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
#include <memory>
#include <string>

CLOCNamespaceS

class CLOglTexture {
public:
    CLOglTexture();
    CLOglTexture(uint32_t textureID, uint32_t width, uint32_t height, bool needFree);
    ~CLOglTexture();
    
    std::string fDescription();
    
    uint32_t fGetTextureID();
    uint32_t fGetWidth();
    uint32_t fGetHeight();
    
    static std::shared_ptr<CLOglTexture> sCreateTexture(unsigned char *pPixel, uint32_t width, uint32_t height);
    
private:
    uint32_t mTextureID = 0;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    bool mNeedFree = false;
    
    bool fDeleteTexture();
};

CLOCNamespaceE
#endif /* CLOglTexture_hpp */
