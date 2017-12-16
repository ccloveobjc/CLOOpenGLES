//
//  CLOglProgram.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/12.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglProgram.hpp"

CLOCNamespaceUse

CLOglProgram::CLOglProgram(const std::string vertexShaderCodeString, const std::string fragmentShaderCodeString)
{
    mAttributes.clear();
    mUniforms.clear();
    
    mProgramID = glCreateProgram();
    CLOCAssert(mProgramID != 0, "Failed to glCreateProgram() == 0");
    
    if ( ! fCompileShader(&mVertexShaderID, GL_VERTEX_SHADER, vertexShaderCodeString)) {
        
        CLOCAssert(false, "Failed to compile vertex shader. Error:%s", mVertexShaderLog.c_str());
    }
    if ( ! fCompileShader(&mFragmentShaderID, GL_FRAGMENT_SHADER, fragmentShaderCodeString)) {
        
        CLOCAssert(false, "Failed to compile fragment shader. Error:%s", mFragmentShaderLog.c_str());
    }
    
    glAttachShader(mProgramID, mVertexShaderID);
    glAttachShader(mProgramID, mFragmentShaderID);
}

CLOglProgram::~CLOglProgram()
{}

std::string CLOglProgram::fDescription()
{
    return "mProgramID:" + std::to_string(mProgramID);
}

bool CLOglProgram::fLink()
{
    if (mProgramID > 0) {
    
        if (mInitialized == true) {
            
            return true;
        }
        
        glLinkProgram(mProgramID);
        
        GLint status = 0;
        glGetProgramiv(mProgramID, GL_LINK_STATUS, &status);
        if (status == GL_TRUE) {
            
            if (mVertexShaderID > 0) {
                
                glDeleteShader(mVertexShaderID);
                mVertexShaderID = 0;
            }
            if (mFragmentShaderID > 0) {
                
                glDeleteShader(mFragmentShaderID);
                mFragmentShaderID = 0;
            }
            
            mInitialized = true;
            
            return true;
        }
    }
    
    return false;
}

bool CLOglProgram::fUse()
{
    if (mInitialized) {
    
        glUseProgram(mProgramID);
        return true;
    }
    
    return false;
}

GLuint CLOglProgram::fGetProgramID()
{
    return mProgramID;
}

// private
bool CLOglProgram::fCompileShader(GLuint *shader, const GLenum type, const std::string shaderCodeString)
{
    const GLchar *source = const_cast<GLchar *>(shaderCodeString.c_str());
    
    // 开始编译
    GLuint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    
    // 巡查错误
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        
        GLint logLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            
            GLchar *log = new GLchar[logLength + 1];
            glGetShaderInfoLog(shaderID, logLength, &logLength, log);
            if (shader == &mVertexShaderID) {
            
                mVertexShaderLog = std::string(log);
            }
            else if (shader == &mFragmentShaderID) {
                
                mFragmentShaderLog = std::string(log);
            }
            else {
                
                CLOCAssert(false, "没有对应的变量保存值");
            }
            
            delete [] log;
        }
        
        glDeleteShader(shaderID);
    }
    else {
    
        *shader = shaderID;
    }
    
    return status == GL_TRUE;
}


