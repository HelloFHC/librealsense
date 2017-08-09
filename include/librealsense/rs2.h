/* License: Apache 2.0. See LICENSE file in root directory.
   Copyright(c) 2015 Intel Corporation. All Rights Reserved. */

/** \file rs2.h
* \brief
* Exposes librealsense functionality for C compilers
*/


#ifndef LIBREALSENSE_RS2_H
#define LIBREALSENSE_RS2_H

#ifdef __cplusplus
extern "C" {
#endif

#define RS2_API_MAJOR_VERSION    2
#define RS2_API_MINOR_VERSION    7
#define RS2_API_PATCH_VERSION    0

#define STRINGIFY(arg) #arg
#define VAR_ARG_STRING(arg) STRINGIFY(arg)

/* Versioning rules            : For each release at least one of [MJR/MNR/PTCH] triple is promoted                                             */
/*                             : Versions that differ by RS2_API_PATCH_VERSION only are interface-compatible, i.e. no user-code changes required */
/*                             : Versions that differ by MAJOR/MINOR VERSION component can introduce API changes                                */
/* Version in encoded integer format (1,9,x) -> 01090x. note that each component is limited into [0-99] range by design                         */
#define RS2_API_VERSION  (((RS2_API_MAJOR_VERSION) * 10000) + ((RS2_API_MINOR_VERSION) * 100) + (RS2_API_PATCH_VERSION))
/* Return version in "X.Y.Z" format */
#define RS2_API_VERSION_STR (VAR_ARG_STRING(RS2_API_MAJOR_VERSION.RS2_API_MINOR_VERSION.RS2_API_PATCH_VERSION))

/** \brief Exception types are the different categories of errors that RealSense API might return */
typedef enum rs2_exception_type
{
    RS2_EXCEPTION_TYPE_UNKNOWN,
    RS2_EXCEPTION_TYPE_CAMERA_DISCONNECTED,      /**< Device was disconnected, this can be caused by outside intervention, by internal firmware error or due to insufficient power */
    RS2_EXCEPTION_TYPE_BACKEND,                  /**< Error was returned from the underlying OS-specific layer */
    RS2_EXCEPTION_TYPE_INVALID_VALUE,            /**< Invalid value was passed to the API */
    RS2_EXCEPTION_TYPE_WRONG_API_CALL_SEQUENCE,  /**< Function precondition was violated */
    RS2_EXCEPTION_TYPE_NOT_IMPLEMENTED,          /**< The method is not implemented at this point */
    RS2_EXCEPTION_TYPE_DEVICE_IN_RECOVERY_MODE,  /**< Device is in recovery mode and might require firmware update */
    RS2_EXCEPTION_TYPE_IO,                       /**< IO Device failure */
    RS2_EXCEPTION_TYPE_COUNT                     /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_exception_type;

/** \brief Streams are different types of data provided by RealSense devices */
typedef enum rs2_stream
{
    RS2_STREAM_ANY,
    RS2_STREAM_DEPTH                            , /**< Native stream of depth data produced by RealSense device */
    RS2_STREAM_COLOR                            , /**< Native stream of color data captured by RealSense device */
    RS2_STREAM_INFRARED                         , /**< Native stream of infrared data captured by RealSense device */
    RS2_STREAM_FISHEYE                          , /**< Native stream of fish-eye (wide) data captured from the dedicate motion camera */
    RS2_STREAM_GYRO                             , /**< Native stream of gyroscope motion data produced by RealSense device */
    RS2_STREAM_ACCEL                            , /**< Native stream of accelerometer motion data produced by RealSense device */
    RS2_STREAM_GPIO                             , /**< Signals from external device connected through GPIO */
    RS2_STREAM_COUNT
} rs2_stream;

/** \brief Format identifies how binary data is encoded within a frame */
typedef enum rs2_format
{
    RS2_FORMAT_ANY             , /**< When passed to enable stream, librealsense will try to provide best suited format */
    RS2_FORMAT_Z16             , /**< 16-bit linear depth values. The depth is meters is equal to depth scale * pixel value. */
    RS2_FORMAT_DISPARITY16     , /**< 16-bit linear disparity values. The depth in meters is equal to depth scale / pixel value. */
    RS2_FORMAT_XYZ32F          , /**< 32-bit floating point 3D coordinates. */
    RS2_FORMAT_YUYV            , /**< Standard YUV pixel format as described in https://en.wikipedia.org/wiki/YUV */
    RS2_FORMAT_RGB8            , /**< 8-bit red, green and blue channels */
    RS2_FORMAT_BGR8            , /**< 8-bit blue, green, and red channels -- suitable for OpenCV */
    RS2_FORMAT_RGBA8           , /**< 8-bit red, green and blue channels + constant alpha channel equal to FF */
    RS2_FORMAT_BGRA8           , /**< 8-bit blue, green, and red channels + constant alpha channel equal to FF */
    RS2_FORMAT_Y8              , /**< 8-bit per-pixel grayscale image */
    RS2_FORMAT_Y16             , /**< 16-bit per-pixel grayscale image */
    RS2_FORMAT_RAW10           , /**< Four 10-bit luminance values encoded into a 5-byte macropixel */
    RS2_FORMAT_RAW16           , /**< 16-bit raw image */
    RS2_FORMAT_RAW8            , /**< 8-bit raw image */
    RS2_FORMAT_UYVY            , /**< Similar to the standard YUYV pixel format, but packed in a different order */
    RS2_FORMAT_MOTION_RAW      , /**< Raw data from the motion sensor */
    RS2_FORMAT_MOTION_XYZ32F   , /**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis */
    RS2_FORMAT_GPIO_RAW        , /**< Raw data from the external sensors hooked to one of the GPIO's */
    RS2_FORMAT_COUNT             /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_format;

/** \brief Per-Frame-Metadata are set of read-only properties that might be exposed for each individual frame */
typedef enum rs2_frame_metadata
{
    RS2_FRAME_METADATA_FRAME_COUNTER        , /**< A sequential index managed per-stream. Integer value*/
    RS2_FRAME_METADATA_FRAME_TIMESTAMP      , /**< Timestamp set by device clock when data readout and transmit commence. usec*/
    RS2_FRAME_METADATA_SENSOR_TIMESTAMP     , /**< Timestamp of the middle of sensor's exposure calculated by device. usec*/
    RS2_FRAME_METADATA_ACTUAL_EXPOSURE      , /**< Sensor's exposure width. When Auto Exposure (AE) is on the value is controlled by firmware. usec*/
    RS2_FRAME_METADATA_GAIN_LEVEL           , /**< A relative value increasing which will increase the Sensor's gain factor. \
                                              When AE is set On, the value is controlled by firmware. Integer value*/
    RS2_FRAME_METADATA_AUTO_EXPOSURE        , /**< Auto Exposure Mode indicator. Zero corresponds to AE switched off. */
    RS2_FRAME_METADATA_WHITE_BALANCE        , /**< White Balance setting as a color temperature. Kelvin degrees*/
    RS2_FRAME_METADATA_TIME_OF_ARRIVAL      , /**< Time of arrival in system clock */
    RS2_FRAME_METADATA_COUNT
} rs2_frame_metadata;

/** \brief Distortion model: defines how pixel coordinates should be mapped to sensor coordinates. */
typedef enum rs2_distortion
{
    RS2_DISTORTION_NONE                  , /**< Rectilinear images. No distortion compensation required. */
    RS2_DISTORTION_MODIFIED_BROWN_CONRADY, /**< Equivalent to Brown-Conrady distortion, except that tangential distortion is applied to radially distorted points */
    RS2_DISTORTION_INVERSE_BROWN_CONRADY , /**< Equivalent to Brown-Conrady distortion, except undistorts image instead of distorting it */
    RS2_DISTORTION_FTHETA                , /**< F-Theta fish-eye distortion model */
    RS2_DISTORTION_BROWN_CONRADY         , /**< Unmodified Brown-Conrady distortion model */
    RS2_DISTORTION_COUNT                 , /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_distortion;

/** \brief For SR300 devices: provides optimized settings (presets) for specific types of usage. */
typedef enum rs2_ivcam_visual_preset
{
    RS2_IVCAM_VISUAL_PRESET_SHORT_RANGE             , /**< Preset for short range */
    RS2_IVCAM_VISUAL_PRESET_LONG_RANGE              , /**< Preset for long range */
    RS2_IVCAM_VISUAL_PRESET_BACKGROUND_SEGMENTATION , /**< Preset for background segmentation */
    RS2_IVCAM_VISUAL_PRESET_GESTURE_RECOGNITION     , /**< Preset for gesture recognition */
    RS2_IVCAM_VISUAL_PRESET_OBJECT_SCANNING         , /**< Preset for object scanning */
    RS2_IVCAM_VISUAL_PRESET_FACE_ANALYTICS          , /**< Preset for face analytics */
    RS2_IVCAM_VISUAL_PRESET_FACE_LOGIN              , /**< Preset for face login */
    RS2_IVCAM_VISUAL_PRESET_GR_CURSOR               , /**< Preset for GR cursor */
    RS2_IVCAM_VISUAL_PRESET_DEFAULT                 , /**< Preset for default */
    RS2_IVCAM_VISUAL_PRESET_MID_RANGE               , /**< Preset for mid-range */
    RS2_IVCAM_VISUAL_PRESET_IR_ONLY                 , /**< Preset for IR only */
    RS2_IVCAM_VISUAL_PRESET_COUNT                     /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_ivcam_visual_preset;

/** \brief Defines general configuration controls.
   These can generally be mapped to camera UVC controls, and unless stated otherwise, can be set/queried at any time. */
typedef enum rs2_option
{
    RS2_OPTION_BACKLIGHT_COMPENSATION                     , /**< Enable / disable color backlight compensation*/
    RS2_OPTION_BRIGHTNESS                                 , /**< Color image brightness*/
    RS2_OPTION_CONTRAST                                   , /**< Color image contrast*/
    RS2_OPTION_EXPOSURE                                   , /**< Controls exposure time of color camera. Setting any value will disable auto exposure*/
    RS2_OPTION_GAIN                                       , /**< Color image gain*/
    RS2_OPTION_GAMMA                                      , /**< Color image gamma setting*/
    RS2_OPTION_HUE                                        , /**< Color image hue*/
    RS2_OPTION_SATURATION                                 , /**< Color image saturation setting*/
    RS2_OPTION_SHARPNESS                                  , /**< Color image sharpness setting*/
    RS2_OPTION_WHITE_BALANCE                              , /**< Controls white balance of color image. Setting any value will disable auto white balance*/
    RS2_OPTION_ENABLE_AUTO_EXPOSURE                       , /**< Enable / disable color image auto-exposure*/
    RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE                  , /**< Enable / disable color image auto-white-balance*/
    RS2_OPTION_VISUAL_PRESET                              , /**< Provide access to several recommend sets of option presets for the depth camera */
    RS2_OPTION_LASER_POWER                                , /**< Power of the F200 / SR300 projector, with 0 meaning projector off*/
    RS2_OPTION_ACCURACY                                   , /**< Set the number of patterns projected per frame. The higher the accuracy value the more patterns projected. Increasing the number of patterns help to achieve better accuracy. Note that this control is affecting the Depth FPS */
    RS2_OPTION_MOTION_RANGE                               , /**< Motion vs. Range trade-off, with lower values allowing for better motion sensitivity and higher values allowing for better depth range*/
    RS2_OPTION_FILTER_OPTION                              , /**< Set the filter to apply to each depth frame. Each one of the filter is optimized per the application requirements*/
    RS2_OPTION_CONFIDENCE_THRESHOLD                       , /**< The confidence level threshold used by the Depth algorithm pipe to set whether a pixel will get a valid range or will be marked with invalid range*/
    RS2_OPTION_EMITTER_ENABLED                            , /**< Laser Emitter enabled */
    RS2_OPTION_FRAMES_QUEUE_SIZE                          , /**< Number of frames the user is allowed to keep per stream. Trying to hold-on to more frames will cause frame-drops.*/
    RS2_OPTION_TOTAL_FRAME_DROPS                          , /**< Total number of detected frame drops from all streams */
    RS2_OPTION_AUTO_EXPOSURE_MODE                         , /**< Auto-Exposure modes: Static, Anti-Flicker and Hybrid */
    RS2_OPTION_POWER_LINE_FREQUENCY                       , /**< Power Line Frequency control for anti-flickering Off/50Hz/60Hz/Auto */
    RS2_OPTION_ASIC_TEMPERATURE                           , /**< Current Asic Temperature */
    RS2_OPTION_ERROR_POLLING_ENABLED                      , /**< disable error handling */
    RS2_OPTION_PROJECTOR_TEMPERATURE                      , /**< Current Projector Temperature */
    RS2_OPTION_OUTPUT_TRIGGER_ENABLED                     , /**< Enable / disable trigger to be outputed from the camera to any external device on every depth frame */
    RS2_OPTION_MOTION_MODULE_TEMPERATURE                  , /**< Current Motion-Module Temperature */
    RS2_OPTION_DEPTH_UNITS                                , /**< Number of meters represented by a single depth unit */
    RS2_OPTION_ENABLE_MOTION_CORRECTION                   , /**< Enable/Disable automatic correction of the motion data */
    RS2_OPTION_ADVANCED_MODE_PRESET                       , /**< Camera Advanced-Mode preset */
    RS2_OPTION_COUNT                                      , /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_option;

/** \brief Read-only strings that can be queried from the device.
   Not all information attributes are available on all camera types.
   This information is mainly available for camera debug and troubleshooting and should not be used in applications. */
typedef enum rs2_camera_info {
    RS2_CAMERA_INFO_NAME                           , /**< Friendly name */
    RS2_CAMERA_INFO_SERIAL_NUMBER                  , /**< Device serial number */
    RS2_CAMERA_INFO_FIRMWARE_VERSION               , /**< Primary firmware version */
    RS2_CAMERA_INFO_LOCATION                       , /**< Unique identifier of the port the device is connected to (platform specific) */
    RS2_CAMERA_INFO_DEBUG_OP_CODE                  , /**< If device supports firmware logging, this is the command to send to get logs from firmware */
    RS2_CAMERA_INFO_ADVANCED_MODE                  , /**< True iff the device is in advanced mode */
    RS2_CAMERA_INFO_PRODUCT_ID                     , /**< Product ID as reported in the USB descriptor */
    RS2_CAMERA_INFO_CAMERA_LOCKED                  , /**< True iff EEPROM is locked */
    RS2_CAMERA_INFO_COUNT                            /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_camera_info;

/** \brief Severity of the librealsense logger */
typedef enum rs2_log_severity {
    RS2_LOG_SEVERITY_DEBUG, /**< Detailed information about ordinary operations */
    RS2_LOG_SEVERITY_INFO , /**< Terse information about ordinary operations */
    RS2_LOG_SEVERITY_WARN , /**< Indication of possible failure */
    RS2_LOG_SEVERITY_ERROR, /**< Indication of definite failure */
    RS2_LOG_SEVERITY_FATAL, /**< Indication of unrecoverable failure */
    RS2_LOG_SEVERITY_NONE , /**< No logging will occur */
    RS2_LOG_SEVERITY_COUNT  /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_log_severity;

/** \brief Category of the librealsense notifications */
typedef enum rs2_notification_category{
    RS2_NOTIFICATION_CATEGORY_FRAMES_TIMEOUT,   /**< Frames didn't arrived within 5 seconds */
    RS2_NOTIFICATION_CATEGORY_FRAME_CORRUPTED,  /**< Received partial/incomplete frame */
    RS2_NOTIFICATION_CATEGORY_HARDWARE_ERROR,   /**< Error reported from the device */
    RS2_NOTIFICATION_CATEGORY_UNKNOWN_ERROR,    /**< Received unknown error from the device */
    RS2_NOTIFICATION_CATEGORY_COUNT             /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_notification_category;

/** \brief Specifies the clock in relation to which the frame timestamp was measured. */
typedef enum rs2_timestamp_domain
{
    RS2_TIMESTAMP_DOMAIN_HARDWARE_CLOCK, /**< Frame timestamp was measured in relation to the camera clock */
    RS2_TIMESTAMP_DOMAIN_SYSTEM_TIME,    /**< Frame timestamp was measured in relation to the OS system clock */
    RS2_TIMESTAMP_DOMAIN_COUNT           /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
} rs2_timestamp_domain;

/** \brief Specifies advanced interfaces (capabilities) objects may implement */
typedef enum rs2_extension
{
    RS2_EXTENSION_UNKNOWN,
    RS2_EXTENSION_DEBUG,
    RS2_EXTENSION_INFO,
    RS2_EXTENSION_MOTION,
    RS2_EXTENSION_OPTIONS,
    RS2_EXTENSION_VIDEO,
    RS2_EXTENSION_ROI,
    RS2_EXTENSION_DEPTH_SENSOR,
    RS2_EXTENSION_VIDEO_FRAME,
    RS2_EXTENSION_MOTION_FRAME,
    RS2_EXTENSION_COMPOSITE_FRAME,
    RS2_EXTENSION_POINTS,
    RS2_EXTENSION_ADVANCED_MODE,
    RS2_EXTENSION_RECORD,
    RS2_EXTENSION_VIDEO_PROFILE,
    RS2_EXTENSION_PLAYBACK,
    RS2_EXTENSION_COUNT
} rs2_extension;

typedef enum rs2_playback_status
{
    RS2_PLAYBACK_STATUS_UNKNOWN, /**< Unknown state */
    RS2_PLAYBACK_STATUS_PLAYING, /**< One or more sensors were started, playback is reading and raising data */
    RS2_PLAYBACK_STATUS_PAUSED,  /**< One or more sensors were started, but playback paused reading and paused raising data*/
    RS2_PLAYBACK_STATUS_STOPPED, /**< All sensors were stopped, or playback has ended (all data was read). This is the initial playback status*/
    RS2_PLAYBACK_STATUS_COUNT
} rs2_playback_status;

/** \brief Video stream intrinsics */
typedef struct rs2_intrinsics
{
    int           width;     /**< Width of the image in pixels */
    int           height;    /**< Height of the image in pixels */
    float         ppx;       /**< Horizontal coordinate of the principal point of the image, as a pixel offset from the left edge */
    float         ppy;       /**< Vertical coordinate of the principal point of the image, as a pixel offset from the top edge */
    float         fx;        /**< Focal length of the image plane, as a multiple of pixel width */
    float         fy;        /**< Focal length of the image plane, as a multiple of pixel height */
    rs2_distortion model;     /**< Distortion model of the image */
    float         coeffs[5]; /**< Distortion coefficients */
} rs2_intrinsics;

/** \brief Motion device intrinsics: scale, bias, and variances */
typedef struct rs2_motion_device_intrinsic
{
    /* Scale X        cross axis        cross axis      Bias X */
    /* cross axis     Scale Y           cross axis      Bias Y */
    /* cross axis     cross axis        Scale Z         Bias Z */
    float data[3][4];          /**< Interpret data array values */

    float noise_variances[3];  /**< Variance of noise for X, Y, and Z axis */
    float bias_variances[3];   /**< Variance of bias for X, Y, and Z axis */
} rs2_motion_device_intrinsic;

/** \brief Cross-stream extrinsics: encode the topology describing how the different devices are connected. */
typedef struct rs2_extrinsics
{
    float rotation[9];    /**< Column-major 3x3 rotation matrix */
    float translation[3]; /**< Three-element translation vector, in meters */
} rs2_extrinsics;

/** \brief 3D coordinates with origin at topmost left corner of the lense,
     with positive Z pointing away from the camera, positive X pointing camera right and positive Y pointing camera down */
typedef struct rs2_vertex
{
    float xyz[3];
} rs2_vertex;

/** \brief Pixel location within 2D image. (0,0) is the topmost, left corner. Positive X is right, positive Y is down */
typedef struct rs2_pixel
{
    int ij[2];
} rs2_pixel;

typedef struct rs2_device_info rs2_device_info;
typedef struct rs2_context rs2_context;
typedef struct rs2_device_list rs2_device_list;
typedef struct rs2_device rs2_device;
typedef struct rs2_sensor_list rs2_sensor_list;
typedef struct rs2_sensor rs2_sensor;
typedef struct rs2_error rs2_error;
typedef struct rs2_stream_profile_list rs2_stream_profile_list;
typedef struct rs2_stream_profile rs2_stream_profile;
typedef struct rs2_raw_data_buffer rs2_raw_data_buffer;
typedef struct rs2_frame rs2_frame;
typedef struct rs2_frame_queue rs2_frame_queue;
typedef struct rs2_notification rs2_notification;
typedef struct rs2_notifications_callback rs2_notifications_callback;
typedef struct rs2_devices_changed_callback rs2_devices_changed_callback;
typedef struct rs2_frame_callback rs2_frame_callback;
typedef struct rs2_log_callback rs2_log_callback;
typedef struct rs2_syncer rs2_syncer;
typedef struct rs2_pipeline rs2_pipeline;
typedef struct rs2_device_serializer rs2_device_serializer;
typedef struct rs2_source rs2_source;
typedef struct rs2_processing_block rs2_processing_block;
typedef struct rs2_frame_processor_callback rs2_frame_processor_callback;
typedef struct rs2_playback_status_changed_callback rs2_playback_status_changed_callback;

typedef void (*rs2_frame_callback_ptr)(rs2_frame*, void*);
typedef void (*rs2_frame_processor_callback_ptr)(rs2_frame**, int, rs2_source*, void*);
typedef void (*rs2_notification_callback_ptr)(rs2_notification*, void*);
typedef void (*rs2_devices_changed_callback_ptr)(rs2_device_list*, rs2_device_list*, void*);
typedef void (*rs2_log_callback_ptr)(rs2_log_severity min_severity, const char* message, void* user);
typedef void (*rs2_playback_status_changed_callback_ptr)(rs2_playback_status);

typedef double      rs2_time_t;     /**< Timestamp format. units are milliseconds */
typedef long long   rs2_metadata_t; /**< Metadata attribute type is defined as 64 bit signed integer*/

/**
* \brief Creates RealSense context that is required for the rest of the API.
* \param[in] api_version Users are expected to pass their version of \c RS2_API_VERSION to make sure they are running the correct librealsense version.
* \param[out] error  If non-null, receives any error that occurs during this call, otherwise, errors are ignored.
* \return            Context object
*/
rs2_context* rs2_create_context(int api_version, rs2_error** error);

/**
* \brief Frees the relevant context object.
*
* This action might invalidate \c rs2_device pointers created from this context.
* \param[in] context Object that is no longer needed
*/
void rs2_delete_context(rs2_context* context);

/**
* create a static snapshot of all connected devices at the time of the call
* \param context     Object representing librealsense session
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            the list of devices, should be released by rs2_delete_device_list
*/
rs2_device_list* rs2_query_devices(const rs2_context* context, rs2_error** error);

/**
* return the time at specific time point
* \param context     Object representing librealsense session
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            the time at specific time point, in live and redord mode it will return the system time and in playback mode it will return the recorded time
*/
rs2_time_t rs2_get_context_time(const rs2_context* context, rs2_error** error);

/**
* Determines number of devices in a list
* \param[in] info_list The list of connected devices captured using rs2_query_devices
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            Device count
*/
int rs2_get_device_count(const rs2_device_list* info_list, rs2_error** error);

/**
* Deletes device list, any devices created from this list will remain unaffected
* \param[in] info_list list to delete
*/
void rs2_delete_device_list(rs2_device_list* info_list);

/**
* this function returns true if the specific device is contained inside the device list "removed"
* \param[in] device    RealSense device
* \param[in] event_information    handle returned from a callback
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            true if the device was disconnected and false otherwise
*/
int rs2_device_list_contains(const rs2_device_list* removed, const rs2_device* dev, rs2_error** error);

/**
* create device by index
* \param[in] index   the zero based index of device to retrieve
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            the requested device, should be released by rs2_delete_device
*/
rs2_device* rs2_create_device(const rs2_device_list* list, int index, rs2_error** error);

/**
* delete relasense device
* \param[in] device realsense device to delete
*/
void rs2_delete_device(rs2_device* device);

/**
* create a static snapshot of all connected sensors within specific device
* \param devuice     Specific RealSense device
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            the list of devices, should be released by rs2_delete_device_list
*/
rs2_sensor_list* rs2_query_sensors(const rs2_device* device, rs2_error** error);

/**
* Deletes sensors list, any sensors created from this list will remain unaffected
* \param[in] info_list list to delete
*/
void rs2_delete_sensor_list(rs2_sensor_list* info_list);

/**
* Determines number of sensors in a list
* \param[in] info_list The list of connected sensors captured using rs2_query_sensors
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            Sensors count
*/
int rs2_get_sensors_count(const rs2_sensor_list* info_list, rs2_error** error);

/**
* delete relasense sensor
* \param[in] sensor realsense sensor to delete
*/
void rs2_delete_sensor(rs2_sensor* sensor);

/**
* create sensor by index
* \param[in] index   the zero based index of sensor to retrieve
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            the requested sensor, should be released by rs2_delete_sensor
*/
rs2_sensor* rs2_create_sensor(const rs2_sensor_list* list, int index, rs2_error** error);

/**
 * TODO
 * \param[out] error        if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_get_extrinsics(const rs2_stream_profile* from,
                        const rs2_stream_profile* to,
                        rs2_extrinsics * extrin, rs2_error ** error);

rs2_device* rs2_create_device_from_sensor(const rs2_sensor* sensor, rs2_error ** error);

/**
 * returns the intrinsics of specific stream configuration
 * \param[in]  device    RealSense device to query
 * \param[in]  stream    type of stream
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_get_motion_intrinsics(const rs2_sensor * device, rs2_stream stream, rs2_motion_device_intrinsic * intrinsics, rs2_error ** error);

/**
 * send hardware reset request to the device
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_hardware_reset(const rs2_device * device, rs2_error ** error);

/**
* check if physical subdevice is supported
* \param[in] device  input RealSense device
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            list of stream profiles that given subdevice can provide, should be released by rs2_delete_profiles_list
*/
rs2_stream_profile_list* rs2_get_stream_profiles(rs2_sensor* device, rs2_error** error);

/**
* Get pointer to specific stream profile
* \param[in] list        the list of supported profiles returned by rs2_get_supported_profiles
* \param[in] index       the zero based index of the streaming mode
* \param[out] error      if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
const rs2_stream_profile* rs2_get_stream_profile(const rs2_stream_profile_list* list, int index, rs2_error** error);

void rs2_get_stream_profile_data(const rs2_stream_profile* mode, rs2_stream* stream, rs2_format* format, int* index, int* unique_id, int* framerate, rs2_error** error);

void rs2_set_stream_profile_data(rs2_stream_profile* mode, rs2_stream stream, int index, rs2_format format, rs2_error** error);

rs2_stream_profile* rs2_clone_stream_profile(const rs2_stream_profile* mode, rs2_error** error);

void rs2_delete_stream_profile(rs2_stream_profile* mode);

int rs2_stream_profile_is(const rs2_stream_profile* mode, rs2_extension type, rs2_error** error);

/**
 * returns the intrinsics of specific stream configuration
   TODO
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_get_video_stream_intrinsics(const rs2_stream_profile* from, rs2_intrinsics * intrinsics, rs2_error ** error);

void rs2_get_video_stream_resolution(const rs2_stream_profile* from, int* width, int* height, rs2_error** error);

int rs2_get_stream_profile_size(const rs2_stream_profile* profile, rs2_error** error);
int rs2_is_stream_profile_recommended(const rs2_stream_profile* profile, rs2_error** error);

/**
* get the number of supported stream profiles
* \param[in] list        the list of supported profiles returned by rs2_get_supported_profiles
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return number of supported subdevice profiles
*/
int rs2_get_stream_profiles_count(const rs2_stream_profile_list* list, rs2_error** error);

/**
* delete stream profiles list
* \param[in] list        the list of supported profiles returned by rs2_get_supported_profiles
*/
void rs2_delete_stream_profiles_list(rs2_stream_profile_list* list);

/**
* open subdevice for exclusive access, by committing to a configuration
* \param[in] sensor relevant RealSense device
* \param[in] profile TODO
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_open(rs2_sensor* device, const rs2_stream_profile* profile, rs2_error** error);

/**
* open subdevice for exclusive access, by committing to composite configuration, specifying one or more stream profiles
* this method should be used for interdependent  streams, such as depth and infrared, that have to be configured together
* \param[in] sensor relevant RealSense device
TODO
* \param[in] count      number of simultaneous  stream profiles to configure
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_open_multiple(rs2_sensor* device, const rs2_stream_profile** profiles, int count, rs2_error** error);

/**
* stop any streaming from specified subdevice
* \param[in] sensor     RealSense device
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_close(const rs2_sensor* sensor, rs2_error** error);

/**
* start streaming from specified configured sensor
* \param[in] sensor  RealSense device
* \param[in] on_frame function pointer to register as per-frame callback
* \param[in] user auxiliary  data the user wishes to receive together with every frame callback
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_start(const rs2_sensor* sensor, rs2_frame_callback_ptr on_frame, void* user, rs2_error** error);

/**
* set callback to get notifications from specified sensor
* \param[in] sensor  RealSense device
* \param[in] device  RealSense device
* \param[in] callback function pointer to register as per-notifications callback
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_set_notifications_callback(const rs2_sensor* sensor, rs2_notification_callback_ptr on_notification, void* user, rs2_error** error);

/**
* start streaming from specified configured sensor
* \param[in] sensor  RealSense device
* \param[in] callback callback object created from c++ application. ownership over the callback object is moved into the relevant streaming lock
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_start_cpp(const rs2_sensor* sensor, rs2_frame_callback* callback, rs2_error** error);

/**
* stops streaming from specified configured device
* \param[in] sensor  RealSense sensor
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_stop(const rs2_sensor* sensor, rs2_error** error);

/**
* set callback to get notifications from specified device
* \param[in] sensor  RealSense sensor
* \param[in] callback callback object created from c++ application. ownership over the callback object is moved into the relevant subdevice lock
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_set_notifications_callback_cpp(const rs2_sensor* sensor, rs2_notifications_callback* callback, rs2_error** error);


/**
* retrieve description from notification handle
* \param[in] notification      handle returned from a callback
* \return            the notification description
*/
const char* rs2_get_notification_description(rs2_notification* notification, rs2_error** error);

/**
* retrieve timestamp from notification handle
* \param[in] notification      handle returned from a callback
* \return            the notification timestamp
*/
rs2_time_t rs2_get_notification_timestamp(rs2_notification * notification, rs2_error** error);

/**
* retrieve severity from notification handle
* \param[in] notification      handle returned from a callback
* \return            the notification severity
*/
rs2_log_severity rs2_get_notification_severity(rs2_notification * notification, rs2_error** error);

/**
* retrieve category from notification handle
* \param[in] notification      handle returned from a callback
* \return            the notification category
*/
rs2_notification_category rs2_get_notification_category(rs2_notification * notification, rs2_error** error);
/**
* set callback to get devices changed events
* these events will be raised by the context whenever new RealSense device is connected or existing device gets disconnected
* \param context     Object representing librealsense session
* \param[in] callback callback object created from c++ application. ownership over the callback object is moved into the context
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_set_devices_changed_callback_cpp(rs2_context* context, rs2_devices_changed_callback* callback, rs2_error** error);

/**
* set callback to get devices changed events
* these events will be raised by the context whenever new RealSense device is connected or existing device gets disconnected
* \param context     Object representing librealsense session
* \param[in] callback function pointer to register as per-notifications callback
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_set_devices_changed_callback(rs2_context* context, rs2_devices_changed_callback_ptr callback, rs2_error** error);

/**
* retrieve metadata from frame handle
* \param[in] frame      handle returned from a callback
* \param[in] frame_metadata  the rs2_frame_metadata whose latest frame we are interested in
* \return            the metadata value
*/
rs2_metadata_t rs2_get_frame_metadata(const rs2_frame* frame, rs2_frame_metadata frame_metadata, rs2_error** error);

/**
* determine device metadata
* \param[in] frame      handle returned from a callback
* \param[in] metadata    the metadata to check for support
* \return                true if device has this metadata
*/
int rs2_supports_frame_metadata(const rs2_frame* frame, rs2_frame_metadata frame_metadata, rs2_error** error);

/**
* retrieve timestamp from frame handle in milliseconds
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               the timestamp of the frame in milliseconds
*/
rs2_time_t rs2_get_frame_timestamp(const rs2_frame* frame, rs2_error** error);

/**
* retrieve timestamp domain from frame handle. timestamps can only be comparable if they are in common domain
* (for example, depth timestamp might come from system time while color timestamp might come from the device)
* this method is used to check if two timestamp values are comparable (generated from the same clock)
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               the timestamp domain of the frame (camera / microcontroller / system time)
*/
rs2_timestamp_domain rs2_get_frame_timestamp_domain(const rs2_frame* frameset, rs2_error** error);

/**
* retrieve frame number from frame handle
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               the frame nubmer of the frame, in milliseconds since the device was started
*/
unsigned long long rs2_get_frame_number(const rs2_frame* frame, rs2_error** error);

/**
* retrieve data from frame handle
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               the pointer to the start of the frame data
*/
const void* rs2_get_frame_data(const rs2_frame* frame, rs2_error** error);

/**
* retrieve frame width in pixels
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               frame width in pixels
*/
int rs2_get_frame_width(const rs2_frame* frame, rs2_error** error);

/**
* retrieve frame height in pixels
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               frame height in pixels
*/
int rs2_get_frame_height(const rs2_frame* frame, rs2_error** error);

/**
* retrieve frame stride in bytes (number of bytes from start of line N to start of line N+1)
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               stride in bytes
*/
int rs2_get_frame_stride_in_bytes(const rs2_frame* frame, rs2_error** error);

/**
* retrieve bits per pixels in the frame image
* (note that bits per pixel is not necessarily divided by 8, as in 12bpp)
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               bits per pixel
*/
int rs2_get_frame_bits_per_pixel(const rs2_frame* frame, rs2_error** error);

/**
* create additional reference to a frame without duplicating frame data
* \param[in] frame      handle returned from a callback
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               new frame reference, has to be released by rs2_release_frame
*/
void rs2_frame_add_ref(rs2_frame* frame, rs2_error ** error);

/**
* relases the frame handle
* \param[in] frame handle returned from a callback
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_release_frame(rs2_frame* frame);

rs2_vertex* rs2_get_frame_vertices(const rs2_frame* frame, rs2_error** error);

rs2_pixel* rs2_get_frame_texture_coordinates(const rs2_frame* frame, rs2_error** error);

int rs2_get_frame_points_count(const rs2_frame* frame, rs2_error** error);

const rs2_stream_profile* rs2_get_frame_stream_profile(const rs2_frame* frame, rs2_error** error);

/**
* check if an option is read-only
* \param[in] sensor   the RealSense sensor
* \param[in] option   option id to be checked
* \param[out] error   if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return true if option is read-only
*/
int rs2_is_option_read_only(const rs2_sensor* sensor, rs2_option option, rs2_error** error);

/**
* read option value from the sensor
* \param[in] sensor   the RealSense sensor
* \param[in] option   option id to be queried
* \param[out] error   if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return value of the option
*/
float rs2_get_option(const rs2_sensor* sensor, rs2_option option, rs2_error** error);

/**
* write new value to sensor option
* \param[in] sensor     the RealSense sensor
* \param[in] option     option id to be queried
* \param[in] value      new value for the option
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_set_option(const rs2_sensor* sensor, rs2_option option, float value, rs2_error** error);

/**
* check if particular option is supported by a subdevice
* \param[in] sensor     the RealSense sensor
* \param[in] option     option id to be checked
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return true if option is supported
*/
int rs2_supports_option(const rs2_sensor* sensor, rs2_option option, rs2_error** error);

/**
* retrieve the available range of values of a supported option
* \param[in] sensor  the RealSense device
* \param[in] option  the option whose range should be queried
* \param[out] min    the minimum value which will be accepted for this option
* \param[out] max    the maximum value which will be accepted for this option
* \param[out] step   the granularity of options which accept discrete values, or zero if the option accepts continuous values
* \param[out] def    the default value of the option
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_get_option_range(const rs2_sensor* sensor, rs2_option option, float* min, float* max, float* step, float* def, rs2_error** error);

/**
* get option description
* \param[in] sensor     the RealSense sensor
* \param[in] option     option id to be checked
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return human-readable option description
*/
const char* rs2_get_option_description(const rs2_sensor* sensor, rs2_option option, rs2_error ** error);

/**
* get option value description (in case specific option value hold special meaning)
* \param[in] device     the RealSense device
* \param[in] option     option id to be checked
* \param[in] value      value of the option
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return human-readable description of a specific value of an option or null if no special meaning
*/
const char* rs2_get_option_value_description(const rs2_sensor* sensor, rs2_option option, float value, rs2_error ** error);

/**
 * \brief sets the active region of interest to be used by auto-exposure algorithm
 * \param[in] sensor     the RealSense sensor
 * \param[in] min_x      lower horizontal bound in pixels
 * \param[in] min_y      lower vertical bound in pixels
 * \param[in] max_x      upper horizontal bound in pixels
 * \param[in] max_y      upper vertical bound in pixels
 * \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_set_region_of_interest(const rs2_sensor* sensor, int min_x, int min_y, int max_x, int max_y, rs2_error ** error);

/**
 * \brief gets the active region of interest to be used by auto-exposure algorithm
 * \param[in] sensor     the RealSense sensor
 * \param[out] min_x     lower horizontal bound in pixels
 * \param[out] min_y     lower vertical bound in pixels
 * \param[out] max_x     upper horizontal bound in pixels
 * \param[out] max_y     upper vertical bound in pixels
 * \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_get_region_of_interest(const rs2_sensor* sensor, int* min_x, int* min_y, int* max_x, int* max_y, rs2_error ** error);

/**
* retrieve camera specific information, like versions of various internal components
* \param[in] device     the RealSense device
* \param[in] info       camera info type to retrieve
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               the requested camera info string, in a format specific to the device model
*/
const char* rs2_get_device_info(const rs2_device* device, rs2_camera_info info, rs2_error** error);

/**
* check if specific camera info is supported
* \param[in] info    the parameter to check for support
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return                true if the parameter both exist and well-defined for the specific device
*/
int rs2_supports_device_info(const rs2_device* device, rs2_camera_info info, rs2_error** error);

/**
* retrieve sensor specific information, like versions of various internal components
* \param[in] sensor     the RealSense sensor
* \param[in] info       camera info type to retrieve
* \param[out] error     if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return               the requested camera info string, in a format specific to the device model
*/
const char* rs2_get_sensor_info(const rs2_sensor* sensor, rs2_camera_info info, rs2_error** error);

/**
* check if specific sensor info is supported
* \param[in] info    the parameter to check for support
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return                true if the parameter both exist and well-defined for the specific device
*/
int rs2_supports_sensor_info(const rs2_sensor* sensor, rs2_camera_info info, rs2_error** error);

/**
* create frame queue. frame queues are the simplest x-platform synchronization primitive provided by librealsense
* to help developers who are not using async APIs
* \param[in] capacity max number of frames to allow to be stored in the queue before older frames will start to get dropped
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return handle to the frame queue, must be released using rs2_delete_frame_queue
*/
rs2_frame_queue* rs2_create_frame_queue(int capacity, rs2_error** error);

/**
* deletes frame queue and releases all frames inside it
* \param[in] frame queue to delete
*/
void rs2_delete_frame_queue(rs2_frame_queue* queue);

/**
* wait until new frame becomes available in the queue and dequeue it
* \param[in] queue the frame queue data structure
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return frame handle to be released using rs2_release_frame
*/
rs2_frame* rs2_wait_for_frame(rs2_frame_queue* queue, rs2_error** error);

/**
* poll if a new frame is available and dequeue if it is
* \param[in] queue the frame queue data structure
* \param[out] output_frame frame handle to be released using rs2_release_frame
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return true if new frame was stored to output_frame
*/
int rs2_poll_for_frame(rs2_frame_queue* queue, rs2_frame** output_frame, rs2_error** error);

/**
* enqueue new frame into a queue
* \param[in] frame frame handle to enqueue (this operation passed ownership to the queue)
* \param[in] queue the frame queue data structure
*/
void rs2_enqueue_frame(const rs2_frame* frame, void* queue);

/**
* send raw data to device
* \param[in] device  input RealSense device
* \param[in] raw_data_to_send   raw data to be send to device
* \param[in] size_of_raw_data_to_send   size of raw_data_to_send
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return            rs2_raw_data_buffer, should be released by rs2_delete_raw_data
*/
const rs2_raw_data_buffer* rs2_send_and_receive_raw_data(rs2_device* device, void* raw_data_to_send, unsigned size_of_raw_data_to_send, rs2_error** error);

/**
* get the size of rs2_raw_data_buffer
* \param[in] buffer  pointer to rs2_raw_data_buffer returned by rs2_send_and_receive_raw_data
* \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return size of rs2_raw_data_buffer
*/
int rs2_get_raw_data_size(const rs2_raw_data_buffer* buffer, rs2_error** error);

/**
* Delete rs2_raw_data_buffer
* \param[in] buffer        rs2_raw_data_buffer returned by rs2_send_and_receive_raw_data
*/
void rs2_delete_raw_data(const rs2_raw_data_buffer* buffer);

/**
* Retrieve char array from rs2_raw_data_buffer
* \param[in] buffer   rs2_raw_data_buffer returned by rs2_send_and_receive_raw_data
* \param[out] error   if non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return raw data
*/
const unsigned char* rs2_get_raw_data(const rs2_raw_data_buffer* buffer, rs2_error** error);

/**
 * librealsense Recorder is intended for effective unit-testing
 * Currently supports three modes of operation:
 */
typedef enum rs2_recording_mode
{
    RS2_RECORDING_MODE_BLANK_FRAMES, /* frame metadata will be recorded, but pixel data will be replaced with zeros to save space */
    RS2_RECORDING_MODE_COMPRESSED,   /* frames will be encoded using a proprietary lossy encoding, aiming at x5 compression at some CPU expense */
    RS2_RECORDING_MODE_BEST_QUALITY, /* frames will not be compressed, but rather stored as-is. This gives best quality and low CPU overhead, but you might run out of memory */
    RS2_RECORDING_MODE_COUNT
} rs2_recording_mode;

/**
 * Create librealsense context that will try to record all operations over librealsense into a file
 * \param[in] api_version realsense API version as provided by RS2_API_VERSION macro
 * \param[in] filename string representing the name of the file to record
 * \param[in] section  string representing the name of the section within existing recording
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return            context object, should be released by rs2_delete_context
 */
rs2_context* rs2_create_recording_context(int api_version, const char* filename, const char* section, rs2_recording_mode mode, rs2_error** error);

/**
 * Create librealsense context that given a file will respond to calls exactly as the recording did
 * if the user calls a method that was either not called during recording or violates causality of the recording error will be thrown
 * \param[in] api_version realsense API version as provided by RS2_API_VERSION macro
 * \param[in] filename string representing the name of the file to play back from
 * \param[in] section  string representing the name of the section within existing recording
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return            context object, should be released by rs2_delete_context
 */
rs2_context* rs2_create_mock_context(int api_version, const char* filename, const char* section, rs2_error** error);

/**
 * Retrieve the API version from the source code. Evaluate that the value is conformant to the established policies
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return            the version API encoded into integer value "1.9.3" -> 10903
 */
int          rs2_get_api_version      (rs2_error ** error);

const char * rs2_get_failed_function  (const rs2_error * error);
const char * rs2_get_failed_args      (const rs2_error * error);
const char * rs2_get_error_message    (const rs2_error * error);
void         rs2_free_error           (rs2_error * error);
rs2_exception_type rs2_get_librealsense_exception_type(const rs2_error * error);

const char * rs2_stream_to_string           (rs2_stream stream);
const char * rs2_format_to_string           (rs2_format format);
const char * rs2_distortion_to_string       (rs2_distortion distortion);
const char * rs2_option_to_string           (rs2_option option);
const char * rs2_camera_info_to_string      (rs2_camera_info info);
const char * rs2_frame_metadata_to_string   (rs2_frame_metadata metadata);
const char * rs2_timestamp_domain_to_string (rs2_timestamp_domain info);
const char * rs2_notification_category_to_string(rs2_notification_category category);
const char * rs2_visual_preset_to_string    (rs2_ivcam_visual_preset preset);
const char * rs2_log_severity_to_string     (rs2_log_severity info);
const char * rs2_visual_preset_to_string    (rs2_ivcam_visual_preset preset);
const char * rs2_exception_type_to_string   (rs2_exception_type type);
const char * rs2_extension_type_to_string   (rs2_extension type);
const char * rs2_playback_status_to_string  (rs2_playback_status status);

void rs2_log_to_console(rs2_log_severity min_severity, rs2_error ** error);
void rs2_log_to_file(rs2_log_severity min_severity, const char * file_path, rs2_error ** error);

/**
 * Test if the given sensor can be extended to the requested extension
 * \param[in] sensor  Realsense sensor
 * \param[in] extension The extension to which the sensor should be tested if it is extendable
 * \param[out] error  if non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return non-zero value iff the sensor can be extended to the given extension
 */
int rs2_is_sensor_extendable_to(const rs2_sensor* sensor, rs2_extension extension, rs2_error ** error);

/**
* Test if the given device can be extended to the requested extension
* \param[in]  device     Realsense device
* \param[in]  extension  The extension to which the device should be tested if it is extendable
* \param[out] error      If non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return non-zero value iff the device can be extended to the given extension
*/
int rs2_is_device_extendable_to(const rs2_device* device, rs2_extension extension_type, rs2_error ** error);

/**
* Test if the given frame can be extended to the requested extension
* \param[in]  frame      Realsense frame
* \param[in]  extension  The extension to which the frame should be tested if it is extendable
* \param[out] error      If non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return non-zero value iff the frame can be extended to the given extension
*/
int rs2_is_frame_extendable_to(const rs2_frame* frame, rs2_extension extension_type, rs2_error ** error);

/**
 * Creates a recording device to record the given device and save it to the given file
 * \param[in]  device    The device to record
 * \param[in]  file      The desired path to which the recorder should save the data
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return A pointer to a device that records its data to file, or null in case of failure
 */

rs2_device* rs2_create_record_device(const rs2_device* device, const char* file, rs2_error** error);

/**
* Pause the recording device without stopping the actual device from streaming. 
* Pausing will cause the device to stop writing new data to the file, in particular, frames and changes to extensions
* \param[in]  device    A recording device
* \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_record_device_pause(const rs2_device* device, rs2_error** error);

/**
* Unpause the recording device. Resume will cause the device to continue writing new data to the file, in particular, frames and changes to extensions
* \param[in]  device    A recording device
* \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
*/
void rs2_record_device_resume(const rs2_device* device, rs2_error** error);

/**
* Creates a playback device to play the content of the given file
* \param[in]  file      Path to the file to play
* \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
* \return A pointer to a device that plays data from the file, or null in case of failure
*/
rs2_device* rs2_create_playback_device(const char* file, rs2_error** error);

/**
 * Gets the path of the file used by the playback device
 * \param[in] device A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return Path to the file used by the playback device
 */
const char* rs2_playback_device_get_file_path(const rs2_device* device, rs2_error** error);

/**
 * Create a new device and add it to the context
 * \param ctx   The context to which the new device will be added
 * \param file  The file from which the device should be created
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * @return  A pointer to a device that plays data from the file, or null in case of failure
 */
rs2_device* rs2_context_add_device(rs2_context* ctx, const char* file, rs2_error** error);

/**
 * Removes a playback device from the context, if exists
 * \param[in]  ctx       The context from which the device should be removed
 * \param[in]  file      The file name that was used to add the device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_context_remove_device(rs2_context* ctx, const char* file, rs2_error** error);

/**
 * Gets the total duration of the file in units of nanoseconds
 * \param[in] device     A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return Total duration of the file in units of nanoseconds
 */
unsigned long long int rs2_playback_get_duration(const rs2_device* device, rs2_error** error);

/**
 * Set the playback to a specified time point of the played data
 * \param[in] device     A playback device.
 * \param[in] time       The time point to which playback should seek, expressed in units of nanoseconds (zero value = start)
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_playback_seek(const rs2_device* device, unsigned long long int time, rs2_error** error);

/**
 * Gets the current position of the playback in the file in terms of time. Units are expressed in nanoseconds
 * \param[in] device     A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
unsigned long long int rs2_playback_get_position(const rs2_device* device, rs2_error** error);

/**
 * Pauses the playback
 * Calling pause() in "Paused" status does nothing
 * If pause() is called while "Playing" or "Stopped", the playback will not play until resume() is called
 * \param[in] device A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_playback_device_resume(const rs2_device* device, rs2_error** error);

/**
 * Un-pauses the playback
 * Calling resume(), when in "Playing" or "Stopped" status, does nothing
 * \param[in] device A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_playback_device_pause(const rs2_device* device, rs2_error** error);

/**
 * Set the playback to work in real time or non real time
 *
 * In real time mode, playback will play the same way the file was recorded.
 * In real time mode if the application takes too long to handle the callback, frames may be dropped.
 * In non real time mode, playback will wait for each callback to finish handling the data before
 * reading the next frame. In this mode no frames will be dropped, and the application controls the
 * frame rate of the playback (according to the callback handler duration).
 * \param[in] device A playback device
 * \param[in] real_time  Indicates if real time is requested, 0 means false, otherwise true
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return True on successfully setting the requested mode
 */
void rs2_playback_device_set_real_time(const rs2_device* device, int real_time, rs2_error** error);

/**
 * Indicates if playback is in real time mode or non real time
 * \param[in] device A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return True iff playback is in real time mode. 0 means false, otherwise true
 */
int rs2_playback_device_is_real_time(const rs2_device* device, rs2_error** error);

/**
 * Register to receive callback from playback device upon its status changes
 *
 * Callbacks are invoked from the reading thread, any heaving processing in the callback handler will affect
 * the reading thread and may cause frame drops\ high latency
 * \param[in] device     A playback device
 * \param[in] callback   A callback handler that will be invoked when the playback status changes
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 */
void rs2_playback_device_set_status_changed_callback(const rs2_device* device, rs2_playback_status_changed_callback* callback, rs2_error** error);

/**
 * Returns the current state of the playback device
 * \param[in] device     A playback device
 * \param[out] error     If non-null, receives any error that occurs during this call, otherwise, errors are ignored
 * \return Current state of the playback
 */
rs2_playback_status rs2_playback_device_get_current_status(const rs2_device* device, rs2_error** error);

rs2_frame* rs2_allocate_synthetic_video_frame(rs2_source* source, const rs2_stream_profile* new_stream, rs2_frame* original, 
    int new_bpp, int new_width, int new_height, int new_stride, rs2_error** error);

rs2_frame* rs2_allocate_composite_frame(rs2_source* source, rs2_frame** frames, int count, rs2_error** error);

rs2_frame* rs2_extract_frame(rs2_frame* composite, int index, rs2_error** error);

int rs2_embedded_frames_count(rs2_frame* composite, rs2_error** error);

void rs2_synthetic_frame_ready(rs2_source* source, rs2_frame* frame, rs2_error** error);

rs2_pipeline* rs2_create_pipeline(rs2_context* ctx, rs2_error ** error);

void rs2_start_pipeline_with_callback(rs2_pipeline* pipe, rs2_frame_callback* on_frame, rs2_error ** error);

void rs2_start_pipeline(rs2_pipeline* pipe, rs2_error ** error);

rs2_frame* rs2_pipeline_wait_for_frames(rs2_pipeline* pipe, unsigned int timeout_ms, rs2_error ** error);

void rs2_delete_pipeline(rs2_pipeline* block);

rs2_processing_block* rs2_create_processing_block(rs2_context* ctx, rs2_frame_processor_callback* proc, rs2_error** error);

rs2_processing_block* rs2_create_sync_processing_block(rs2_error** error);

void rs2_start_processing(rs2_processing_block* block, rs2_frame_callback* on_frame, rs2_error** error);

void rs2_process_frame(rs2_processing_block* block, rs2_frame* frame, rs2_error** error);

void rs2_delete_processing_block(rs2_processing_block* block);

rs2_processing_block* rs2_create_pointcloud(rs2_context* ctx, rs2_error** error);

rs2_processing_block* rs2_create_colorizer(rs2_error** error);

float rs2_get_depth_scale(rs2_sensor* sensor, rs2_error** error);

#ifdef __cplusplus
}
#endif
#endif