#pragma once
#include <VuforiaEngine/Driver/Driver.h>
#include "MediaCaptureCamera.h"

#include "DriverUserData.h"

class MediaCaptureDriver : public VuforiaDriver::Driver
{
public:
	MediaCaptureDriver(VuforiaDriver::PlatformData* platformData, DriverUserData* userData);
	~MediaCaptureDriver();

	VuforiaDriver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION createExternalCamera() override;
	void VUFORIA_DRIVER_CALLING_CONVENTION destroyExternalCamera(VuforiaDriver::ExternalCamera* instance) override;

	uint32_t VUFORIA_DRIVER_CALLING_CONVENTION getCapabilities() override;

private:
	VuforiaDriver::PlatformData* m_PlatformData{ nullptr };

	DriverUserData* m_UserData{ nullptr };
	MediaCaptureCamera* m_ExternalCamera{ nullptr };
};
