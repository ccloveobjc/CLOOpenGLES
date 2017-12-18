//
//  CLOglFilter.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/13.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglFilter_hpp
#define CLOglFilter_hpp

#include <stdio.h>
#include "CLOglGlobal.h"
#include <string>
#include <vector>
#include <memory>

CLOCNamespaceS

class CLOglProgram;
class CLOglSession;

class CLOglFilter {
public:
    CLOglFilter();
    CLOglFilter(std::shared_ptr<CLOglSession> session);
    CLOglFilter(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString);
    CLOglFilter(std::shared_ptr<CLOglSession> session, const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString);
    ~CLOglFilter();
    
    std::string fDescription();
    
    bool fUse();
    
    int fGetAttribute_position();
    int fGetAttribute_inputTextureCoordinate();
    int fGetUniform_inputImageTexture();
    
    virtual bool fMake();
private:
    std::shared_ptr<CLOglSession> mSession;
    std::shared_ptr<CLOglProgram> mProgram;
    std::string mVertexShaderString;
    std::string mFragmentShaderString;
};

CLOCNamespaceE
#endif /* CLOglFilter_hpp */
