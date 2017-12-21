//
//  CLOOpenGLView.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/4.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLView.h"
#import "CLOOpenGLCtr.h"
#import "CLOOpenGLContext.h"
#import "CLOOpenGLGlobal.h"

#include "CLOglCtr.hpp"
#include "CLOglFrameBuffer.hpp"
#include "CLOglRenderBuffer.hpp"

@interface CLOOpenGLView()

    @property (nonatomic,strong) CLOOpenGLCtr *mGLCtr;
    @property (nonatomic,assign) std::shared_ptr<CLO::CLOglFrameBuffer> mFramebuffer;
    @property (nonatomic,assign) std::shared_ptr<CLO::CLOglRenderBuffer> mRenderbuffer;
    @property (nonatomic,assign) CVOpenGLESTextureCacheRef mOpenGLESTextureCacheRef;

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
        
        if (ctr) {
        
            _mGLCtr = ctr;
        }
        else {
            
            _mGLCtr = [[CLOOpenGLCtr alloc] init];
        }
        
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
        self.opaque = YES;
        self.hidden = NO;
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = @{
                                         kEAGLDrawablePropertyRetainedBacking : @YES ,
                                         kEAGLDrawablePropertyColorFormat : kEAGLColorFormatRGBA8 ,
                                         };
        

        
        CLOWS
        [_mGLCtr.mGLContext fRunSynchronouslyOnContextQueue:^{
            CLOSS
            CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, _mGLCtr.mGLContext.mEAGLContext, NULL, &_mOpenGLESTextureCacheRef);
            if (err != kCVReturnSuccess) {
                
                CLONSLog(@"CVOpenGLESTextureCacheCreate error = %@", @(err));
                return;
            }
            
            _mRenderbuffer = std::shared_ptr<CLO::CLOglRenderBuffer>(new CLO::CLOglRenderBuffer());
            _mFramebuffer = std::shared_ptr<CLO::CLOglFrameBuffer>(new CLO::CLOglFrameBuffer(frame.size.width, frame.size.height));
            _mFramebuffer->fBindRenderbuffer(_mRenderbuffer);
            
            [_mGLCtr.mGLContext.mEAGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
            
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                CLONSLog(@"CLOOpenGLView 初始化 失败");
            }
        }];
        
        CLONSLog(@"CLOOpenGLView 初始化完成");
    }
    
    return self;
}

- (void)dealloc
{
    [_mGLCtr.mGLContext fRunSynchronouslyOnContextQueue:^{
       
        _mRenderbuffer = nullptr;
        _mFramebuffer = nullptr;
        if (_mOpenGLESTextureCacheRef) {
            
            CFRelease(_mOpenGLESTextureCacheRef);
            _mOpenGLESTextureCacheRef = NULL;
        }
    }];
    CLONSLog(@"CLOOpenGLView dealloc");
}

- (void)setFrame:(CGRect)frame
{
    [super setFrame:frame];
    // TODO:
}

- (BOOL)fRenderBuffer:(CMSampleBufferRef)buffer
{
    __block BOOL bRet = NO;
    
    // 获取buffer 的textureID
    int textureID = 0, width = 0, height = 0;
    if ( ! [self fGetTextureIDFormSampleBuffer:buffer withOutTextureID:&textureID withOutWidth:&width withOutHeight:&height]) {
        
        return NO;
    }
    
    // 设置图片到 index = 0
    if ( ! [self.mGLCtr fSetupIndex:0 withTextureID:textureID withWidth:width withHeight:height withNeedFree:false]) {
        
        return NO;
    }
    
    CLOWS
    [self.mGLCtr.mGLContext fRunSynchronouslyOnContextQueue:^{
        CLOSS
       
        self.mFramebuffer->fBind(width, height);
        
        CLO::CLOglCtr *glCtr = static_cast<CLO::CLOglCtr *>([self.mGLCtr fGetCLOglCtr]);
        
        static const GLfloat imageVertices[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f,  1.0f,
            1.0f,  1.0f,
        };
        static const GLfloat noRotationTextureCoordinates[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
        };
        
        glCtr->fMakeImage(self.mFramebuffer, imageVertices, noRotationTextureCoordinates);
        
        bRet = [self.mGLCtr.mGLContext.mEAGLContext presentRenderbuffer:GL_RENDERBUFFER];
    }];
    
    return bRet;
}

- (BOOL)fGetTextureIDFormSampleBuffer:(CMSampleBufferRef)sampleBuffer withOutTextureID:(int *)oTextureID withOutWidth:(int *)oWidth withOutHeight:(int *)oHeight
{
    BOOL bRet = NO;
    //正式开始
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    //锁住
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    NSInteger bufferHeight = CVPixelBufferGetHeight(pixelBuffer);
    NSInteger bufferWidth = CVPixelBufferGetWidth(pixelBuffer);
    
//    CVOpenGLESTextureCacheFlush(self.mOpenGLESTextureCacheRef, 0);
    CVOpenGLESTextureRef cvtexture = NULL;
    
    CVReturn cvReturn = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                                     self.mOpenGLESTextureCacheRef,
                                                                     pixelBuffer, NULL, GL_TEXTURE_2D, GL_RGBA,
                                                                     (int)bufferWidth,
                                                                     (int)bufferHeight, GL_BGRA, GL_UNSIGNED_BYTE,
                                                                     0,
                                                                     &cvtexture);
    
    if (cvReturn != kCVReturnSuccess) {
        
        NSAssert(NO, @"CVOpenGLESTextureCacheCreateTextureFromImage error %d", cvReturn);
    }
    else {
        //正确 开始处理
        GLuint textureName = CVOpenGLESTextureGetName(cvtexture);
        GLenum textureTarget = CVOpenGLESTextureGetTarget(cvtexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(textureTarget, textureName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        *oTextureID = (int)textureName;
        *oWidth = (int)bufferWidth;
        *oHeight = (int)bufferHeight;
        
        bRet = YES;
    }
    
    //清除内存
    if (cvtexture != NULL) {
        
        CFRelease(cvtexture);
        cvtexture = NULL;
    }
    
    //解锁
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    return bRet;
}
@end
