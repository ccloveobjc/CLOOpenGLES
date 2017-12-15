//
//  CLOglFrameBuffer.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglFrameBuffer_hpp
#define CLOglFrameBuffer_hpp

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

class CLOglFrameBuffer {
public:
    CLOglFrameBuffer(uint32_t width, uint32_t height);
    ~CLOglFrameBuffer();
    
    std::string fDescription();
    
    bool fBindTexture(std::shared_ptr<CLOglTexture> texture);
    
private:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    GLuint mFramebufferID;
    
    std::shared_ptr<CLOglTexture> mTexture = nullptr;
};

CLOCNamespaceE
#endif /* CLOglFrameBuffer_hpp */
