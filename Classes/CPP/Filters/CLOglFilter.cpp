//
//  CLOglFilter.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/13.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglFilter.hpp"
#include "CLOglProgram.hpp"
#include "CLOglSession.hpp"
#include "CLOglTexture.hpp"
#include "CLOglFrameBuffer.hpp"

CLOCNamespaceUse

CLOglFilter::CLOglFilter()
    : CLOglFilter(nullptr)
{}

CLOglFilter::CLOglFilter(std::shared_ptr<CLOglSession> session)
    : CLOglFilter(session,
                  R"(
attribute vec4 position;
attribute vec4 inputTextureCoordinate;

varying vec2 textureCoordinate;

void main()
{
  gl_Position = position;
  textureCoordinate = inputTextureCoordinate.xy;
}
                  )",
                R"(
varying highp vec2 textureCoordinate;

uniform sampler2D inputImageTexture;

void main()
{
    gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
}
                )")
{}

CLOglFilter::CLOglFilter(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString)
    : CLOglFilter(nullptr, vertexShaderCodeString, fragmentShaderCodeString)
{}

CLOglFilter::CLOglFilter(std::shared_ptr<CLOglSession> session, const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString)
{
    mSession = session;
    mVertexShaderString = vertexShaderCodeString;
    mFragmentShaderString = fragmentShaderCodeString;
    
    std::shared_ptr<CLOglProgram> program = nullptr;
    if (mSession) {
        
        program = mSession->fGetOrCreateProgram(mVertexShaderString, mFragmentShaderString);
    }
    else {
    
        program = std::shared_ptr<CLOglProgram>(new CLOglProgram(vertexShaderCodeString, fragmentShaderCodeString));
    }
    mProgram = program;
}

CLOglFilter::~CLOglFilter()
{
    
}

bool CLOglFilter::fUse()
{
    bool bRet = mProgram->fUse();
    if (!bRet) {
        
        CLOCLog("Use 失败");
    }
    
    return bRet;
}

//bool CLOglFilter::fAddAttribute(std::string name)
//{
//    auto obj = std::find(mAttributes.begin(), mAttributes.end(), name);
//    if (obj == mAttributes.end()) {
//
//        mAttributes.push_back(name);
//        long index = std::distance(mAttributes.begin(), obj);
//        glBindAttribLocation(mProgram->fGetProgramID(), (GLuint)index, name.c_str());
//    }
//
//    return true;
//}

int CLOglFilter::fGetAttribute_position()
{
    auto index = glGetAttribLocation(mProgram->fGetProgramID(), "position");
    return index;
}
int CLOglFilter::fGetAttribute_inputTextureCoordinate()
{
    auto index = glGetAttribLocation(mProgram->fGetProgramID(), "inputTextureCoordinate");
    return index;
}
int CLOglFilter::fGetUniform_textureCoordinate()
{
    auto index = glGetUniformLocation(mProgram->fGetProgramID(), "textureCoordinate");
    return index;
}


bool CLOglFilter::fMake()
{
    auto input0 = mSession->fGetImage(0);
    if (!input0) {
    
        CLOCLog("没有设置输入图片 index:0");
        return false;
    }
    
    // 使用 ProgramID
    if (!fUse()) {
        
        CLOCLog("使用 %s 时 错误！", mProgram->fDescription().c_str());
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
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input0->fGetTextureID());
    glUniform1i(fGetUniform_textureCoordinate(), 0);
    
    static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
    };
    glVertexAttribPointer(fGetAttribute_position(), 2, GL_FLOAT, 0, 0, (GLvoid*)imageVertices);
    
    static const GLfloat noRotationTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    glVertexAttribPointer(fGetAttribute_inputTextureCoordinate(), 2, GL_FLOAT, 0, 0, (GLvoid *)noRotationTextureCoordinates);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    return true;
}
