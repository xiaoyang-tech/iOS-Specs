//
//  MeasurementSDK.h
//  MeasurementSDK
//
//  OC 对外唯一入口。
//
//  集成方式：
//    #import <MeasurementSDK/MeasurementSDK.h>
//
//  注意：
//  - Swift 宿主请通过 Bridging Header 引入，不要使用 `import MeasurementSDK`
//  - 报告类型 Report / HrReport 见 Report.pbobjc.h
//  - 测量指标枚举见 Category.pbobjc.h
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
