//
//  XYFaceDetector.h
//  MeasurementSDK
//
//  人脸引导检测。用于测量前的姿态校验，不上传特征数据。
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 人脸检测完成回调。
 * @param isValid 当前帧是否通过校验（人脸在框内、姿态/距离合规等）
 * @param code    校验结果码；无效时可用于区分原因（如人脸丢失 3011）
 * @param message 可读提示文案，可直接展示给用户
 */
typedef void (^XYFaceDetectCompletion)(BOOL isValid, NSInteger code, NSString *message);

/**
 * 人脸引导检测器（单例）。
 *
 * 用于 BEFORE 引导、二次人脸确认等阶段：
 * - 调用 `detectAsyncWithSampleBuffer:...` 异步校验每一帧
 * - 累计足够多 isValid == YES 的帧后，再调用 XYMeasurementManager.start
 *
 * 注意：正式测量阶段应改用 XYMeasurementManager.enqueueSampleBuffer:... 送帧。
 * 停止送帧约 1.5s 后再送帧会自动重置累计状态，无需手动 reset。
 */
@interface XYFaceDetector : NSObject

/// 全局单例
@property (class, nonatomic, readonly) XYFaceDetector *shared;

/**
 * 异步检测一帧（previewSize 默认取主屏幕 bounds.size）。
 * @param sampleBuffer 相机 CMSampleBuffer
 * @param orientation  帧方向
 * @param timestamp    帧时间戳（毫秒，建议用 Date 转 wall-clock ms）
 * @param guideFrame   引导框 CGRect（预览视图坐标系）
 * @param completion   主线程回调检测结果
 */
- (void)detectAsyncWithSampleBuffer:(CMSampleBufferRef)sampleBuffer
                        orientation:(UIImageOrientation)orientation
                          timestamp:(NSInteger)timestamp
                         guideFrame:(CGRect)guideFrame
                         completion:(nullable XYFaceDetectCompletion)completion;

/**
 * 异步检测一帧（指定 previewSize）。
 * @param previewSize 预览区域尺寸，须与 guideFrame 处于同一坐标系
 */
- (void)detectAsyncWithSampleBuffer:(CMSampleBufferRef)sampleBuffer
                        orientation:(UIImageOrientation)orientation
                          timestamp:(NSInteger)timestamp
                         guideFrame:(CGRect)guideFrame
                        previewSize:(CGSize)previewSize
                         completion:(nullable XYFaceDetectCompletion)completion;

@end

NS_ASSUME_NONNULL_END
