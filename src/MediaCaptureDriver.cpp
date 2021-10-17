#include "pch.h"
#include "MediaCaptureDriver.h"

namespace
{
	MediaCaptureDriver* g_MediaCaptureDriverInstance = nullptr;
}


//=============================================================================
// VUFORIA EXTERNAL PROVIDER-API IMPLEMENTATION
//=============================================================================
extern "C"
{
	VuforiaDriver::Driver* vuforiaDriver_init(VuforiaDriver::PlatformData* platformData, void* userdata)
	{
		if (g_MediaCaptureDriverInstance == nullptr)
		{
			auto driverUserData = static_cast<DriverUserData*>(userdata);
			g_MediaCaptureDriverInstance = new MediaCaptureDriver(platformData, driverUserData);
			return g_MediaCaptureDriverInstance;
		}

		// Attempting to init multiple instances is considered an error
		return nullptr;
	}

	void vuforiaDriver_deinit(VuforiaDriver::Driver* instance)
	{
		if (instance == g_MediaCaptureDriverInstance)
		{
			delete static_cast<MediaCaptureDriver*>(instance);
			g_MediaCaptureDriverInstance = nullptr;
		}
	}

	uint32_t vuforiaDriver_getAPIVersion()
	{
		return VuforiaDriver::VUFORIA_DRIVER_API_VERSION;
	}

	uint32_t vuforiaDriver_getLibraryVersion(char* outString, const uint32_t maxLength)
	{
		std::string versionCode = "UwpDriver-v1";
		uint32_t numBytes = versionCode.size() > maxLength ? maxLength : versionCode.size();
		memcpy(outString, versionCode.c_str(), numBytes);
		return numBytes;
	}
}


//=============================================================================
// PUBLIC INTERFACE IMPLEMENTATION
//=============================================================================

MediaCaptureDriver::MediaCaptureDriver(VuforiaDriver::PlatformData* platformData, DriverUserData* userData)
	: m_PlatformData(platformData),
	m_UserData(userData)
{
}

MediaCaptureDriver::~MediaCaptureDriver()
{
}

VuforiaDriver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureDriver::createExternalCamera()
{
	if (m_ExternalCamera == nullptr)
	{
		m_ExternalCamera = new MediaCaptureCamera(m_PlatformData, m_UserData);
		return m_ExternalCamera;
	}

	// creating multiple cameras considered an error
	return nullptr;
}

void VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureDriver::destroyExternalCamera(VuforiaDriver::ExternalCamera* instance)
{
	if (instance == m_ExternalCamera)
	{
		delete static_cast<MediaCaptureCamera*>(instance);
		m_ExternalCamera = nullptr;
	}
}

uint32_t VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureDriver::getCapabilities()
{
	return (uint32_t)(VuforiaDriver::Capability::CAMERA_IMAGE);
}
