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
