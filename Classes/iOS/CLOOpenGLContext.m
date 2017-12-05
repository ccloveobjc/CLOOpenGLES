//
//  CLOOpenGLContext.m
//  CLOOpenGLESDemo
//
//  Created by Cc on 2017/12/5.
//  Copyright © 2017年 Cc. All rights reserved.
//

#import "CLOOpenGLContext.h"
#import <OpenGLES/EAGL.h>
#import <UIKit/UIKit.h>

@interface CLOOpenGLContext()

    @property (nonatomic,strong) dispatch_queue_t mContextQueue;
    @property (nonatomic,strong) EAGLContext *mEAGLContext;

@end
@implementation CLOOpenGLContext

static void *sOpenGLESContextQueueKey;

- (instancetype)init
{
    self = [super init];
    if (self) {
        
        sOpenGLESContextQueueKey = &sOpenGLESContextQueueKey;
        _mContextQueue = dispatch_queue_create("com.CLO.openGLESContextQueue", [self fGetQueueAttribute]);
        dispatch_queue_set_specific(_mContextQueue, sOpenGLESContextQueueKey, (__bridge void * _Nullable)(self), NULL);
        _mEAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
    
    return self;
}

- (void)dealloc
{
    printf("CLOOpenGLContext dealloc");
}

- (dispatch_queue_attr_t)fGetQueueAttribute
{
#if TARGET_OS_IPHONE
    if ([[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] != NSOrderedAscending) {
        
        return dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_DEFAULT, 0);
    }
#endif
    return nil;
}
- (void)fUseAsCurrentContext;
{
    if ([EAGLContext currentContext] != self.mEAGLContext) {
        
        [EAGLContext setCurrentContext:self.mEAGLContext];
    }
}
- (void)runSynchronouslyOnContextQueue:(void (^)(void))block
{
    dispatch_queue_t videoProcessingQueue = self.mContextQueue;
#if !OS_OBJECT_USE_OBJC
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    if (dispatch_get_current_queue() == self.mEAGLContext)
    #pragma clang diagnostic pop
#else
    if (dispatch_get_specific(sOpenGLESContextQueueKey))
#endif
    {
        block();
    }
    else {
        
        dispatch_sync(videoProcessingQueue, block);
    }
}
@end
