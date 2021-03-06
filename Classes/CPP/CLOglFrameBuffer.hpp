//
//  CLOglFrameBuffer.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglFrameBuffer_hpp
#define CLOglFrameBuffer_hpp

#include <stdio.h>
#include "CLOglGlobal.h"
#include <memory>
#include <string>
#ifdef __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

CLOCNamespaceS

class CLOglTexture;
class CLOglPixel;
class CLOglRenderBuffer;

class CLOglFrameBuffer {
public:
    CLOglFrameBuffer(uint32_t width, uint32_t height);
    ~CLOglFrameBuffer();
    
    std::string fDescription();
    
    GLuint fGetFramebufferID();
    uint32_t fGetWidth();
    uint32_t fGetHeight();
    
    bool fBind();
    bool fBind(uint32_t width, uint32_t height);
    
    bool fBindTexture(std::shared_ptr<CLOglTexture> texture);
    bool fBindRenderbuffer(std::shared_ptr<CLOglRenderBuffer> renderbuffer);
    
    std::shared_ptr<CLOglPixel> fReadPixels();
private:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    GLuint mFramebufferID = 0;
    
    std::shared_ptr<CLOglTexture> mTexture = nullptr;
};

CLOCNamespaceE
#endif /* CLOglFrameBuffer_hpp */
