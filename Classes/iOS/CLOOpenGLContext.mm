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
#import "CLOOpenGLGlobal.h"

#include <string>


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
        
        dispatch_queue_attr_t attr_t = [self fGetQueueAttribute];
        _mContextQueue = dispatch_queue_create(self.mQueueLabel.c_str(), attr_t);

        _mEAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        CLONSLog(@"CLOOpenGLContext 初始化完成。 使用线程: %@", [NSString stringWithUTF8String:_mQueueLabel.c_str()]);
    }
    
    return self;
}

- (void)dealloc
{
    CLONSLog(@"CLOOpenGLContext dealloc");
}

- (dispatch_queue_attr_t)fGetQueueAttribute
{
    if ([[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] != NSOrderedAscending) {
        
        return dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_DEFAULT, 0);
    }

    return nil;
}

- (void)fUseAsCurrentContext;
{
    if ([EAGLContext currentContext] != self.mEAGLContext) {
        
        [EAGLContext setCurrentContext:self.mEAGLContext];
    }
}
- (void)fRunSynchronouslyOnContextQueue:(void (^)(void))block
{
    const char* label = dispatch_queue_get_label(self.mContextQueue);
    if (strcmp(self.mQueueLabel.c_str(), label)) {
        
        [self fUseAsCurrentContext];
        block();
    }
    else {
        CLOWS
        dispatch_sync(self.mContextQueue, ^{
            CLOSS
            [self fUseAsCurrentContext];
            block();
        });
    }
}
@end
