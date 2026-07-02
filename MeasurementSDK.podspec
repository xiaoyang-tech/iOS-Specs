Pod::Spec.new do |s|
  s.name = 'MeasurementSDK'
  s.version = '1.0.1'
  s.summary = 'Private binary iOS measurement SDK.'
  s.description = 'A private binary iOS SDK distributed through CocoaPods with its runtime dependencies and model resources.'
  s.homepage = 'https://example.com/MeasurementSDK'
  s.license = { :type => 'Private', :text => 'Copyright. All rights reserved.' }
  s.author = { 'MeasurementSDK' => 'support@example.com' }


  s.source = { :git => 'https://github.com/xiaoyang-tech/iOS-Specs.git', :tag => 'v1.0.1' }

  s.platform = :ios, '14.0'
  s.requires_arc = true
  # s.static_framework = true

  s.vendored_frameworks = [
    'MeasurementSDK.framework',
    'libmeasurement.framework'
  ]
  s.resources = [
    'measurementSDK.bundle'
  ]
  s.preserve_paths = [
    'MeasurementSDK.framework',
    'libmeasurement.framework',
    'measurementSDK.bundle'
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


end
