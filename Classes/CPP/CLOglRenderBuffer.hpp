//
//  CLOglRenderBuffer.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/19.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglRenderBuffer_hpp
#define CLOglRenderBuffer_hpp

#include <stdio.h>
#include "CLOglGlobal.h"
#include <string>
#ifdef __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

CLOCNamespaceS

class CLOglRenderBuffer {
public:
    CLOglRenderBuffer();
    ~CLOglRenderBuffer();
    
    std::string fDescription();
    
    GLuint fGetRenderbufferID();
    
    bool fBind();
    
private:

    GLuint mRenderbufferID = 0;
};

CLOCNamespaceE
#endif /* CLOglRenderBuffer_hpp */
