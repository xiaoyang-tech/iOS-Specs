# 小阳心健康测量 iOS SDK

## 简介

小阳心健康测量 SDK 是一套基于摄像头视频流的人脸生理信号测量方案，通过实时分析面部特征及视频信号，可生成健康测量报告。

SDK 采用二进制 XCFramework 形式发布，通过 CocoaPods 集成，内部已集成：

- MediaPipe Face Landmarker
- Protobuf 3.29.6
- 模型资源文件（`measurementSDK.bundle`）
- 测量通信模块（SignalR）

业务方仅需接入摄像头数据并实现回调接口即可完成测量。

---

## 系统要求

| 项目 | 要求 |
| --- | --- |
| iOS | 14.0+ |
| Xcode | 15.0+ |
| Swift | 5.8+（Swift 宿主可选） |
| 架构 | arm64 真机 / arm64 + x86_64 模拟器 |

---

## 集成方式

### 1. 配置 Podfile

```ruby
platform :ios, '14.0'

target 'YourApp' do
  use_frameworks!

  pod 'MeasurementSDK',
      :git => 'https://github.com/xiaoyang-tech/iOS-Specs.git',
      :tag => 'v1.0.1'
end
```

> Pod 会自动安装 `MediaPipeTasksVision`、`Protobuf` 及 SDK 资源，业务方无需手动添加。

### 2. 执行安装

```bash
pod install
```

### 3. 权限配置

在 `Info.plist` 中添加相机权限说明：

```xml
<key>NSCameraUsageDescription</key>
<string>需要使用摄像头进行健康测量</string>
```

### 4. 导入 SDK

#### Objective-C

在需要使用 SDK 的 `.m` 文件中：

```objc
#import <MeasurementSDK/MeasurementSDK.h>
```

#### Swift

**请勿使用** `import MeasurementSDK`。

Swift 宿主通过 **Bridging Header** 引入 OC 公开 API。在 App 的 Bridging Header（如 `YourApp-Bridging-Header.h`）中添加：

```objc
#import "MeasurementSDK-SwiftBridge.h"
```

> `MeasurementSDK-SwiftBridge.h` 由 Pod 随 `Support/` 目录下发，CocoaPods 安装后位于 `Pods/MeasurementSDK/Support/`。  
> 若 Xcode 提示找不到头文件，可在 Target → Build Settings → **Header Search Paths** 中添加：
>
> ```
> $(PODS_ROOT)/MeasurementSDK/Support
> ```

Bridging Header 已包含以下公开类型，Swift 中可直接使用：

- `XYMeasurementManager` / `XYMeasurementConfig` / `XYMeasurementDelegate`
- `XYFaceDetector` / `XYLogConfig`
- `Report` / `HrReport` / `MeasurementCategory`

---

## SDK 初始化

### 创建配置对象

#### Swift

```swift
let config = XYMeasurementConfig(appId: "YOUR_APP_ID", sdkKey: "YOUR_SDK_KEY")
config.measurementDuration = 30
config.measurementCategoryValues = [NSNumber(value: MeasurementCategory_All.rawValue)]
config.enableLog = true
```

#### Objective-C

```objc
XYMeasurementConfig *config = [[XYMeasurementConfig alloc] initWithAppId:@"YOUR_APP_ID"
                                                                  sdkKey:@"YOUR_SDK_KEY"];
config.measurementDuration = 30;
config.measurementCategoryValues = @[@(MeasurementCategory_All)];
config.enableLog = YES;
```

### 初始化 SDK 实例

#### Swift

```swift
let measureSDK = XYMeasurementManager()
measureSDK.delegate = self
measureSDK.setConfig(config)
```

#### Objective-C

```objc
XYMeasurementManager *measureSDK = [[XYMeasurementManager alloc] init];
measureSDK.delegate = self;
[measureSDK setConfig:config];
```

---

## XYMeasurementConfig 配置项

