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

CLOCNamespaceUse

CLOglTexture::CLOglTexture()
{
    
}

CLOglTexture::CLOglTexture(uint32_t textureID, uint32_t width, uint32_t height, bool needFree)
{
    mTextureID = textureID;
    mWidth = width;
    mHeight = height;
    mNeedFree = needFree;
}

CLOglTexture::~CLOglTexture()
{
    fDeleteTexture();
}

std::string CLOglTexture::fDescription()
{
    return "TextureID:" + std::to_string(mTextureID)
    + "   Width:" + std::to_string(mWidth)
    + "   Height:" + std::to_string(mHeight)
    + "   NeedFree:" + std::to_string(mNeedFree);
}

uint32_t CLOglTexture::fGetTextureID() { return mTextureID; }
uint32_t CLOglTexture::fGetWidth() { return mWidth; }
uint32_t CLOglTexture::fGetHeight() { return mHeight; }


bool CLOglTexture::fUpdateSize(const uint32_t width, const uint32_t height)
{
    if (width == fGetWidth() && height == fGetHeight()) {
        
        return true;
    }
    
    if (mTextureID > 0) {
    
        mWidth = width;
        mHeight = height;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    else {
        
        
    }
    
    return true;
}

std::shared_ptr<CLOglTexture> CLOglTexture::sCreateTexture(unsigned char *pPixel, const uint32_t width, const uint32_t height)
{
    auto texture = new CLOglTexture();
    if (CLOglTexture::sCreateTexture(texture, pPixel, width, height)) {
    
        return std::shared_ptr<CLOglTexture>(texture);
    }
    else {
        
        CLOCClassRelease(texture);
    }
    
    return nullptr;
}

bool CLOglTexture::sCreateTexture(CLOglTexture *pTexture, unsigned char *pPixel, const uint32_t width, const uint32_t height)
{
    pTexture->fDeleteTexture();
    
    GLuint FValue = 0;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &FValue);
    glBindTexture(GL_TEXTURE_2D, FValue);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // This is necessary for non-power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    if (pPixel) {
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixel);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    
    pTexture->mTextureID = FValue;
    pTexture->mWidth = width;
    pTexture->mHeight = height;
    pTexture->mNeedFree = true;
    
    CLOCLog("创建纹理 %s", pTexture->fDescription().c_str());
    
    return true;
}
///  Private:
bool CLOglTexture::fDeleteTexture()
{
    if (mTextureID > 0 && mNeedFree) {
        
        glDeleteTextures(1, &mTextureID);
        CLOCLog("删除纹理 %s", fDescription().c_str());
    }
    
    mTextureID = 0;
    mWidth = 0;
    mHeight = 0;
    mNeedFree = false;
    
    return true;
}
