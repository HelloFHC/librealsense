// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2024 Intel Corporation. All Rights Reserved.
#pragma once

#include <rsutils/number/float3.h>

#define _USE_MATH_DEFINES
#include <math.h>


inline rsutils::number::float3x3 rotx( float a )
{
    double rad = a * M_PI / 180.;
    return { 
        { 1,      0,               0           },
        { 0, float(cos(rad)), float(-sin(rad)) },
        { 0, float(sin(rad)), float(cos(rad))  }
    };
}




