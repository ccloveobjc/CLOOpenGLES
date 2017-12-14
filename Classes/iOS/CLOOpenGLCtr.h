//
//  CLOOpenGLCtr.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import <Foundation/Foundation.h>

    @class CLOOpenGLContext;

@interface CLOOpenGLCtr : NSObject

    @property (nonatomic,strong,readonly) CLOOpenGLContext *mGLContext;

- (instancetype)init;

@end