| 属性 | 类型 | 说明 |
| --- | --- | --- |
| appId | String | 平台分配的 APP_ID（必填） |
| sdkKey | String | 平台分配的 SDK_KEY（必填） |
| measurementDuration | Int | 测量总时长（秒），默认 30 |
| minChunkTimeSpan | Int | 阶段性 partial 报告最小间隔（秒），默认 6 |
| measurementUrl | String? | 私有化测量服务地址 |
| authUrl | String? | 私有化认证地址 |
| measurementCategoryValues | [NSNumber] | 测量指标 rawValue 数组 |
| enableLog | Bool | 是否打印 SDK 日志，默认 YES |

### 测量类型枚举（MeasurementCategory）

```objc
typedef GPB_ENUM(MeasurementCategory) {
  MeasurementCategory_None = 0,
  MeasurementCategory_HeartRate = 1,        // 心率房颤
  MeasurementCategory_BloodPressure = 2,    // 血压
  MeasurementCategory_BloodOxygen = 4,      // 血氧
  MeasurementCategory_Risk = 8,             // 健康风险
  MeasurementCategory_Aggressivity = 16,    // 攻击性
  MeasurementCategory_Anxiety = 32,         // 焦虑度
  MeasurementCategory_Depression = 64,      // 压抑度
  MeasurementCategory_Vitality = 128,       // 活力度
  MeasurementCategory_Positivity = 256,     // 积极性
  MeasurementCategory_Suppression = 512,    // 抑郁度
  MeasurementCategory_Fatigue = 1024,       // 疲劳度
  MeasurementCategory_Physiology = 15,      // 生理测量组合
  MeasurementCategory_Emotion = 1713,       // 情绪测量组合
  MeasurementCategory_All = 1727,           // 全部指标
};
```

### 配置示例

#### Swift

```swift
let config = XYMeasurementConfig(appId: "YOUR_APP_ID", sdkKey: "YOUR_SDK_KEY")
config.measurementDuration = 15
config.measurementCategoryValues = [NSNumber(value: MeasurementCategory_All.rawValue)]
measureSDK.setConfig(config)
```

#### Objective-C

```objc
XYMeasurementConfig *config = [[XYMeasurementConfig alloc] initWithAppId:@"YOUR_APP_ID"
                                                                  sdkKey:@"YOUR_SDK_KEY"];
config.measurementDuration = 15;
config.measurementCategoryValues = @[@(MeasurementCategory_All)];
[measureSDK setConfig:config];
```

---

## 测量流程

推荐接入顺序：

```
setConfig → 人脸引导（XYFaceDetector）→ beginFaceGuidance（可选）→ start → 持续送帧 → stop
```

1. **setConfig:** 配置鉴权参数，SDK 建立 SignalR 连接
2. **人脸引导:** 使用 `XYFaceDetector` 校验人脸位置与姿态
3. **start:** 开始正式测量
4. **enqueueSampleBuffer:** 每帧相机数据送入 SDK
5. **stop / interrupt:** 结束或中断测量

### 人脸引导（测量前）

引导阶段使用 `XYFaceDetector` 单例，正式测量阶段改用 `XYMeasurementManager` 送帧。

#### Swift

```swift
let faceDetector = XYFaceDetector.shared
let timestamp = Int(Date().timeIntervalSince1970 * 1000)

faceDetector.detectAsync(
    with: sampleBuffer,
    orientation: orientation,
    timestamp: timestamp,
    guideFrame: guideRect
) { isValid, code, message in
    self.tipsLabel.text = message
    if isValid {
        // 累计足够有效帧后，调用 measureSDK.start()
    }
}
```

#### Objective-C

```objc
NSInteger timestamp = (NSInteger)([[NSDate date] timeIntervalSince1970] * 1000);
[[XYFaceDetector shared] detectAsyncWithSampleBuffer:sampleBuffer
                                         orientation:orientation
                                           timestamp:timestamp
                                          guideFrame:guideRect
                                          completion:^(BOOL isValid, NSInteger code, NSString *message) {
    self.tipsLabel.text = message;
    if (isValid) {
        // 累计足够有效帧后，调用 [measureSDK start]
    }
}];
```

