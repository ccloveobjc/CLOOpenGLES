//
//  CLOglRenderBuffer.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/19.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglRenderBuffer.hpp"

CLOCNamespaceUse

CLOglRenderBuffer::CLOglRenderBuffer()
{
    glGenRenderbuffers(1, &mRenderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderbufferID);
}

CLOglRenderBuffer::~CLOglRenderBuffer()
{
    if (mRenderbufferID > 0) {
        
        glDeleteRenderbuffers(1, &mRenderbufferID);
        mRenderbufferID = 0;
    }
}

std::string CLOglRenderBuffer::fDescription()
{
    return "RenderbufferID:" + std::to_string(mRenderbufferID);
}

GLuint CLOglRenderBuffer::fGetRenderbufferID() { return mRenderbufferID; }


bool CLOglRenderBuffer::fBind()
{
    if (mRenderbufferID > 0) {
        
        glBindRenderbuffer(GL_RENDERBUFFER, mRenderbufferID);
        return true;
    }
    
    return false;
}
