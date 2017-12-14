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
#include <string>
#include "CLOglGlobal.h"

@interface CLOOpenGLContext()

    @property (nonatomic,strong) dispatch_queue_t mContextQueue;
    @property (nonatomic,strong) EAGLContext *mEAGLContext;
    @property (nonatomic,assign) std::string mQueueLabel;

@end
@implementation CLOOpenGLContext


- (instancetype)init
{
    self = [super init];
    if (self) {
        
        NSString *uuid = [[NSUUID UUID] UUIDString];
        std::string cuuid = std::string([uuid UTF8String]);
        _mQueueLabel = "com.CLO.openGLESContextQueue" + cuuid;
        
        _mContextQueue = dispatch_queue_create(self.mQueueLabel.c_str(), [self fGetQueueAttribute]);

        _mEAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        CLOLog("CLOOpenGLContext 初始化完成。 使用线程: %s", _mQueueLabel.c_str());
    }
    
    return self;
}

- (void)dealloc
{
    CLOLog("CLOOpenGLContext dealloc");
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
    const char* label = dispatch_queue_get_label(self.mContextQueue);
    if (strcmp(self.mQueueLabel.c_str(), label)) {
        
        block();
    }
    else {
        
        dispatch_sync(self.mContextQueue, block);
    }
}
@end
