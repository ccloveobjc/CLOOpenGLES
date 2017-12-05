//
//  CLOOpenGLCtr.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLCtr.h"
#import "CLOOpenGLContext.h"

@interface CLOOpenGLCtr()

    @property (nonatomic,strong) CLOOpenGLContext *mGLContext;

@end
@implementation CLOOpenGLCtr

- (instancetype)init
{
    self = [super init];
    if (self) {
        
        _mGLContext = [[CLOOpenGLContext alloc] init];
    }
    
    return self;
}

@end