---

## 核心功能调用

### 开始测量

#### Swift

```swift
measureSDK.beginFaceGuidance()  // 可选：开启引导模式
measureSDK.start()
```

#### Objective-C

```objc
[measureSDK beginFaceGuidance];  // 可选
[measureSDK start];
```

### 视频流输入

相机输出格式须为 **NV12**（`kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange`）。

#### Swift

```swift
measureSDK.enqueueSampleBuffer(
    sampleBuffer,
    orientation: orientation,
    guideRect: guideRect
)
```

#### Objective-C

```objc
[measureSDK enqueueSampleBuffer:sampleBuffer
                    orientation:orientation
                      guideRect:guideRect];
```

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| sampleBuffer | CMSampleBuffer | 摄像头采集的视频帧（NV12） |
| orientation | UIImageOrientation | 视频方向 |
| guideRect | CGRect | 测量引导框（预览视图坐标系） |
| previewSize | CGSize | 可选，预览区域尺寸，须与 guideRect 同一坐标系 |

#### 完整示例（Swift）

```swift
func captureOutput(
    _ output: AVCaptureOutput,
    didOutput sampleBuffer: CMSampleBuffer,
    from connection: AVCaptureConnection
) {
    let orientation: UIImage.Orientation = .leftMirrored
    measureSDK.enqueueSampleBuffer(
        sampleBuffer,
        orientation: orientation,
        guideRect: scanningFrame
    )
}
```

### 停止测量

#### Swift

```swift
measureSDK.stop()       // 正常结束，等待完整报告
measureSDK.interrupt()  // 立即中断（用户取消 / 异常恢复）
```

#### Objective-C

```objc
[measureSDK stop];
[measureSDK interrupt];
```

---

## 回调协议（XYMeasurementDelegate）

所有回调方法均为 `@optional`，按需实现即可。

### Swift

```swift
extension ViewController: XYMeasurementDelegate {

    func onStarted(withMeasurementId measurementId: String) {
        print("测量任务 ID：\(measurementId)")
    }

    func onStateUpdate(withCode code: Int, message: String) {
        tipsLabel.text = message
    }

    func onInterrupted(withReason reason: String) {
        print("中断原因：\(reason)")
    }

    func onCollected() {
        print("采集完成，等待报告")
    }

    func onChunkReportGenerated(withMeasurementId measurementId: String, hrReport: HrReport) {
        let hr = Int(hrReport.data_p.hrBpm)
        heartRateLabel.text = "\(hr)"
    }

    func onWholeReportGenerated(withMeasurementId measurementId: String, report: Report) {
        print("测量完成")
    }

    func onCrashed(withCode code: Int, message: String) {
        print("异常：\(code) - \(message)")
    }
}
```

### Objective-C

```objc
@interface ViewController () <XYMeasurementDelegate>
@end

@implementation ViewController

- (void)onStartedWithMeasurementId:(NSString *)measurementId {
    NSLog(@"测量任务 ID：%@", measurementId);
}

- (void)onStateUpdateWithCode:(NSInteger)code message:(NSString *)message {
    self.tipsLabel.text = message;
}

- (void)onInterruptedWithReason:(NSString *)reason {
    NSLog(@"中断原因：%@", reason);
}

- (void)onCollected {
    NSLog(@"采集完成，等待报告");
}

- (void)onChunkReportGeneratedWithMeasurementId:(NSString *)measurementId
                                       hrReport:(HrReport *)hrReport {
    self.heartRateLabel.text = [NSString stringWithFormat:@"%d", hrReport.data_p.hrBpm];
}

- (void)onWholeReportGeneratedWithMeasurementId:(NSString *)measurementId
                                         report:(Report *)report {
    NSLog(@"测量完成");
}

- (void)onCrashedWithCode:(NSInteger)code message:(NSString *)message {
    NSLog(@"异常：%ld - %@", (long)code, message);
}

@end
```

### 回调说明

