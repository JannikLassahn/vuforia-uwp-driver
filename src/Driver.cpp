#include "pch.h"
#include "Driver.h"

using namespace winrt;

namespace
{
    Driver* g_driverInstance = nullptr;
}

extern "C"
{
    VuforiaDriver::Driver* vuforiaDriver_init(VuforiaDriver::PlatformData* platformData, void* userdata)
    {
        if (g_driverInstance == nullptr)
        {
            g_driverInstance = new Driver(platformData, static_cast<DriverUserData*>(userdata));
            return g_driverInstance;
        }
        return nullptr;
    }

    void vuforiaDriver_deinit(VuforiaDriver::Driver* instance)
    {
        if (instance == g_driverInstance)
        {
            delete static_cast<Driver*>(instance);
            g_driverInstance = nullptr;
        }
    }

    uint32_t vuforiaDriver_getAPIVersion()
    {
        return VuforiaDriver::VUFORIA_DRIVER_API_VERSION;
    }

    uint32_t vuforiaDriver_getLibraryVersion(char* outString, const uint32_t maxLength)
    {
        std::string versionCode = "VuforiaUwpDriver-v1";
        uint32_t numBytes = static_cast<uint32_t>(versionCode.size() > maxLength ? maxLength : versionCode.size());
        memcpy(outString, versionCode.c_str(), numBytes);
        return numBytes;
    }
}

Driver::Driver(VuforiaDriver::PlatformData* platformData, DriverUserData* userdata)
    : mPlatformData (platformData),
      mUserdata(userdata),
      mExternalCamera(nullptr)
{
}

VuforiaDriver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION Driver::createExternalCamera()
{
    if (mExternalCamera == nullptr)
    {
        mExternalCamera = new Camera(mPlatformData, static_cast<DriverUserData*>(mUserdata));
        return mExternalCamera;
    }
    return nullptr;
}

void VUFORIA_DRIVER_CALLING_CONVENTION Driver::destroyExternalCamera(VuforiaDriver::ExternalCamera* instance)
{
    if (instance == mExternalCamera)
    {
        delete static_cast<Camera*>(instance);

        mExternalCamera = nullptr;
    }
}

uint32_t VUFORIA_DRIVER_CALLING_CONVENTION Driver::getCapabilities()
{
    return (uint32_t)(VuforiaDriver::Capability::CAMERA_IMAGE);
}

