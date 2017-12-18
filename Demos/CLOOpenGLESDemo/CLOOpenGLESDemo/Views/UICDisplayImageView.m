//
//  UICDisplayImageView.m
//  UICEditSDK
//
//  Created by Cc on 14/12/11.
//  Copyright (c) 2014年 PinguoSDK. All rights reserved.
//

#import "UICDisplayImageView.h"
#import "CLOOpenGLGlobal.h"

@interface UICDisplayImageView ()

//原始图
@property (nonatomic,strong) UIImage *mOrigImage;

//原始缩略图
@property (nonatomic,strong) UIImage *mOrigPreviewImage;

//编辑后效果图
@property (nonatomic,strong) UIImage *mPreviewImage;

@property (nonatomic,assign) BOOL     mIsShowOrigImage;

@end

@implementation UICDisplayImageView

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    self.userInteractionEnabled = YES;
    self.mIsShowOrigImage = NO;
}

- (void)pSetupOrigImage:(UIImage *)image
{
    self.mOrigImage = image;
    {
//        CGFloat w = self.frame.size.width;
//        CGFloat h = w / (image.size.width / image.size.height);
//        self.mOrigPreviewImage = [image c_common_ResizedImage:CGSizeMake(w, h) interpolationQuality:(kCGInterpolationDefault)];
        self.mOrigPreviewImage = image;
    }
    [self pSetupPreviewImage:nil];
}

- (void)pSetupPreviewImage:(UIImage *)image
{
    self.mPreviewImage = image;
    [self pUpdateDisplay];
}

- (void)pUpdateDisplay
{
    CLOWS
    dispatch_async(dispatch_get_main_queue(), ^{
        CLOSS
        if (self.mIsShowOrigImage) {
            
            [self setImage:self.mOrigPreviewImage];
        }
        else {
            
            if (self.mPreviewImage) {
                
                [self setImage:self.mPreviewImage];
            }
            else {
                
                [self setImage:self.mOrigPreviewImage];
            }
        }
    });
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    self.mIsShowOrigImage = YES;
    [self pUpdateDisplay];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    self.mIsShowOrigImage = NO;
    [self pUpdateDisplay];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    self.mIsShowOrigImage = NO;
    [self pUpdateDisplay];
}
@end
