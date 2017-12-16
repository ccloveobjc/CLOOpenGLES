//
//  CLOglProgram.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/12.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglProgram_hpp
#define CLOglProgram_hpp

#include "CLOglGlobal.h"
#include <string>
#include <vector>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

CLOCNamespaceS

class CLOglProgram {
public:
    CLOglProgram(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString);
    ~CLOglProgram();
    
    std::string fDescription();
    
    bool fLink();
    bool fUse();
    
    GLuint fGetProgramID();
private:
    
    bool fCompileShader(GLuint *shader, const GLenum type, const std::string shaderCodeString);
    
    GLuint mProgramID = 0;
    GLuint mVertexShaderID = 0;
    GLuint mFragmentShaderID = 0;
    
    std::vector<std::string> mAttributes;
    std::vector<std::string> mUniforms;
    
    bool mInitialized = false; // 如果link成功，会变为true
    std::string mVertexShaderLog;
    std::string mFragmentShaderLog;
    std::string mProgramLog;
};

CLOCNamespaceE
#endif /* CLOglProgram_hpp */
