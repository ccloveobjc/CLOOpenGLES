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
#include "CLOglColorMatrixFilter.hpp"
#include "CLOglFrameBuffer.hpp"

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

bool CLOglCtr::fSetupImage(uint32_t index, uint32_t textureID, uint32_t width, uint32_t height, bool needFree)
{
    auto texture = std::shared_ptr<CLOglTexture>(new CLOglTexture(textureID, width, height, needFree));
    return fSetupImage(index, texture);
}

bool CLOglCtr::fSetupEffectString(std::string effectStr)
{
    CLOglColorMatrixFilter *matrixFilter = new CLOglColorMatrixFilter(mSession);
    matrixFilter->fSetupIntensity(1.0);
    matrixFilter->fSetupColorMatrix({
        {0.3588, 0.7044, 0.1368, 0.0},
        {0.2990, 0.5870, 0.1140, 0.0},
        {0.2392, 0.4696, 0.0912, 0.0},
        {0,          0,         0,          1.0}});
    
    auto filter = dynamic_cast<CLOglFilter *>(matrixFilter);
    mFilter = std::shared_ptr<CLOglFilter>(filter);
    if (mFilter->fUse()) {

        return true;
    }
    
    return false;
}

bool CLOglCtr::fMakeImage()
{
    if ( ! mFilter) {
        
        CLOCLog("没有设置Filter");
        return false;
    }
    
    auto input0 = mSession->fGetImage(0);
    if (!input0) {
        
        CLOCLog("没有设置输入图片 index:0");
        return false;
    }
    
    auto framebuffer = mSession->fGetOrCreateFramebuffer(input0->fGetWidth(), input0->fGetHeight());
    auto output0 = mSession->fGetOutputTexture(0);
    if (!output0) {
        
        auto outText = CLOglTexture::sCreateTexture(nullptr, input0->fGetWidth(), input0->fGetHeight());
        output0 = std::shared_ptr<CLOglTexture>(outText);
        mSession->fSetupOutputTexture(0, output0);
    }
    else {
        
        output0->fUpdateSize(input0->fGetWidth(), input0->fGetHeight());
    }
    
    if (!framebuffer->fBindTexture(output0)) {
        
        CLOCLog("绑定 framebuffer:%s 和 texture:%s 错误！", framebuffer->fDescription().c_str(), output0->fDescription().c_str());
        return false;
    }
    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };
    static const GLfloat noRotationTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    return fMakeImage(framebuffer, imageVertices, noRotationTextureCoordinates);
}

bool CLOglCtr::fMakeImage(std::shared_ptr<CLOglFrameBuffer> framebuffer, const GLfloat *pVertices, const GLfloat *pTextureCoordinates)
{
    if ( ! mFilter) {
        
        CLOCLog("没有设置Filter");
        return false;
    }
    
    auto input0 = mSession->fGetImage(0);
    if (!input0) {
        
        CLOCLog("没有设置输入图片 index:0");
        return false;
    }
    
    if ( ! framebuffer->fBind()) {
        
        CLOCLog("FrameBuffer 绑定失败 .");
        return false;
    }
    
    return mFilter->fMake(pVertices, pTextureCoordinates);
}

std::shared_ptr<CLOglPixel> CLOglCtr::fGetMakedImage()
{
    auto output0 = mSession->fGetOutputTexture(0);
    if ( ! output0) {

        return nullptr;
    }

    auto framebuffer = mSession->fGetOrCreateFramebuffer(output0->fGetWidth(), output0->fGetHeight());
    if ( ! framebuffer) {

        return nullptr;
    }

    framebuffer->fBindTexture(output0);
    
    return framebuffer->fReadPixels();
}
