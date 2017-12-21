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
int CLOglFilter::fGetUniform_inputImageTexture()
{
    auto index = glGetUniformLocation(mProgram->fGetProgramID(), "inputImageTexture");
    return index;
}


bool CLOglFilter::fMake(const GLfloat *pVertices, const GLfloat *pTextureCoordinates)
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
    
    glClearColor(255, 0, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if ( ! onSetupParams()) {
        
        return false;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input0->fGetTextureID());
    glUniform1i(fGetUniform_inputImageTexture(), 0);
    
    int position_id = fGetAttribute_position();
    glVertexAttribPointer(position_id, 2, GL_FLOAT, 0, 0, pVertices); // 2 2维
    glEnableVertexAttribArray(position_id);
    
    int inputTextureCoordinate_id = fGetAttribute_inputTextureCoordinate();
    glVertexAttribPointer(inputTextureCoordinate_id, 2, GL_FLOAT, 0, 0, pTextureCoordinates);
    glEnableVertexAttribArray(inputTextureCoordinate_id);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableVertexAttribArray(position_id);
    glDisableVertexAttribArray(inputTextureCoordinate_id);
    
    if (glGetError() != GL_NO_ERROR) {
        
        CLOCAssert(false, "");
    }
    
    return true;
}

bool CLOglFilter::onSetupParams()
{
    return true;
}
