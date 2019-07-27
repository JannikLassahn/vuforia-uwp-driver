#pragma once
#include <Vuforia/Driver/Driver.h>
#include "MediaCaptureCamera.h"

#include "DriverUserData.h"

class MediaCaptureDriver : public Vuforia::Driver::VuforiaDriver
{
public:
	MediaCaptureDriver(Vuforia::Driver::PlatformData* platformData, DriverUserData* userData);
	~MediaCaptureDriver();

	Vuforia::Driver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION createExternalCamera() override;

	void VUFORIA_DRIVER_CALLING_CONVENTION destroyExternalCamera(Vuforia::Driver::ExternalCamera* instance) override;

private:
	Vuforia::Driver::PlatformData* m_PlatformData{ nullptr };

	DriverUserData* m_UserData{ nullptr };
	MediaCaptureCamera* m_ExternalCamera{ nullptr };
};
