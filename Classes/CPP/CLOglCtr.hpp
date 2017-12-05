//
//  CLOglCtr.hpp
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#ifndef CLOglCtr_hpp
#define CLOglCtr_hpp

#include "CLOOpenGLGlobal.h"
CLONamespaceS

class CLOglTexture;

enum eImageType : int {
    
    eImageType_RGBA = 0,
    eImageType_BGRA
};

class CLOglCtr {
public:
    CLOglCtr();
    ~CLOglCtr();
    
    bool fSetupImage(uint32_t index, CLOglTexture &texture);
    bool fSetupImage(uint32_t index, unsigned char *pData, eImageType type, uint32_t w, uint32_t h, bool needFree);
    
    bool fMakeImage();
    
private:
    
};

CLONamespaceE
#endif /* CLOglCtr_hpp */
