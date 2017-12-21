//
//  CLOglColorMatrixFilter.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/13.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglColorMatrixFilter.hpp"
#include "CLOglProgram.hpp"

CLOCNamespaceUse

CLOglColorMatrixFilter::CLOglColorMatrixFilter(std::shared_ptr<CLOglSession> session)
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

uniform lowp mat4 colorMatrix;
uniform lowp float intensity;

void main()
{
    lowp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    lowp vec4 outputColor = textureColor * colorMatrix;
    
    gl_FragColor = (intensity * outputColor) + ((1.0 - intensity) * textureColor);
}
            )")
{}

CLOglColorMatrixFilter::~CLOglColorMatrixFilter()
{
    
}

bool CLOglColorMatrixFilter::fSetupIntensity(float value)
{
    if (value >= 0 && value <= 1.0) {
    
        mIntensity = value;
        return true;
    }
    
    return false;
}
float CLOglColorMatrixFilter::fGetIntensity()
{
    return mIntensity;
}

bool CLOglColorMatrixFilter::fSetupColorMatrix(CLOMatrix4x4 value)
{
//    GLfloat *pDst=&mColorMatrix[0][0];
//    GLfloat *pSrc = value;
//    int floatSize=sizeof(GLfloat);
//    int memSize=floatSize*4*4;
//    memcpy(pDst, pSrc, memSize);
    
    mColorMatrix = value;
    return true;
}

CLOMatrix4x4 CLOglColorMatrixFilter::fGetColorMatrix()
{
    return mColorMatrix;
}

int CLOglColorMatrixFilter::fGetUniform_colorMatrix()
{
    auto index = glGetUniformLocation(mProgram->fGetProgramID(), "colorMatrix");
    return index;
}
int CLOglColorMatrixFilter::fGetUniform_intensity()
{
    auto index = glGetUniformLocation(mProgram->fGetProgramID(), "intensity");
    return index;
}

bool CLOglColorMatrixFilter::onSetupParams()
{
    glUniformMatrix4fv(fGetUniform_colorMatrix(), 1, GL_FALSE, (GLfloat *)&mColorMatrix);
    glUniform1f(fGetUniform_intensity(), mIntensity);
    
    return CLOglFilter::onSetupParams();
}

