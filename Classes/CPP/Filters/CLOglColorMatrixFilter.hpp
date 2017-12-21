//
//  CLOglColorMatrixFilter.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/13.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglColorMatrixFilter_hpp
#define CLOglColorMatrixFilter_hpp

#include <stdio.h>
#include "CLOglFilter.hpp"


CLOCNamespaceS

class CLOglSession;

class CLOglColorMatrixFilter : public CLOglFilter {
public:
    CLOglColorMatrixFilter(std::shared_ptr<CLOglSession> session);
    ~CLOglColorMatrixFilter();
    
    bool fSetupIntensity(float value);
    float fGetIntensity();
    
    bool fSetupColorMatrix(CLOMatrix4x4 value);
    CLOMatrix4x4 fGetColorMatrix();
    
    int fGetUniform_colorMatrix();
    int fGetUniform_intensity();
    
    virtual bool onSetupParams();
    
private:
    float mIntensity = 1.0;
    CLOMatrix4x4 mColorMatrix = {
        {1.f, 0.f, 0.f, 0.f},
        {0.f, 1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f},
        {0.f, 0.f, 0.f, 1.f}};
};

CLOCNamespaceE
#endif /* CLOglColorMatrixFilter_hpp */
