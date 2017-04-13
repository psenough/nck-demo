
/**
* NCK-DEMO SCENE
* https://github.com/nczeroshift/nck-demo
*/

#ifndef DS_CONFIG_H
#define DS_CONFIG_H

#include "nckVec2.h"
#include "nckColor4.h"
#include "nckWindow.h"
#include "nckGraphics.h"
#include "nckThread.h"
#include "nckGraphics.h"
#include "nckFont.h"
#include "nckWidget.h"
#include "nckUtils.h"
#include "nckException.h"
#include "nckMathUtils.h"
#include "nckCompound.h"
#include "nckQueueBuffer.h"
#include "nckAudioDevice.h"
#include "nckVec3.h"
#include "nckMat44.h"
#include "nckTimeline.h"
#include "nckGeometry.h"
#include "nckModel.h"
#include "nckHttpServer.h"
#include "bxonDataIO.h"
#include "nckTransform.h"
#include "nckMarchingCubes.h"

#include <stdint.h>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#define TWOPI	(2.0*M_PI)

#define REFERENCE_HEIGHT 720
#define REFERENCE_SUBTITLE_SIZE  (REFERENCE_HEIGHT/14)
#define REFERENCE_DURATION 30e6 // 30s

#define _DS_BEGIN namespace DS{
#define _DS_END }

#endif