//
//  CLOOpenGLView.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLView.h"
#import "CLOOpenGLCtr.h"

@implementation CLOOpenGLView

- (instancetype)initWithFrame:(CGRect)frame withContext:(CLOOpenGLCtr *)context
{
    self = [super initWithFrame:frame];
    if (self) {
        
        _mGLContext = context;
    }
    
    return self;
}

@end