| 回调 | 触发时机 |
| --- | --- |
| onStarted | 测量成功启动，返回服务端 measurementId |
| onStateUpdate | 状态或人脸引导提示更新（如「请正视屏幕」「请靠近一些」） |
| onInterrupted | 测量被中断（人脸丢失、姿态异常、用户取消等） |
| onCollected | 视频帧采集完成，SDK 等待完整报告 |
| onChunkReportGenerated | 阶段性心率报告（partial），可实时更新 UI |
| onWholeReportGenerated | 完整健康报告生成 |
| onCrashed | 不可恢复错误（鉴权失败、网络异常等） |

---

## 完整接入示例

### Swift

```swift
import UIKit
import AVFoundation

class ViewController: UIViewController {

    let measureSDK = XYMeasurementManager()
    let scanningFrame = CGRect(x: 40, y: 120, width: 295, height: 400)

    override func viewDidLoad() {
        super.viewDidLoad()
        measureSDK.delegate = self

        let config = XYMeasurementConfig(appId: "YOUR_APP_ID", sdkKey: "YOUR_SDK_KEY")
        config.measurementCategoryValues = [NSNumber(value: MeasurementCategory_All.rawValue)]
        measureSDK.setConfig(config)
    }

    func startMeasure() {
        measureSDK.start()
    }

    func captureOutput(
        _ output: AVCaptureOutput,
        didOutput sampleBuffer: CMSampleBuffer,
        from connection: AVCaptureConnection
    ) {
        measureSDK.enqueueSampleBuffer(
            sampleBuffer,
            orientation: .leftMirrored,
            guideRect: scanningFrame
        )
    }
}

extension ViewController: XYMeasurementDelegate {
    func onStarted(withMeasurementId measurementId: String) { }
    func onStateUpdate(withCode code: Int, message: String) { }
    func onInterrupted(withReason reason: String) { }
    func onCollected() { }
    func onChunkReportGenerated(withMeasurementId measurementId: String, hrReport: HrReport) { }
    func onWholeReportGenerated(withMeasurementId measurementId: String, report: Report) { }
    func onCrashed(withCode code: Int, message: String) { }
}
```

### Objective-C

```objc
#import <MeasurementSDK/MeasurementSDK.h>

@interface ViewController () <XYMeasurementDelegate>
@property (nonatomic, strong) XYMeasurementManager *measureSDK;
@property (nonatomic, assign) CGRect scanningFrame;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.scanningFrame = CGRectMake(40, 120, 295, 400);
    self.measureSDK = [[XYMeasurementManager alloc] init];
    self.measureSDK.delegate = self;

    XYMeasurementConfig *config = [[XYMeasurementConfig alloc] initWithAppId:@"YOUR_APP_ID"
                                                                      sdkKey:@"YOUR_SDK_KEY"];
    config.measurementCategoryValues = @[@(MeasurementCategory_All)];
    [self.measureSDK setConfig:config];
}

- (void)startMeasure {
    [self.measureSDK start];
}

- (void)captureOutput:(AVCaptureOutput *)output
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection {
    [self.measureSDK enqueueSampleBuffer:sampleBuffer
                             orientation:UIImageOrientationLeftMirrored
                               guideRect:self.scanningFrame];
}

@end
```

---

## 注意事项

1. **Swift 宿主请勿** `import MeasurementSDK`，统一通过 Bridging Header 引入
2. 面部需完整出现在引导框内，保持环境光线充足均匀
3. 避免剧烈运动与头部晃动，不建议佩戴口罩测量
4. 测量中不要切换前后台，必须持续传入视频帧
5. 相机输出格式须为 NV12，引导框与 previewSize 须处于同一坐标系
6. 正式测量前建议先通过 `XYFaceDetector` 完成人脸引导与二次确认

---

## 开发验证

```bash
pod spec lint MeasurementSDK.podspec --quick --allow-warnings
```

---

## 联系支持

如需技术支持、私有化部署或商务合作，请联系小阳心健康技术团队。
