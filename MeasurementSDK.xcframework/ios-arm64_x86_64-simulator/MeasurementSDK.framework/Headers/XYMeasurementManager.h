//
//  XYMeasurementManager.h
//  MeasurementSDK
//
//  测量主入口。负责鉴权、送帧、SignalR 通信与报告回调。
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

#import <MeasurementSDK/XYMeasurementConfig.h>
#import <MeasurementSDK/XYMeasurementDelegate.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 健康测量管理器（OC 对外主 API）。
 *
 * 典型流程：
 * 1. `setConfig:` — 配置并鉴权
 * 2. （可选）引导阶段使用 XYFaceDetector 做人脸校验
 * 3. `start` — 开始正式测量
 * 4. 相机每帧调用 `enqueueSampleBuffer:...` 送入视频
 * 5. 通过 XYMeasurementDelegate 接收报告
 *
 * 视频帧要求：NV12（kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange）。
 */
@interface XYMeasurementManager : NSObject

/// SDK 版本号（CFBundleShortVersionString）
@property (class, nonatomic, readonly) NSString *sdkVersion;

/// SDK Build 号（CFBundleVersion）
@property (class, nonatomic, readonly) NSString *sdkBuild;

/// 完整版本字符串，如 "1.0.1 (1.1)"
@property (class, nonatomic, readonly) NSString *sdkFullVersion;

/// 测量回调代理（弱引用）
@property (nonatomic, weak, nullable) id<XYMeasurementDelegate> delegate;

/**
 * 设置配置并触发鉴权。
 * 鉴权成功后 SDK 会建立 SignalR 连接；失败时通过 delegate onCrashed 回调。
 */
- (void)setConfig:(XYMeasurementConfig *)config;

/**
 * 开启人脸引导检测模式。
 * 仅积累人脸姿态提示，不重置测量状态；引导/倒计时阶段可调用。
 * 正式测量前仍需调用 `start`。
 */
- (void)beginFaceGuidance;

/**
 * 开始正式测量。
 * 重置内部状态、确保 SignalR 连接，之后方可 enqueue 送帧。
 * 调用前建议先通过 XYFaceDetector 完成人脸引导与二次确认。
 */
- (void)start;

/// 当前是否满足开始测量的前置条件
- (BOOL)canStart;

/**
 * 送入一帧相机视频（使用屏幕尺寸作为 previewSize）。
 * @param sampleBuffer 相机输出的 CMSampleBuffer（NV12）
 * @param orientation  帧方向（UIImageOrientation）
 * @param guideRect      人脸引导框，预览视图坐标系中的 CGRect
 */
- (void)enqueueSampleBuffer:(CMSampleBufferRef)sampleBuffer
                orientation:(UIImageOrientation)orientation
                  guideRect:(CGRect)guideRect;

/**
 * 送入一帧相机视频（指定 previewSize）。
 * @param previewSize 预览区域尺寸，须与 guideRect 处于同一坐标系
 */
- (void)enqueueSampleBuffer:(CMSampleBufferRef)sampleBuffer
                orientation:(UIImageOrientation)orientation
                  guideRect:(CGRect)guideRect
                previewSize:(CGSize)previewSize;

/**
 * 正常结束测量。
 * 采集达标后 SDK 会等待完整报告，期间保持 SignalR 连接。
 */
- (void)stop;

/// 主动断开 SignalR 连接（一般无需手动调用，stop 后会自行管理）
- (void)disconnectSignalR;

/**
 * 立即中断当前测量（异常场景）。
 * 与 stop 不同，interrupt 用于用户取消或错误恢复，会清理进行中的采集状态。
 */
- (void)interrupt;

@end

NS_ASSUME_NONNULL_END
