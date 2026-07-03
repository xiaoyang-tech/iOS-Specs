# MeasurementSDK

Private binary iOS SDK package for `MeasurementSDK.framework`.

## Contents

- `MeasurementSDK.framework`: private binary framework
- `measurementSDK.bundle`: SDK resource bundle
- `MeasurementSDK.podspec`: CocoaPods private pod specification

The SDK source code is not included in this repository. Customers receive the
compiled framework plus the podspec metadata needed to install runtime
dependencies.

## CocoaPods

After this repository is pushed to a private Git remote, update the `s.source`
URL in `MeasurementSDK.podspec`, tag the release, then reference the pod from an
app:


pod 'MeasurementSDK', :git => 'https://github.com/xiaoyang-tech/iOS-Specs.git', :tag => 'v1.0.0'

The podspec automatically installs:

- `measurementSDK.framework`
- `measurementSDK.bundle`
- `MediaPipeTasksVision`
- `Protobuf`

Customers do not need to add `MediaPipeTasksVision`, `Protobuf`, or
`face_landmarker.task` manually.

## Development

Validate the podspec with:

```sh
pod lib lint MeasurementSDK.podspec --quick --allow-warnings --skip-import-validation --skip-tests
```
