#include "pch.h"
#include "MediaCaptureDriver.h"

MediaCaptureDriver::MediaCaptureDriver(Vuforia::Driver::PlatformData* platformData)
	: mPlatformData(platformData)
{
}

MediaCaptureDriver::~MediaCaptureDriver()
{
}

Vuforia::Driver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureDriver::createExternalCamera()
{
	if (mExternalCamera == nullptr)
	{
		mExternalCamera = new MediaCaptureCamera(mPlatformData);
		return mExternalCamera;
	}

	// creating multiple cameras considered an error
	return nullptr;
}

void VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureDriver::destroyExternalCamera(Vuforia::Driver::ExternalCamera* instance)
{
	if (instance == mExternalCamera)
	{
		delete static_cast<MediaCaptureCamera*>(instance);
		mExternalCamera = nullptr;
	}
}