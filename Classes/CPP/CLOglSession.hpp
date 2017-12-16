//
//  CLOglSession.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/14.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglSession_hpp
#define CLOglSession_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "CLOglGlobal.h"

CLOCNamespaceS

class CLOglTexture;
class CLOglFrameBuffer;
class CLOglProgram;

class CLOglSession {
public:
    CLOglSession();
    ~CLOglSession();
    
    // 纹理坑位 下标最多31
    bool fSetupImage(const uint32_t index, std::shared_ptr<CLOglTexture> texture);
    std::shared_ptr<CLOglTexture> fGetImage(const uint32_t index);
    
    // 纹理输出 下标最大0
    bool fSetupOutputTexture(const uint32_t index, std::shared_ptr<CLOglTexture> texture);
    std::shared_ptr<CLOglTexture> fGetOutputTexture(const uint32_t index);
    
    // Framebuffer
    std::shared_ptr<CLOglFrameBuffer> fGetOrCreateFramebuffer(const uint32_t width, const uint32_t height);
    
    // Program 缓存
    std::shared_ptr<CLOglProgram> fGetOrCreateProgram(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString);
private:
    std::vector<std::shared_ptr<CLOglTexture>> mInputs;
    std::vector<std::shared_ptr<CLOglTexture>> mOutputs;
    std::map<std::string, std::shared_ptr<CLOglFrameBuffer>> mFrameBufferCache;
    std::map<std::string, std::shared_ptr<CLOglProgram>> mProgramCache;
};

CLOCNamespaceE
#endif /* CLOglSession_hpp */
