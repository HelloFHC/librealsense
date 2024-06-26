
realdds is a library that ease the control of a realsense camera over network using librealsense API.
realdds provides "off the shelf" types and logic that makes it easier and faster to write applications that control a camera over the network.
It is a work in progress aimed at, ultimately, enabling remote users to do with the camera everything a user can do with a USB connected camera, and more (multiple clients for frames, etc...).

realdds uses DDS topics to send and receive information to/from the camera. See [DDS-ICD](https://github.com/IntelRealSense/librealsense/blob/dds/doc/DDS/DDS%20ICD.md)

Users can use realdds stand-alone, or use librealsense with the cmake `BUILD_WITH_DDS` flag on, to use DDS devices seemlessly as any other librealsense device.

# Actors

## Server/Camera

The **server** or **camera** is either a physical device with built in capabilities supporting these use cases, or a dedicated software operating some other device, e.g., Intel RealSense D455, giving it the required capabilities.

An example software tool like this is the [dds-server](https://github.com/IntelRealSense/librealsense/tree/dds/tools/dds/dds-server)

## User/Client

The **client** is the end **user** of the camera. The **user** consumes the data generated by the camera. The **user** can activate or stop camera data streaming and control various options like camera exposure time or brightness.


# Use Cases

## Camera Discovery

The **server** publishes a connected device existence to all interested **user**s in the network.
Using the discovery data, the **user** can have a complete knowledge of the camera and it's abilities.

**Topics** - For this use case, the *device-info* and *notification* topics are used.
//TODO - Topics section needed?

**Preconditions**
1. The **server** and **client** are running, and using the same DDS domain.

**Trigger** - A new `Intel RealSense` device is connected locally (e.g, USB) and detected by the **server**.

**Successful Result** - The **user** will receive all needed data to interact with the device

**Main Scenario**
1. The **server** will create a new *device-info* writer to publish this connected device existence.
2. Subscribed **user**s are matched for this writer.
3. The **server** publishes *device-info* message with this device details. The *device-info* contains a *topic-root* field. All communication to and from the camera is done using topics containing this *topic-root* in their names (see [ICD](https://github.com/IntelRealSense/librealsense/blob/dds/doc/DDS/DDS%20ICD.md)).
4. The **user** subscibes to the appropriate *notification* topic using the *topic-root* field (e.g., `realsense/D435I/112322073677/notification`).
5. The **server** will recognize the subscription and will send discovery *notification* messages in this order:
    1. A *device-header* message
    2. A *device-options* message if there are device level options
    3. for each supported stream
        - A *stream-header* message
        - A *stream-options* message

**Variations**
1. A new **user** joins the network, subscribing to the *device-info* topic.
    1. Continue from step 2 of the Main Scenario.

**Exceptions**
None.

**Constraints**
1. Discovery *notification* messages timeout is **30 seconds**.


## Camera Disconnection

The **camera** is disconnected and no longer available to the **user**s.

**Topics** - For this use case, no realdds topic is sent, only underlying DDS topics.

**Preconditions**
1. An `Intel RealSense` device is connected to the **server**.

**Trigger** - `Intel RealSense` device is disconnected from the **server**.

**Successful Result** - The **Server** and all **user**s have released the resources related to the camera.

**Main Scenario**
1. The **server** will destroy the appropriate *device-info* writer.
2. Subscribed **user**s will be notified via DDS middleware about the writer destruction (subscription unmatched) and will release all related resources.
3. The **server** will release all other device related resources.

**Variations**
None. 

**Exceptions**
None.

**Constraints**
None.


## Start Streaming

The **user** requests to receive streams of data (images, IMU, etc...). The **server** starts sending the data to it.

**Topics** - *control* topic is used by the **user**, appropriate *image*s are sent back by the **server**.

**Preconditions**
1. The **server** and **client** are running, and using the same DDS domain.
2. Camera Discovery phase (see use-case) has completed successfully.

**Trigger** - The **user** triggers this use-case upon demand.

**Successful Result** - The **user** receives *image* messages at the desired frame rates.

**Main Scenario**
1. The **user** sends a *start streaming* message with the requested profiles listed.
2. The **server** starts publishing *image* messages for each requested profile.

**Variations**

None.

**Exceptions**
1. The device is already streaming data from this sensor
    1. The **server** will decline the request //TODO - should handle throw in rs-dds-adapter?
    2. Terminate the use-case. //TODO - send failure *notification*? Currently not in the ICD, still WIP, will probably change to automatically stream when a reader is matched.

**Constraints**
None.


## Stop Streaming

The **user** requests to stop sending data from some streams (images, IMU, etc...). The **server** stops sending the data.

**Topics** - *control* topic is used by the **user**.

**Preconditions**
1. The **server** and **client** are running, and using the same DDS domain.
2. Camera Discovery phase (see use-case) have completed successfully.

**Trigger** - The **user** triggers this use-case upon demand.

**Successful Result** - The **server** stops publishing *image* messages of the requested profiles.

**Main Scenario**
1. The **user** sends a *stop streaming* message with the profiles requested to close listed.
2. The **server** stops publishing *image* messages for the requested profiles.

**Variations**
None.

**Exceptions**
1. The device is not streaming data of the requested profiles
    1. Terminate the use-case. //TODO - send failure *notification*? Currently not in the ICD

**Constraints**
None.

**Notes**
1. When using librealsense API to control the device (as is the case with the `rs-dds-adapter` tool), when stoping a stream all streams in the containing sensor will be stopped.
realdds does not use a "sensor" concept to start or stop streams, it is a librealsense limitation.
For example, if "Infrared 1" and "Infrared 2" streams of the "Stereo Module" are both streaming and one of them is requested to stop, both will stop streaming.


## Query Option

The **user** requests to current value of a supported option, e.g., color stream's gain or depth stream's depth units value.

**Topics** - *control* topic is used by the **user**. *notification* topic is used by the **server**.

**Preconditions**
1. The **server** and **client** are running, and using the same DDS domain.
2. Camera Discovery phase (see use-case) have completed successfully.

**Trigger** - The **user** triggers this use-case upon demand.

**Successful Result** - The **user** have knowledge of the option value.

**Main Scenario**
1. The **user** sends a *query option* message with the option name and the option owner name (same option can be supported by multiple streams).
2. The **server** queries the actual value from the device.
3. The **server** sends a *query option* reply message with the value to the **user**.

**Variations**
None. 

**Exceptions**
1. Option is not supported or otherwise the value cannot be queried.
    1. The **server** sends a *query option* reply message with failure status and failure reason to the **user**.

**Constraints**
None.


## Set Option

The **user** requests to current value of a supported option, e.g., color stream's gain or depth stream's depth units value.

**Topics** - *control* topic is used by the **user**. *notification* topic is used by the **server**.

**Preconditions**
1. The **server** and **client** are running, and using the same DDS domain.
2. Camera Discovery phase (see use-case) have completed successfully.

**Trigger** - The **user** triggers this use-case upon demand.

**Successful Result** - The **user** have knowledge of the option value.

**Main Scenario**
1. The **user** sends a *query option* message with the option name and the option owner name (same option can be supported by multiple streams).
2. The **server** queries the actual value from the device.
3. The **server** sends a *query option* reply message with the value to the **user**.

**Variations**
None. 

**Exceptions**
1. Option is not supported or otherwise the value cannot be queried.
    1. The **server** sends a *query option* reply message with failure status and failure reason to the **user**.

**Constraints**
None.


## HW Monitor Command

## HW Reset?
//TODO - Is needed? Is different from any other command? possibly no reply...

## FW Update?
//TODO - Should be supported over network? Using HW monitor commands?

## Calibrations
//TODO - Is needed?



--------------------------------------------------------

## Template

Short description

**Topics** - 

**Preconditions**
1. 

**Trigger** - 

**Successful Result** - 

**Main Scenario**
1. 
2. 

**Variations**
1. 

**Exceptions**
1. 

**Constraints**


