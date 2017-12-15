//
//  CLOglFrameBuffer.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglFrameBuffer.hpp"
#include "CLOglTexture.hpp"

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
    
}

std::string CLOglFrameBuffer::fDescription()
{
    return "FramebufferID:" + std::to_string(mFramebufferID)
    + "   Width:" + std::to_string(mWidth)
    + "   Height:" + std::to_string(mHeight);
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
    
    if (texture.get() && texture->fGetWidth() == mWidth && texture->fGetHeight() == mHeight) {
        
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
