//
//  FeatureRaw.h
//  Pods
//
//  Created by DCK on 2025/12/15.
//

#ifndef FeatureRaw_h
#define FeatureRaw_h

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define TRIANGLE_COUNT 100
#define REGION_COUNT 5
#define COLOR_FEATURE_COUNT (REGION_COUNT + TRIANGLE_COUNT)
#define BLENDSHAPE_COUNT 52

typedef struct {
    double *blue;
    double *green;
    double *red;
    double  *blendshape;
} FeatureRaw;

#ifdef __cplusplus
}
#endif


#endif /* FeatureRaw_h */
