//
//  CLOOpenGLView.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLView.h"
#import "CLOOpenGLCtr.h"
#include "CLOglGlobal.h"

@interface CLOOpenGLView()

    @property (nonatomic,strong) CLOOpenGLCtr *mGLCtr;

@end
@implementation CLOOpenGLView

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (instancetype)initWithFrame:(CGRect)frame withGLCtr:(CLOOpenGLCtr *)ctr
{
    self = [super initWithFrame:frame];
    if (self) {
        
        _mGLCtr = ctr;
        
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
        self.opaque = YES;
        self.hidden = NO;
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = @{
                                         kEAGLDrawablePropertyRetainedBacking : @YES ,
                                         kEAGLDrawablePropertyColorFormat : kEAGLColorFormatRGBA8 ,
                                         };
        CLOLog("CLOOpenGLView 初始化完成");
    }
    
    return self;
}

@end
