//
//  CLOglSession.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/14.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglSession.hpp"
#include "CLOglTexture.hpp"
#include "CLOglFrameBuffer.hpp"
#include "CLOglProgram.hpp"

CLOCNamespaceUse

CLOglSession::CLOglSession()
{
    mInputs.resize(32);
    CLOCLog("CLOglSession new");
}

CLOglSession::~CLOglSession()
{
    CLOCLog("CLOglSession delete");
}

bool CLOglSession::fSetupImage(const uint32_t index, std::shared_ptr<CLOglTexture> texture)
{
    if (index < 32) {
        
        CLOCLog("设置纹理 %s  到 index:%u", texture->fDescription().c_str(), index);
        mInputs[index] = texture;
        
        return true;
    }
    
    return false;
}
std::shared_ptr<CLOglTexture> CLOglSession::fGetImage(const uint32_t index)
{
    if (index < 32) {
        
        return mInputs[index];
    }
    
    return nullptr;
}

std::shared_ptr<CLOglFrameBuffer> CLOglSession::fGetOrCreateFramebuffer(const uint32_t width, const uint32_t height)
{
    std::string key = "W:" + std::to_string(width) + " V:" + std::to_string(height);
    if (mFrameBufferCache.find(key) == mFrameBufferCache.end()) {
        
        auto framebuffer = std::shared_ptr<CLOglFrameBuffer>(new CLOglFrameBuffer(width, height));
        CLOCLog("创建Framebuffer %s", framebuffer->fDescription().c_str());
        mFrameBufferCache[key] = framebuffer;
    }
    
    return mFrameBufferCache[key];
}

std::shared_ptr<CLOglProgram> CLOglSession::fGetOrCreateProgram(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString)
{
    std::string key = "V:" + vertexShaderCodeString + " F:" + fragmentShaderCodeString;
    if (mProgramCache.find(key) == mProgramCache.end()) {
        
        auto pFilter = new CLOglProgram(vertexShaderCodeString, fragmentShaderCodeString);
        if (pFilter->fLink()) {
            
            auto filter = std::shared_ptr<CLOglProgram>(pFilter);
            mProgramCache[key] = filter;
        }
        else {
            
            CLOCClassRelease(pFilter);
        }
    }
    
    return mProgramCache[key];
}
