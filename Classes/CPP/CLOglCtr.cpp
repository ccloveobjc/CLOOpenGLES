//
//  CLOglCtr.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglCtr.hpp"
#include "CLOglSession.hpp"
#include "CLOglTexture.hpp"
#include "CLOglFilter.hpp"
#ifdef __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

CLOCNamespaceUse

CLOglCtr::CLOglCtr()
{
    mSession = std::shared_ptr<CLOglSession>(new CLOglSession());
}
CLOglCtr::~CLOglCtr()
{
    
}

bool CLOglCtr::fSetupImage(uint32_t index, std::shared_ptr<CLOglTexture> texture)
{
    return mSession->fSetupImage(index, texture);
}

bool CLOglCtr::fSetupImage(uint32_t index, unsigned char *pPixel, eImageType type, uint32_t width, uint32_t height)
{
    auto texture = CLOglTexture::sCreateTexture(pPixel, width, height);
    return fSetupImage(index, texture);
}

bool CLOglCtr::fSetupEffectString(std::string effectStr)
{
    mFilter = std::shared_ptr<CLOglFilter>(new CLOglFilter(mSession));
    if (mFilter->fUse()) {
        
        return true;
    }
    
    return false;
}

bool CLOglCtr::fMakeImage()
{
    if (mFilter) {
        
        return mFilter->fMake();
    }
    
    return false;
}

std::shared_ptr<CLOglPixel> CLOglCtr::fGetMakedImage()
{
//    auto output0 = mSession->fGetOrCreateOutputTexture(0, 0, 0);
    return nullptr;
}
