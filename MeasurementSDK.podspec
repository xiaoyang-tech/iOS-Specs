Pod::Spec.new do |s|
  s.name = 'MeasurementSDK'
  s.version = '1.0.0'
  s.summary = 'Private binary iOS measurement SDK.'
  s.description = 'A private binary iOS SDK distributed through CocoaPods with its runtime dependencies and model resources.'
  s.homepage = 'https://codeup.aliyun.com/xytech/measurement/docs/blob/master/docs/sdk/ios.md'
  s.license = { :type => 'Private', :text => 'Copyright. All rights reserved.' }
  s.author = { 'MeasurementSDK' => 'https://codeup.aliyun.com/xytech/measurement/docs/blob/master/docs/sdk/ios.md' }


  s.source = { :git => 'https://github.com/xiaoyang-tech/iOS-Specs.git', :tag => "v#{s.version}" }

  s.platform = :ios, '14.0'
  s.requires_arc = true
  s.static_framework = true

  s.vendored_frameworks = [
    'MeasurementSDK.xcframework',
    'libmeasurement.xcframework'
  ]
  
  s.resources = [
    'measurementSDK.bundle'
  ]

  s.preserve_paths = [
    'MeasurementSDK.xcframework',
    'libmeasurement.xcframework',
    'measurementSDK.bundle',
    'Support/MeasurementSDK-Bridging-Header.h',
    'Support/MeasurementSDK-SwiftBridge.h'
  ]

  s.frameworks = [
    'Foundation',
    'UIKit',
    'CoreImage',
    'CoreVideo',
    'CoreMedia',
    'CoreMotion',
    'CoreGraphics',
    'AVFoundation',
    'QuartzCore',
    'ImageIO'
  ]
  s.libraries = 'c++'

  s.dependency 'MediaPipeTasksVision', '~> 0.10.14'
  s.dependency 'Protobuf'
  # s.dependency 'OpenCV'


    # CocoaPods 默认只指向 .xcframework 外层，SourceKit / Bridging Header 需要 slice 内 .framework 路径
  framework_slices = [
    '$(PODS_ROOT)/MeasurementSDK/MeasurementSDK.xcframework/ios-arm64',
    '$(PODS_ROOT)/MeasurementSDK/MeasurementSDK.xcframework/ios-arm64_x86_64-simulator',
    '$(PODS_ROOT)/MeasurementSDK/libmeasurement.xcframework/ios-arm64',
    '$(PODS_ROOT)/MeasurementSDK/libmeasurement.xcframework/ios-arm64-simulator',
  ].map { |p| "\"#{p}\"" }.join(' ')

  header_paths = [
    '$(PODS_ROOT)/MeasurementSDK/MeasurementSDK.xcframework/ios-arm64/MeasurementSDK.framework/Headers',
    '$(PODS_ROOT)/MeasurementSDK/MeasurementSDK.xcframework/ios-arm64_x86_64-simulator/MeasurementSDK.framework/Headers',
  ].map { |p| "\"#{p}\"" }.join(' ')

  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
  }

  s.user_target_xcconfig = {
    'FRAMEWORK_SEARCH_PATHS' => "$(inherited) #{framework_slices}",
    'HEADER_SEARCH_PATHS' => "$(inherited) #{header_paths}",
    'GCC_PREPROCESSOR_DEFINITIONS' => '$(inherited) GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS=1',
    'SWIFT_ENABLE_EXPLICIT_MODULES' => 'NO',
    'OTHER_SWIFT_FLAGS' => '$(inherited) -auto-bridging-header-chaining',
  }

end
