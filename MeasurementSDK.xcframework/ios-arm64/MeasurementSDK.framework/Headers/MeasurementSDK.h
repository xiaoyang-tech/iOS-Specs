//
//  MeasurementSDK.h
//  MeasurementSDK
//
//  OC 对外唯一入口。
//
//  集成方式：
//    OC：#import <MeasurementSDK/MeasurementSDK.h>
//    Swift：Bridging Header 只需 #import "MeasurementSDK-SwiftBridge.h"
//
//  注意：
//  - Swift 宿主请勿 `import MeasurementSDK`
//  - 直接使用 XYMeasurementManager / XYFaceDetector，无需在 App 内自建 Facade
//

#import <Foundation/Foundation.h>

#import <MeasurementSDK/Report.pbobjc.h>
#import <MeasurementSDK/Category.pbobjc.h>
#import <MeasurementSDK/Chunk.pbobjc.h>

#import <MeasurementSDK/XYMeasurementDelegate.h>
#import <MeasurementSDK/XYMeasurementConfig.h>
#import <MeasurementSDK/XYMeasurementManager.h>
#import <MeasurementSDK/XYFaceDetector.h>
#import <MeasurementSDK/XYLogConfig.h>

#import <MeasurementSDK/ProcessFrameManager.h>
#import <MeasurementSDK/FeatureRaw.h>
