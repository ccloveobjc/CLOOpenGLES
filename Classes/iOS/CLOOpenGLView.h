//
//  CLOOpenGLView.h
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

    @class CLOOpenGLCtr;

@interface CLOOpenGLView : UIView

    @property (nonatomic,strong,readonly) CLOOpenGLCtr *mGLCtr;

- (instancetype)initWithFrame:(CGRect)frame withGLCtr:(CLOOpenGLCtr *)ctr;

- (BOOL)fRenderBuffer:(CMSampleBufferRef)sampleBuffer;

@end
