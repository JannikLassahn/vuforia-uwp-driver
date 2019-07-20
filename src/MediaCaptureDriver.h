#pragma once
#include <Vuforia/Driver/Driver.h>
#include "MediaCaptureCamera.h"

class MediaCaptureDriver : public Vuforia::Driver::VuforiaDriver
{
public:
	MediaCaptureDriver(Vuforia::Driver::PlatformData* platformData);
	~MediaCaptureDriver();

	Vuforia::Driver::ExternalCamera* VUFORIA_DRIVER_CALLING_CONVENTION createExternalCamera() override;

	void VUFORIA_DRIVER_CALLING_CONVENTION destroyExternalCamera(Vuforia::Driver::ExternalCamera* instance) override;

private:
	Vuforia::Driver::PlatformData* mPlatformData{ nullptr };
	MediaCaptureCamera* mExternalCamera{ nullptr };
};

