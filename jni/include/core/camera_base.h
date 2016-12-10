/******************************************************************************
 * (C) Copyright [2016 - 2017] ASR International Ltd.
 * All Rights Reserved
******************************************************************************/


#ifndef _CAMERA_CORE_BASE_H_
#define _CAMERA_CORE_BASE_H_


#include <stdint.h>


#ifdef __cplusplus
    extern "C" {
#endif /* extern "C" */


/* ------------------------------------------------------------------------ */


typedef enum {
    CC_ERROR_NONE,
    CC_ERROR_BADPOINTER,        // the input pointer is bad( NULL )
    CC_ERROR_BADARGUMENT,       // the input argument is bad
    CC_ERROR_OUTOFMEMORY,       // memory allocation failed
    CC_ERROR_FATALERROR,        // when fatal happened, camera Core must be set back to idle to clear this error.
                                // FATAL error may be caused by an unrecoverable device failure.
    CC_ERROR_NOTFOUND,          // not found in list/queue
    CC_ERROR_HANDLEDESTROYED,   // handle has been destroyed

    CC_ERROR_ISPRECOVER,    //ISP device error, need to recover
    CC_ERROR_OFFLINELOCK,   //port source is using for offline
    CC_ERROR_ONLINELOCK,    //port source is using for online
    CC_ERROR_POLLTIMEOUT,    //poll timeout
} CCError;


typedef enum {
    CC_IMAGEFORMAT_NULL = 0,
    CC_IMAGEFORMAT_NV12,
    CC_IMAGEFORMAT_YUV422,
    CC_IMAGEFORMAT_JPEG,
    CC_IMAGEFORMAT_RAW,
    CC_IMAGEFORMAT_AFBC,
    CC_IMAGEFORMAT_AFBC_COMPACT,
} CCImageFormat;


typedef struct {
    uint32_t width;
    uint32_t height;
} CCSize;



typedef struct {
    CCImageFormat format;
    CCSize size;
} CCImageInfo;


typedef enum {
    CC_STREAM_INTERNAL = 0x0,
    CC_STREAM_PREVIEW = 0x1,
    CC_STREAM_VIDEO = 0x2,
    CC_STREAM_CALLBACK = 0x4,
    CC_STREAM_SNAPSHOT = 0x8,
    CC_STREAM_RAW = 0x10,
    CC_STREAM_THUMBNAIL = 0x20,
    CC_STREAM_INPUT = 0x40,
} CCStreamId;

/* ------------------------------------------------------------------------ */
// params

/*********************************************************************
 * Camera Core Enum *
 *********************************************************************/

typedef enum
{
    CC_TORCH_OFF = 0x0,
    CC_TORCH_ON = 0x1,
} CCTorchMode;

// Events
typedef enum
{
    CC_EVENT_LIMIT = 0x7fffffff,
} CCEventId;

typedef enum
{
    CC_STREAM_INACTIVE = 0x0,
    CC_STREAM_ACTIVE = 0x1,
} CCStreamState;


/**
 * Enumeration definitions for the various entries that need them
 */
typedef enum
{
    CC_FACE_DETECT_OFF = 0x0,
    CC_FACE_DETECT_SIMPLE,
    CC_FACE_DETECT_FULL,

    CC_FACE_DETECT_NUM,

    CC_FACE_DETECT_LIMIT = 0x7fffffff,
} CCFaceDetectMode;

typedef enum
{
    CC_CHROMATIC_ABERRATION_MODE_OFF = 0x0,
    CC_CHROMATIC_ABERRATION_MODE_FAST,
    CC_CHROMATIC_ABERRATION_MODE_HIGH_QUALITY,

    CC_CHROMATIC_ABERRATION_MODE_NUM,

    CC_CHROMATIC_ABERRATION_MODE_LIMIT = 0x7fffffff,
} CCChromaticAberrationMode;

typedef enum
{
    CC_CONTROL_MODE_OFF = 0x0,
    CC_CONTROL_MODE_AUTO,
    CC_CONTROL_MODE_USE_SCENE_MODE,
    CC_CONTROL_MODE_OFF_KEEP_STATE,

    CC_CONTROL_MODE_NUM,

    CC_CONTROL_MODE_LIMIT = 0x7fffffff,
} CCControl3AMode;

typedef enum
{
    CC_AE_ANTIBANDING_MODE_OFF = 0x0,
    CC_AE_ANTIBANDING_MODE_50HZ,
    CC_AE_ANTIBANDING_MODE_60HZ,
    CC_AE_ANTIBANDING_MODE_AUTO,

    CC_AE_ANTIBANDING_MODE_NUM,

    CC_AE_ANTIBANDING_MODE_LIMIT = 0x7fffffff,
} CCAeAntibandingMode;

typedef enum
{
    CC_AE_MODE_OFF = 0x0,
    CC_AE_MODE_ON,
    CC_AE_MODE_ON_AUTO_FLASH,
    CC_AE_MODE_ON_ALWAYS_FLASH,
    CC_AE_MODE_ON_AUTO_FLASH_REDEYE,

    CC_AE_MODE_NUM,

    CC_AE_MODE_LIMIT = 0x7fffffff,
} CCAeMode;

typedef enum
{
    CC_AF_MODE_OFF = 0x0,
    CC_AF_MODE_AUTO,
    CC_AF_MODE_MACRO,
    CC_AF_MODE_CONTINUOUS_VIDEO,
    CC_AF_MODE_CONTINUOUS_PICTURE,
    CC_AF_MODE_EDOF,

    CC_AF_MODE_NUM,

    CC_AF_MODE_LIMIT = 0x7fffffff,
} CCAfMode;

typedef enum
{
    CC_EFFECT_MODE_OFF = 0x0,
    CC_EFFECT_MODE_MONO,
    CC_EFFECT_MODE_NEGATIVE,
    CC_EFFECT_MODE_SOLARIZE,
    CC_EFFECT_MODE_SEPIA,
    CC_EFFECT_MODE_POSTERIZE,
    CC_EFFECT_MODE_WHITEBOARD,
    CC_EFFECT_MODE_BLACKBOARD,
    CC_EFFECT_MODE_AQUA,

    CC_EFFECT_MODE_NUM,

    CC_EFFECT_MODE_LIMIT = 0x7fffffff,
} CCEffectMode;

typedef enum
{
    CC_SCENE_MODE_DISABLE = 0x0,
    CC_SCENE_MODE_FACE_PRIORITY,
    CC_SCENE_MODE_ACTION,
    CC_SCENE_MODE_PORTRAIT,
    CC_SCENE_MODE_LANDSCAPE,
    CC_SCENE_MODE_NIGHT,
    CC_SCENE_MODE_NIGHT_PORTRAIT,
    CC_SCENE_MODE_THEATRE,
    CC_SCENE_MODE_BEACH,
    CC_SCENE_MODE_SNOW,
    CC_SCENE_MODE_SUNSET,
    CC_SCENE_MODE_STEADYPHOTO,
    CC_SCENE_MODE_FIREWORKS,
    CC_SCENE_MODE_SPORTS,
    CC_SCENE_MODE_PARTY,
    CC_SCENE_MODE_CANDLELIGHT,
    CC_SCENE_MODE_BARCODE,
    CC_SCENE_MODE_HIGH_SPEED_VIDEO,
    CC_SCENE_MODE_HDR,

    CC_SCENE_MODE_NUM,

    CC_SCENE_MODE_LIMIT = 0x7fffffff,
} CCSceneMode;

typedef enum
{
    CC_VIDEO_STABILIZATION_MODE_OFF = 0x0,
    CC_VIDEO_STABILIZATION_MODE_ON,

    CC_VIDEO_STABILIZATION_MODE_NUM,

    CC_VIDEO_STABILIZATION_MODE_LIMIT = 0x7fffffff,
} CCVideoStabilizationMode;

typedef enum
{
    CC_AWB_MODE_OFF = 0x0,
    CC_AWB_MODE_AUTO,
    CC_AWB_MODE_INCANDESCENT,
    CC_AWB_MODE_FLUORESCENT,
    CC_AWB_MODE_WARM_FLUORESCENT,
    CC_AWB_MODE_DAYLIGHT,
    CC_AWB_MODE_CLOUDY_DAYLIGHT,
    CC_AWB_MODE_TWILIGHT,
    CC_AWB_MODE_SHADE,

    CC_AWB_MODE_NUM,

    CC_AWB_MODE_LIMIT = 0x7fffffff,
} CCAwbMode;

typedef enum {
    CC_METERING_REGION_AE = 0x0,
    CC_METERING_REGION_AWB,
    CC_METERING_REGION_AF,

    CC_METERING_REGION_NUM,

    CC_METERING_REGION_LIMIT = 0x7fffffff,
} CCMeteringRegion;

typedef enum {
    CC_LENS_FACING_FRONT = 0x0,
    CC_LENS_FACING_BACK,
    CC_LENS_FACING_EXTERNAL,

    CC_LENS_FACING_NUM,

    CC_LENS_FACING_LIMIT = 0x7fffffff,
} CCLensFacing;

typedef enum {
    CC_NOISE_REDUCTION_OFF = 0x0,
    CC_NOISE_REDUCTION_FAST,
    CC_NOISE_REDUCTION_HIGH_QUALITY,
    CC_NOISE_REDUCTION_MINIMAL,
    CC_NOISE_REDUCTION_ZERO_SHUTTER_LAG,

    CC_NOISE_REDUCTION_NUM,

    CC_NOISE_REDUCTION_LIMIT = 0x7fffffff,
} CCNoiseReductionMode;

typedef enum {
    CC_SCALER_CROPPING_TYPE_CENTER_ONLY = 0x0,
    CC_SCALER_CROPPING_TYPE_FREEFORM,
} CCScalerCropType;

typedef enum {
    CC_SENSOR_TEST_PATTERN_OFF = 0x0,
    CC_SENSOR_TEST_PATTERN_SOLID_COLOR,
    CC_SENSOR_TEST_PATTERN_COLOR_BARS,
    CC_SENSOR_TEST_PATTERN_COLOR_BARS_FADE_TO_GRAY,
    CC_SENSOR_TEST_PATTERN_PN9,
    CC_SENSOR_TEST_PATTERN_CUSTOM1,

    CC_SCALER_CROPPING_TYPE_NUM,

    CC_SCALER_CROPPING_TYPE_LIMIT = 0x7fffffff,
} CCSensorTestPatternMode;

typedef enum {
    CC_SENSOR_TIMESTAMP_SOURCE_UNKNOWN = 0x0,
    CC_SENSOR_TIMESTAMP_SOURCE_REALTIME,
} CCSensorTimestampSource;

typedef enum {
    CC_LENS_OPTICAL_STABILIZATION_OFF = 0x0,
    CC_LENS_OPTICAL_STABILIZATION_ON,

    CC_LENS_OPTICAL_STABILIZATION_NUM,

    CC_LENS_OPTICAL_STABILIZATION_LIMIT = 0x7fffffff,
} CCLensOpticalStabilization;

typedef enum {
    CC_EDGE_ENHANCEMENT_MODE_OFF = 0x0,
    CC_EDGE_ENHANCEMENT_MODE_FAST,
    CC_EDGE_ENHANCEMENT_MODE_HIGH_QUALITY,
    CC_EDGE_ENHANCEMENT_MODE_ZERO_SHUTTER_LAG,

    CC_EDGE_ENHANCEMENT_MODE_NUM,

    CC_EDGE_ENHANCEMENT_MODE_LIMIT = 0x7fffffff,
} CCEdgeEnhancementMode;

typedef enum {
    CC_LENS_FOCUS_DISTANCE_CALIBRATION_MODE_UNCALIBRATED = 0x0,
    CC_LENS_FOCUS_DISTANCE_CALIBRATION_MODE_APPROXIMATE,
    CC_LENS_FOCUS_DISTANCE_CALIBRATION_MODE_CALIBRATED,
} CCLensFocusDistanceCalibraion;

typedef enum {
    CC_LENS_SHADING_OFF = 0x0,
    CC_LENS_SHADING_FAST,
    CC_LENS_SHADING_HIGH_QUALITY,

    CC_LENS_SHADING_NUM,

    CC_LENS_SHADING_LIMIT = 0x7fffffff,
} CCLensShadingMode;

typedef enum {
    CC_TONE_MAP_MODE_CONTRAST_CURVE = 0x0,
    CC_TONE_MAP_MODE_FAST,
    CC_TONE_MAP_MODE_HIGH_QUALITY,
    CC_TONE_MAP_MODE_GAMMA_VALUE,
    CC_TONE_MAP_MODE_PRESET_CURVE,

    CC_TONE_MAP_MODE_NUM,

    CC_TONE_MAP_MODE_LIMIT = 0x7fffffff,
} CCToneMapMode;

typedef enum {
    CC_HOT_PIXEL_CORRECTION_MODE_OFF = 0x0,
    CC_HOT_PIXEL_CORRECTION_MODE_FAST,
    CC_HOT_PIXEL_CORRECTION_MODE_HIGH_QUALITY,

    CC_HOT_PIXEL_CORRECTION_MODE_NUM,

    CC_HOT_PIXEL_CORRECTION_MODE_LIMIT = 0x7fffffff,
} CCHotPixelCorrectionMode;

typedef enum {
    CC_SENSOR_COLOR_FILTER_ARRANGEMENT_RGGB = 0x0,
    CC_SENSOR_COLOR_FILTER_ARRANGEMENT_GRBG,
    CC_SENSOR_COLOR_FILTER_ARRANGEMENT_GBRG,
    CC_SENSOR_COLOR_FILTER_ARRANGEMENT_BGGR,
    CC_SENSOR_COLOR_FILTER_ARRANGEMENT_RGB,
} CCSensorColorFilterArrangement;

typedef enum {
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_DAYLIGHT = 0x0,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_FLUORESCENT,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_TUNGSTEN,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_FLASH,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_FINE_WEATHER,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_CLOUDY_WEATHER,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_SHADE,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_DAYLIGHT_FLUORESCENT,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_DAY_WHITE_FLUORESCENT,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_COOL_WHITE_FLUORESCENT,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_WHITE_FLUORESCENT,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_STANDARD_A,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_STANDARD_B,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_STANDARD_C,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_D55,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_D65,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_D75,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_D50,
    CC_SENSOR_REFERENCE_ILLUMINANT_ARRANGEMENT_ISO_STUDIO_TUNGSTEN,
} CCSensorReferenceIlluminant;

typedef enum {
    CC_STATISTICS_HOT_PIXEL_MAP_MODE_OFF = 0x0,
    CC_STATISTICS_HOT_PIXEL_MAP_MODE_ON,

    CC_STATISTICS_HOT_PIXEL_MAP_MODE_NUM,

    CC_STATISTICS_HOT_PIXEL_MAP_MODE_LIMIT = 0x7fffffff,
} CCStatisticsHotPixelMapMode;

typedef enum {
    CC_STATISTICS_LENS_SHADING_MAP_MODE_OFF = 0x0,
    CC_STATISTICS_LENS_SHADING_MAP_MODE_ON,

    CC_STATISTICS_LENS_SHADING_MAP_MODE_NUM,

    CC_STATISTICS_LENS_SHADING_MAP_MODE_LIMIT = 0x7fffffff,
} CCStatisticsLensShadingMapMode;

typedef enum {
    CC_AF_TRIGGER_IDLE = 0x0,
    CC_AF_TRIGGER_START,
    CC_AF_TRIGGER_CANCEL,
} CCAfTrigger;

typedef enum {
    CC_AF_STATE_INACTIVE = 0x0,
    CC_AF_STATE_PASSIVE_SCAN,
    CC_AF_STATE_PASSIVE_FOCUSED,
    CC_AF_STATE_ACTIVE_SCAN,
    CC_AF_STATE_FOCUSED_LOCKED,
    CC_AF_STATE_NOT_FOCUSED_LOCKED,
    CC_AF_STATE_PASSIVE_UNFOCUSED,
} CCAfState;

typedef enum {
    CC_AWB_STATE_INACTIVE = 0x0,
    CC_AWB_STATE_SEARCHING,
    CC_AWB_STATE_CONVERGED,
    CC_AWB_STATE_LOCKED,
} CCAwbState;

typedef enum {
    CC_AE_PRECAPTURE_TRIGGER_IDLE = 0x0,
    CC_AE_PRECAPTURE_TRIGGER_START,
    CC_AE_PRECAPTURE_TRIGGER_CANCEL,
} CCAePrecaptureTrigger;

typedef enum {
    CC_FLASH_MODE_OFF = 0x0,
    CC_FLASH_MODE_SINGLE,
    CC_FLASH_MODE_TORCH,
} CCFlashMode;

typedef enum {
    CC_AE_STATE_INACTIVE = 0x0,
    CC_AE_STATE_SEARCHING,
    CC_AE_STATE_CONVERGED,
    CC_AE_STATE_LOCKED,
    CC_AE_STATE_FLASH_REQUIRED,
    CC_AE_STATE_PRECAPTURE,
} CCAeState;

typedef enum {
    CC_FLASH_STATE_UNAVAILABLE = 0x0,
    CC_FLASH_STATE_CHARGING,
    CC_FLASH_STATE_READY,
    CC_FLASH_STATE_FIRED,
    CC_FLASH_STATE_PARTIAL,
} CCFlashState;

typedef enum {
    CC_LENS_STATE_STATIONARY = 0x0,
    CC_LENS_STATE_MOVING,
} CCLensState;

typedef enum {
    CC_STATISTICS_SCENE_FLICKER_NONE = 0x0,
    CC_STATISTICS_SCENE_FLICKER_50HZ,
    CC_STATISTICS_SCENE_FLICKER_60HZ,
} CCStatisticsSceneFlicker;

typedef enum {
    CC_COLOR_CORRECTION_MODE_TRANSFORM_MATRIX = 0x0,
    CC_COLOR_CORRECTION_MODE_FAST,
    CC_COLOR_CORRECTION_MODE_HIGH_QUALITY,
} CCColorCorrectionMode;

typedef enum {
    CC_TONEMAP_PRESET_CURVE_SRGB = 0x0,
    CC_TONEMAP_PRESET_CURVE_REC709,
} CCTonemapPresetCurve;


/*********************************************************************
 * Camera Core Structure *
 *********************************************************************/
/**
 * Structure definitions for the various entries that need them
 */
typedef struct {
    int32_t numerator;
    int32_t denominator;
} CCRational;

typedef struct {
    float width;
    float height;
} CCSizeF;

typedef struct {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} CCRect;

typedef struct {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    int32_t meteringWeight;
} CCMeteringRect;

typedef struct {
    int32_t faceIds;
    int32_t faceLandmarks;
    int32_t faceRectangles;
    int32_t faceScores;
} CCFaceInfo;

typedef struct {
    float read;
    float greenEven;
    float greenOdd;
    float blue;
} CCGainMatrix;

typedef struct {
    int32_t x;
    int32_t y;
} CCPoint;
/**************************************/


typedef struct {
   //BC capability
    int32_t supportedControlModeCnt;
    CCControl3AMode supportedControlMode[CC_CONTROL_MODE_NUM]; //supported 3A control mode

    int32_t supportedAeAntibandingModeCnt;
    CCAeAntibandingMode supportedAeAntibandingMode[CC_AE_ANTIBANDING_MODE_NUM]; //supported ae anti-banding mode

    int32_t supportedAeModeCnt;
    CCAeMode supportedAeMode[CC_AE_MODE_NUM]; //supported ae mode

    int32_t minFpsForMaxYuvSize; //supported FPS range
    int32_t maxFpsForMaxYuvSize;

    CCRational aeCompensationStep; //the smallest of exposure compensation

    int32_t minAeCompensationValue; //supported exposuer compensation values
    int32_t maxAeCompensationValue;

    int32_t aeLockAvailable; //whether device support ae lock, >=Andorid6.0 available

    int32_t supportedAfModeCnt;
    CCAfMode supportedAfMode[CC_AF_MODE_NUM]; //supported af mode

    int32_t supportedEffectModeCnt;
    CCEffectMode supportedEffectMode[CC_EFFECT_MODE_NUM]; //supported effect mode

    int32_t supportedSceneModeCnt;
    CCSceneMode supportedSceneMode[CC_SCENE_MODE_NUM]; //supported scene mode

    int32_t supportedVideoStabilizationModeCnt;
    CCVideoStabilizationMode supportedVideoStabMode[CC_VIDEO_STABILIZATION_MODE_NUM]; //supported video stabilization mode

    int32_t supportedAwbModeCnt;
    CCAwbMode supportedAwbMode[CC_AWB_MODE_NUM]; //supported awb mode

    int32_t awbLockAvailable; //whether device support awb lock, >=Andorid6.0 available

    int32_t supportedMaxMeteringRegions[CC_METERING_REGION_NUM]; //The maximum number of metering regions that can be used by AE/AWB/AF

    int32_t flashInfoAvailable; //Whether this camera device has a flash unit
} CCControlCapability;

typedef struct {
    //BC capability
    int32_t supportedChromaticAberrationModeCnt;
    CCChromaticAberrationMode supportedChromaticAberrationMode[CC_CHROMATIC_ABERRATION_MODE_NUM]; //supported chromatic aberration correction algorithm mode

    int32_t supportedNoiseReductionModeCnt;
    CCNoiseReductionMode supportedNoiseReductionMode[CC_NOISE_REDUCTION_NUM]; //supported noise reduction

    float supportedMaxDigitalZoom; //supported digital zoom

    CCScalerCropType supportedScalerCropType; //supported scaler cropping type

    int32_t supportedEdgeEnhancementModeCnt;
    CCEdgeEnhancementMode supportedEdgeEnhancementMode[CC_EDGE_ENHANCEMENT_MODE_NUM]; //supported edge enhancement mode

    //MANUAL_POSTPROC capability
    int32_t supportedShadingModeCnt;
    CCLensShadingMode supportedShadingMode[CC_LENS_SHADING_NUM]; //supported lens shading mode

    int32_t supportedToneMapModeCnt;
    CCToneMapMode supportedToneMapMode[CC_TONE_MAP_MODE_NUM]; //supported tone mapping mode

    int32_t toneMapMaxCurvePoints; //Maximum number of supported points

    //RAW capability
    int32_t supportedHotPixelModeCnt;
    CCHotPixelCorrectionMode supportedHotPixelMode[CC_HOT_PIXEL_CORRECTION_MODE_NUM]; //List of hot pixel correction modes

    int32_t minPostRawSensitivityBoost;
    int32_t maxPostRawSensitivityBoost; //Range of boosts for controling post RAW sensitivity boost
} CCPostProcessCapability;

typedef struct {
    //BC capability
    int32_t supportedSensorTestPatternModeCnt;
    CCSensorTestPatternMode supportedSensorTestPatternMode[CC_SCALER_CROPPING_TYPE_NUM]; //supported sensor test pattern mode

    CCRect sensorActiveArraySize; //The area of the image sensor which corresponds to active pixels

    CCSizeF sensorPhysicalSize; //The physical dimensions of the full pixel array

    CCSize sensorPixelArraySize; //Dimensions of the full pixel array

    CCSensorTimestampSource sensorTimestampSource; //The time base source for sensor capture start timestamps

    //MANUAL_SENSOR capability
    int32_t sensorWhiteLevel; //Maximum raw value output by sensor

    int32_t sensorBlackLevelPatternCnt;
    int32_t sensorBlackLevelPattern[10]; //A fixed black level offset for each of the color filter arrangement (CFA) mosaic channels

    int64_t minSensorExposureTime;
    int64_t maxSensorExposureTime; //The range of image exposure times

    int64_t sensorMaxFrameDuration; //The maximum possible frame duration

    int32_t minSensorSensitivity;
    int32_t maxSensorSensitivity; //Range of sensitivities

    int32_t sensorMaxAnalogSensitivity; //Maximum sensitivity

    //RAW capability
    CCRational sensorCalibrationTransform1[3][3]; //A per-device calibration transform matrix

    CCRational sensorCalibrationTransform2[3][3]; //A per-device calibration transform matrix

    CCRational sensorColorTransform1[3][3]; //A matrix that transforms color values from CIE XYZ color space to reference sensor color space

    CCRational sensorColorTransform2[3][3]; //A matrix that transforms color values from CIE XYZ color space to reference sensor color space

    CCRational sensorForwardMatrix1[3][3]; //A matrix that transforms white balanced camera colors from the reference sensor colorspace to the CIE XYZ colorspace with a D50 whitepoint

    CCRational sensorForwardMatrix2[3][3]; //A matrix that transforms white balanced camera colors from the reference sensor colorspace to the CIE XYZ colorspace with a D50 whitepoint

    CCSensorColorFilterArrangement sensorColortFilterArrange; //The arrangement of color filters on sensor

    CCSensorReferenceIlluminant sensorReferenceIlluminant1; //The standard reference illuminant used as the scene light source

    CCSensorReferenceIlluminant sensorReferenceIlluminant2; //The standard reference illuminant used as the scene light source

    int32_t sensorLensShadingApplied; //Whether the RAW images output from this camera device are subject to lens shading correction

    //other keys
    CCRect sensorOpticalBlackRegions[10]; //List of disjoint rectangles indicating the sensor optically shielded black pixel regions
} CCSensorCapability;

typedef struct {
    //BC capability
    int32_t supportedLensFocalLengthCnt;
    float supportedLensFocalLength[5]; //List of supported lens focal length

    int32_t supportedLensOpticalStabModeCnt;
    CCLensOpticalStabilization supportedLensOpticalStabMode[CC_LENS_OPTICAL_STABILIZATION_NUM]; //List of optical image stabilization (OIS) modes

    float lensHyperfocalDistance; //Hyperfocal distance for this lens

    float lensMinFocusDistance; //Shortest distance from frontmost surface of the lens

    //MANUAL_SENSOR capability
    CCLensFocusDistanceCalibraion lensFocusDistanceCalibrationMode; //The lens focus distance calibration quality

    int32_t supportedLensAperturesCnt;
    float supportedLensApertures[5]; //List of aperture size values

    int32_t supportedLensFilterDensitiesCnt;
    float supportedLensFilterDensities[5]; //List of neutral density filter values
} CCLensCapability;

typedef struct {
    //BC capability
    int32_t supportedJpegThumbnailSizeCnt;
    CCSize supportedJpegThumbnailSize[5]; //List of supported JPEG thumbnail sizes
} CCJpegCapability;

typedef struct {
    //BC capability
    int32_t facialDetectModeCnt;
    CCFaceDetectMode faceDetectMode[CC_FACE_DETECT_NUM]; //List of face detection modes

    int32_t maxFaceCount; //The maximum number of simultaneously detectable faces

    //RAW capability
    int32_t supportedHotpixelMapModeCnt;
    CCStatisticsHotPixelMapMode supportedHotpixelMapMode[CC_STATISTICS_HOT_PIXEL_MAP_MODE_NUM]; //List of hot pixel map output modes

    int32_t supportedLensShadingMapModeCnt;
    CCStatisticsLensShadingMapMode supportedLensShadingMapMode[CC_STATISTICS_LENS_SHADING_MAP_MODE_NUM]; //List of lens shading map output modes
} CCStatisticsCapability;

//camera characteristics parameters
typedef struct {
    CCControlCapability supportedControlCaps;
    CCPostProcessCapability supportedPostProcCaps;
    CCSensorCapability supportedSensorCaps;
    CCLensCapability supportedLensCaps;
    CCJpegCapability supportedJpegCaps;
    CCStatisticsCapability supportedStatisticsCaps;
} CCCameraCapability;

//shooting parameters
typedef struct {
    //BC capability
    //scene mode
    CCControl3AMode control3AMode; //control mode
    CCSceneMode sceneMode; //scene mode

    //AF control
    CCAfMode afMode; //af mode
    CCAfTrigger afTrigger; //af trigger
    CCMeteringRect afRegions; //af regions

    //AWB control
    CCAwbMode awbMode; //awb mode
    CCMeteringRect awbRegions; //awb regions

    //AE control
    CCAeMode aeMode; //ae mode
    CCMeteringRect aeRegions; //ae regions
    CCAeAntibandingMode aeAntiBandingMode; //ae Antibanding mode
    int32_t aeCompensation; //ae exposure compensation
    CCAePrecaptureTrigger aeTrigger; //ae precapture trigger
    int32_t minAeTargetFps; //ae target fps range
    int32_t maxAeTargetFps;
    CCFlashMode flashMode; //flash mode

    //JPEG encoder
    double gpsCoordinates[3];
    int8_t gpsProcessingMethod[32];
    int64_t gpsTimeStamp;
    int32_t jpegOrientation;
    int8_t jpegQuality;
    int8_t jpegThumbnailQuality;
    CCSize jpegThumbnailSize;

    //sensor test pattern
    int32_t sensorTestPatternData[4]; //sensor test pattern data
    CCSensorTestPatternMode sensorTestPatternMode; //sensor test pattern mode

    //effect mode
    CCEffectMode effectMode;

    //video stabilization
    CCVideoStabilizationMode videoStabMode;

    //edge enhancement
    CCEdgeEnhancementMode edgeEnhancementMode;

    //lens focal length
    float lensFocalLength;

    //noise reduction mode
    CCNoiseReductionMode noiseReductionMode;

    //crop region
    CCRect scalerCropRegion;

    //MANUAL_SENSOR capability
    //manual black level lock
    int32_t blockLevelLock; //lock black level

    //ae lock
    int32_t aeLock; //ae lock

    //manual exposure control
    int64_t sensorExposureTime;
    int64_t sensorFrameDuration;
    int32_t sensorSensitivity;

    //manual lens control
    float lensAperture;
    float lensFilterDensity;
    float lensFocusDistance;
    CCLensOpticalStabilization lensOpticalStabilization;

    //MANUAL_POSTPROC capability
    //manual aberration correction control
    CCChromaticAberrationMode chromaticAberrationMode;

    //manual white balance control
    CCColorCorrectionMode colorCorrectionMode;
    CCGainMatrix colorCorrectionGains;
    CCRational colorCorrectionTransform[3][3];

    //manual lens shading map control
    CCLensShadingMode lensShadingMode;
    CCStatisticsLensShadingMapMode lensShadingMapMode;

    //manual tonemap control
    CCToneMapMode toneMapMode;
    float toneMapCurveBlue[20]; //iToneMapMaxCurvePoints must be <= 20
    float toneMapCurveGreen[20];
    float toneMapCurveRed[20];
    float toneMapCurveGamma;
    CCTonemapPresetCurve toneMapPresetCurveMode;

    //awb lock
    int32_t awbLock; //awb lock

    //RAW capability
    //sensitivity boosting
    int32_t postRawSensitivityBoost;

    //Hot pixel
    CCHotPixelCorrectionMode hotPixelMode;
    CCStatisticsHotPixelMapMode hotPixelMapMode;

    //YUV_REPROCESSING capability
    float effectiveExposureFactor;
} CCShotParam;

//frame metadata
typedef struct {
    //BC capability
    //scene mode
    CCControl3AMode control3AMode; //control mode
    CCSceneMode sceneMode; //scene mode

    //AF control
    CCAfMode afMode; //af mode
    CCAfTrigger afTrigger; //af trigger
    CCMeteringRect afRegions; //af regions
    CCAfState afState; //af state

    //AWB control
    CCAwbMode awbMode; //awb mode
    CCMeteringRect awbRegions; //awb regions
    CCAwbState awbState; //awb state

    //AE control
    CCAeMode aeMode; //ae mode
    CCMeteringRect aeRegions; //ae regions
    CCAeAntibandingMode aeAntiBandingMode; //ae Antibanding mode
    int32_t aeCompensation; //ae exposure compensation
    CCAePrecaptureTrigger aeTrigger; //ae precapture trigger
    int32_t minAeTargetFps; //ae target fps range
    int32_t maxAeTargetFps;
    CCFlashMode flashMode; //flash mode
    CCAeState aeState; //ae state
    CCFlashState flashState; //flash state

    //JPEG encoder
    double gpsCoordinates[3];
    int8_t gpsProcessingMethod[32];
    int64_t gpsTimeStamp;
    int32_t jpegOrientation;
    int8_t jpegQuality;
    int8_t jpegThumbnailQuality;
    CCSize jpegThumbnailSize;

    //sensor test pattern
    int32_t sensorTestPatternData[4]; //sensor test pattern data
    CCSensorTestPatternMode sensorTestPatternMode; //sensor test pattern mode

    //effect mode
    CCEffectMode effectMode;

    //video stabilization
    CCVideoStabilizationMode videoStabMode;

    //edge enhancement
    CCEdgeEnhancementMode edgeEnhancementMode;

    //lens focal length
    float lensFocalLength;
    CCLensState lensState;

    //noise reduction mode
    CCNoiseReductionMode noiseReductionMode;

    //crop region
    CCRect scalerCropRegion;

    //face info
    int32_t faceCnt;
    CCFaceInfo faceInfo[10];

    //sensor timestamp
    int64_t sensorTimestamp;

    //MANUAL_SENSOR capability
    //manual black level lock
    int32_t blockLevelLock; //lock black level

    //ae lock
    int32_t aeLock; //ae lock

    //manual exposure control
    int64_t sensorExposureTime;
    int64_t sensorFrameDuration;
    int32_t sensorSensitivity;
    CCStatisticsSceneFlicker statisticsSceneFlicker;

    //manual lens control
    float lensAperture;
    float lensFilterDensity;
    float lensFocusDistance;
    CCLensOpticalStabilization lensOpticalStabilization;

    //MANUAL_POSTPROC capability
    //manual aberration correction control
    CCChromaticAberrationMode chromaticAberrationMode;

    //manual white balance control
    CCColorCorrectionMode colorCorrectionMode;
    CCGainMatrix colorCorrectionGains;
    CCRational colorCorrectionTransform[3][3];

    //manual lens shading map control
    CCLensShadingMode lensShadingMode;
    CCStatisticsLensShadingMapMode lensShadingMapMode;
    float lensShadingCorrectionMap[3][4];

    //manual tonemap control
    CCToneMapMode toneMapMode;
    float toneMapCurveBlue[20]; //iToneMapMaxCurvePoints must be <= 20
    float toneMapCurveGreen[20];
    float toneMapCurveRed[20];
    float toneMapCurveGamma;
    CCTonemapPresetCurve toneMapPresetCurveMode;

    //awb lock
    int32_t awbLock; //awb lock

    //RAW capability
    //sensitivity boosting
    int32_t postRawSensitivityBoost;

    //Hot pixel
    CCHotPixelCorrectionMode hotPixelMode;
    CCStatisticsHotPixelMapMode hotPixelMapMode;
    CCPoint hotPixelMap[100];

    //YUV_REPROCESSING capability
    float effectiveExposureFactor;
} CCFrameMetadataParam;

typedef struct {
    int8_t name[32];
    CCLensFacing facing;
    int32_t orientation;
} CCDeviceInfo;

#define buffer_handle_t void
// image buffer header
typedef struct {
    CCImageInfo imageInfo;

    buffer_handle_t* bufferHandle;
    uint8_t* bufferVir; //must be set by cameraCore

    CCFrameMetadataParam *frameMetadata;

    CCStreamId streamId;
} CCHalImageBuffer;


/* ------------------------------------------------------------------------ */


#ifdef __cplusplus
    }
#endif /* extern "C" */


#endif /* _CAMERA_CORE_BASE_H_ */
