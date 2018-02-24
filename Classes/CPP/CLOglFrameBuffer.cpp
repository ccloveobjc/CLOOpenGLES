//
//  CLOglFrameBuffer.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglFrameBuffer.hpp"
#include "CLOglTexture.hpp"
#include "CLOglPixel.hpp"
#include "CLOglRenderBuffer.hpp"

CLOCNamespaceUse

CLOglFrameBuffer::CLOglFrameBuffer(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;
    
    glGenFramebuffers(1, &mFramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
}

CLOglFrameBuffer::~CLOglFrameBuffer()
{
    if (mFramebufferID != 0) {
        
        glDeleteFramebuffers(1, &mFramebufferID);
        mFramebufferID = 0;
    }
}

std::string CLOglFrameBuffer::fDescription()
{
    return "FramebufferID:" + std::to_string(mFramebufferID)
    + "   Width:" + std::to_string(mWidth)
    + "   Height:" + std::to_string(mHeight);
}

GLuint CLOglFrameBuffer::fGetFramebufferID() { return mFramebufferID; }
uint32_t CLOglFrameBuffer::fGetWidth() { return mWidth; }
uint32_t CLOglFrameBuffer::fGetHeight() { return mHeight; }

bool CLOglFrameBuffer::fBind()
{
    if (mFramebufferID > 0) {
        
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
        glViewport(0, 0, mWidth, mHeight);
        return true;
    }
    
    return false;
}
bool CLOglFrameBuffer::fBind(uint32_t width, uint32_t height)
{
    if (mWidth != width || mHeight != height) {
        
        mWidth = width;
        mHeight = height;
    }
    
    return fBind();
}

bool CLOglFrameBuffer::fBindTexture(std::shared_ptr<CLOglTexture> texture)
{
    if (texture == nullptr) {
        
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glViewport(0, 0, 0, 0);
        return true;
    }
    
    if (mFramebufferID > 0 && texture.get() && texture->fGetWidth() == mWidth && texture->fGetHeight() == mHeight) {
        
        mTexture = texture;
        uint32_t textID = texture->fGetTextureID();
        
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
        glBindTexture(GL_TEXTURE_2D, textID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textID, 0);
        glViewport(0, 0, mWidth, mHeight);
        return true;
    }
    
    return false;
}

bool CLOglFrameBuffer::fBindRenderbuffer(std::shared_ptr<CLOglRenderBuffer> renderbuffer)
{
    if (renderbuffer == nullptr) {
    
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        return true;
    }
   
    if (mFramebufferID > 0) {
        
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
        if (renderbuffer->fBind()) {
        
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer->fGetRenderbufferID());
            return true;
        }
        else {
            CLOCLog("renderbuffer fBind 失败. Error: %s", renderbuffer->fDescription().c_str());
        }
    }
    
    return false;
}

std::shared_ptr<CLOglPixel> CLOglFrameBuffer::fReadPixels()
{
    if (mFramebufferID > 0) {
    
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
        
        unsigned char * pixels = new unsigned char[mWidth * mHeight * 4];
        glReadPixels(0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        
        CLOglPixel *cloPixel = new CLOglPixel(pixels, mWidth, mHeight, 0, true);
        std::shared_ptr<CLOglPixel> sPixel = std::shared_ptr<CLOglPixel>(cloPixel);
        
        return sPixel;
    }
    
    return nullptr;
}
