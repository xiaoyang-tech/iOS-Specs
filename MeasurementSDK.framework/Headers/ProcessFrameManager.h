//
//  ProcessFrameManager.h
//  measurementSDK
//
//  Created by dck on 2024/8/7.
//

#import <Foundation/Foundation.h>
#import "FeatureRaw.h"
NS_ASSUME_NONNULL_BEGIN

#define XYLog(fmt, ...) NSLog((@"[路径:%s]\n" "[函数名:%s]\n" "[行号:%d] \n" fmt), [[NSString stringWithFormat:@"%s", __FILE__].lastPathComponent UTF8String], __FUNCTION__, __LINE__, ##__VA_ARGS__);

@interface ProcessFrameManager : NSObject

+ (instancetype)sharedInstance;

- (NSString *)processVideoFrameBGRAStr:(void *)baseAddress
                        width:(int)width
                       height:(int)height
                  bytesPerRow:(int)bytesPerRow
                faceLandmarks:(float *)faceLandmarks
             faceBlendShapes:(float *)faceBlendShapes
                   featureExtractor:(int)featureExtractor;

@end

NS_ASSUME_NONNULL_END
