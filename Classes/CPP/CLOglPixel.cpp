//
//  CLOglPixel.cpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/16.
//  Copyright © 2017年 Cc. All rights reserved.
//

#include "CLOglPixel.hpp"

CLOCNamespaceUse

CLOglPixel::CLOglPixel()
{
    
}
CLOglPixel::CLOglPixel(unsigned char *datas, int w, int h, int rowstep, bool needFree)
{
    mDatas = datas;
    mW = w;
    mH = h;
    mNeedFree = needFree;
    mRowstep = rowstep;
}

CLOglPixel::~CLOglPixel()
{
    fRelease();
}

unsigned char * CLOglPixel::fGetRAW()
{
    return mDatas;
}
int CLOglPixel::fGetWidth()
{
    return mW;
}
int CLOglPixel::fGetHeight()
{
    return mH;
}
int CLOglPixel::fGetRowStep()
{
    if (mRowstep > 0) {
        
        return mRowstep;
    }
    else {
        
        return fGetWidth() * 4;
    }
}

void CLOglPixel::fExchange(CLOglPixel *raw)
{
    if (mW == raw->fGetWidth() && mH == raw->fGetHeight()) {
        
        for (int i = 0; i < mW * mH * 4; ++i) {
            
            unsigned char tmp = mDatas[i];
            mDatas[i] = raw->mDatas[i];
            raw->mDatas[i] = tmp;
        }
        
        bool tmpNeedFree = mNeedFree;
        mNeedFree = raw->mNeedFree;
        raw->mNeedFree = tmpNeedFree;
    }
    else {
        
        CLOCAssert(false, "");
    }
}

void CLOglPixel::fCopyData(CLOglPixel *raw)
{
    int size = fGetSize();
    if (size == raw->fGetSize()) {
        
        for (int i = 0; i < size; ++i) {
            
            mDatas[i] = raw->mDatas[i];
        }
    }
    else {
        
        CLOCAssert(false, "");
    }
}

void CLOglPixel::fUpdateSize(int w, int h, int rowstep)
{
    if (mW == w && mH == h && mRowstep == rowstep) {
        
        return;
    }
    else {
        
        fRelease();
        mW = w;
        mH = h;
        mRowstep = rowstep;
        mNeedFree = true;
        mDatas = new unsigned char[fGetSize()];
    }
}

int CLOglPixel::fGetSize()
{
    if (mRowstep > 0) {
        
        return mH * mRowstep;
    }
    else {
        
        return mH * mW * 4;
    }
}

void CLOglPixel::fRelease()
{
    if (mNeedFree) {
        
        CLOCArrayRelease(mDatas);
    }
    
    mNeedFree = false;
    mDatas = nullptr;
    mW = 0;
    mH = 0;
    mRowstep = 0;
}
