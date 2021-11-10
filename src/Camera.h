#pragma once
#include "DriverUserData.h"
#include "MediaCaptureAdapter.h"

class Camera final : public VuforiaDriver::ExternalCamera
{
public:
    Camera( VuforiaDriver::PlatformData* platformData, DriverUserData* userData);

    bool VUFORIA_DRIVER_CALLING_CONVENTION open() override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION close()  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION start(VuforiaDriver::CameraMode, VuforiaDriver::CameraCallback* cb)  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION stop()  override;
    uint32_t VUFORIA_DRIVER_CALLING_CONVENTION getNumSupportedCameraModes()  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION getSupportedCameraMode(uint32_t  index, VuforiaDriver::CameraMode* out)  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION supportsExposureMode(VuforiaDriver::ExposureMode)  override;
    VuforiaDriver::ExposureMode VUFORIA_DRIVER_CALLING_CONVENTION getExposureMode()  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION setExposureMode(VuforiaDriver::ExposureMode)  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION supportsExposureValue()  override;
    uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValueMin()  override;
    uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValueMax()  override;
    uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValue()  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION setExposureValue(uint64_t)  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION supportsFocusMode(VuforiaDriver::FocusMode)  override;
    VuforiaDriver::FocusMode VUFORIA_DRIVER_CALLING_CONVENTION getFocusMode()  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION setFocusMode(VuforiaDriver::FocusMode)  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION supportsFocusValue()  override;
    float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValueMin()  override;
    float  VUFORIA_DRIVER_CALLING_CONVENTION getFocusValueMax()  override;
    float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValue()  override;
    bool VUFORIA_DRIVER_CALLING_CONVENTION setFocusValue(float)  override;

    void invokeEngineCallback(VuforiaDriver::CameraFrame* frame);

private:
    VuforiaDriver::CameraCallback* m_EngineCameraCallback{ nullptr };
    VuforiaDriver::PlatformData* m_PlatformData{ nullptr };
    DriverUserData* m_UserData{ nullptr };

    MediaCaptureAdapter m_Adapter;
};