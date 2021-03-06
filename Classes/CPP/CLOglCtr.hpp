//
//  CLOglCtr.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglCtr_hpp
#define CLOglCtr_hpp

#include "CLOglGlobal.h"
#include "CLOglCommon.hpp"
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
class CLOglFilter;
class CLOglSession;
class CLOglPixel;
class CLOglFrameBuffer;

class CLOglCtr {
public:
    CLOglCtr();
    ~CLOglCtr();
    
    bool fSetupImage(uint32_t index, std::shared_ptr<CLOglTexture> texture);
    bool fSetupImage(uint32_t index, unsigned char *pPixel, eImageType type, uint32_t width, uint32_t height);
    bool fSetupImage(uint32_t index, uint32_t textureID, uint32_t width, uint32_t height, bool needFree);
    
    bool fSetupEffectString(std::string effectStr);
    
    bool fMakeImage();
    bool fMakeImage(std::shared_ptr<CLOglFrameBuffer> framebuffer, const GLfloat *pVertices, const GLfloat *pTextureCoordinates);
    
    std::shared_ptr<CLOglPixel> fGetMakedImage();
    
private:
    std::shared_ptr<CLOglSession> mSession;
    std::shared_ptr<CLOglFilter> mFilter;
};

CLOCNamespaceE
#endif /* CLOglCtr_hpp */
