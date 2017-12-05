//
//  CLOglTexture.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglTexture.hpp"
#ifdef __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
CLONamespaceUse

CLOglTexture::CLOglTexture()
{
    
}

CLOglTexture::CLOglTexture(uint32_t textureID, uint32_t w, uint32_t h, bool needFree)
{
    mTextureID = textureID;
    mW = w;
    mH = h;
    mNeedFree = needFree;
}

CLOglTexture::~CLOglTexture()
{
    fDeleteTexture();
}

///  Private:
bool CLOglTexture::fDeleteTexture()
{
    if (mTextureID > 0 && mNeedFree) {
        
        glDeleteTextures(1, &mTextureID);
    }
    
    mTextureID = 0;
    mW = 0;
    mH = 0;
    mNeedFree = false;
    
    return true;
}
