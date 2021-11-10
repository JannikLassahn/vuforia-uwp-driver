#pragma once

#include "Camera.h"

using namespace winrt;

class Driver final : public VuforiaDriver::Driver
{
public:
    Driver(VuforiaDriver::PlatformData* platformData, DriverUserData* userdata);

    VuforiaDriver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION createExternalCamera() override;
    void VUFORIA_DRIVER_CALLING_CONVENTION destroyExternalCamera(VuforiaDriver::ExternalCamera* instance) override;
    uint32_t VUFORIA_DRIVER_CALLING_CONVENTION getCapabilities() override;

private:

    VuforiaDriver::PlatformData* mPlatformData{ nullptr };
    DriverUserData* mUserdata{ nullptr };
    Camera* mExternalCamera{ nullptr };
};
