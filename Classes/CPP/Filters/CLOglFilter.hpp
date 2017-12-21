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

#ifdef __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

CLOCNamespaceS

struct CLOVector4 {
    GLfloat one = 0;
    GLfloat two = 0;
    GLfloat three = 0;
    GLfloat four = 0;
};

struct CLOVector3 {
    GLfloat one = 0;
    GLfloat two = 0;
    GLfloat three = 0;
};

struct CLOMatrix3x3 {
    CLOVector3 one;
    CLOVector3 two;
    CLOVector3 three;
};

struct CLOMatrix4x4 {
    CLOVector4 one;
    CLOVector4 two;
    CLOVector4 three;
    CLOVector4 four;
};

class CLOglProgram;
class CLOglSession;

class CLOglFilter {
public:
    CLOglFilter();
    CLOglFilter(std::shared_ptr<CLOglSession> session);
    CLOglFilter(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString);
    CLOglFilter(std::shared_ptr<CLOglSession> session, const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString);
    virtual ~CLOglFilter();
    
    std::string fDescription();
    
    virtual bool fUse();
    
    int fGetAttribute_position();
    int fGetAttribute_inputTextureCoordinate();
    int fGetUniform_inputImageTexture();
    
    virtual bool fMake(const GLfloat *pVertices, const GLfloat *pTextureCoordinates);
    virtual bool onSetupParams();
protected:
    std::shared_ptr<CLOglSession> mSession;
    std::shared_ptr<CLOglProgram> mProgram;
    std::string mVertexShaderString;
    std::string mFragmentShaderString;
};

CLOCNamespaceE
#endif /* CLOglFilter_hpp */
