//
//  CLOOpenGLView.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CLOOpenGLCtr;

@interface CLOOpenGLView : UIView

    @property (nonatomic,strong,readonly) CLOOpenGLCtr *mGLContext;

- (instancetype)initWithFrame:(CGRect)frame withContext:(CLOOpenGLCtr *)context;

@end
