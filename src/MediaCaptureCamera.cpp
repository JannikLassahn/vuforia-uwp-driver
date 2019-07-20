#include "pch.h"
#include "MediaCaptureCamera.h"

MediaCaptureCamera::MediaCaptureCamera(Vuforia::Driver::PlatformData* platformData)
{
}

MediaCaptureCamera::~MediaCaptureCamera()
{
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::open()
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::close()
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::start(Vuforia::Driver::CameraMode cameraMode, Vuforia::Driver::CameraCallback* cb)
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::stop()
{
	return false;
}

uint32_t VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getNumSupportedCameraModes()
{
	return 1;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getSupportedCameraMode(uint32_t index, Vuforia::Driver::CameraMode* out)
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::supportsExposureMode(Vuforia::Driver::ExposureMode parameter)
{
	return false;
}

Vuforia::Driver::ExposureMode VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getExposureMode()
{
	return Vuforia::Driver::ExposureMode::UNKNOWN;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::setExposureMode(Vuforia::Driver::ExposureMode mode)
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::supportsExposureValue()
{
	return false;
}

uint64_t VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getExposureValueMin()
{
	return 0;
}

uint64_t VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getExposureValueMax()
{
	return 0;
}

uint64_t VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getExposureValue()
{
	return 0;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::setExposureValue(uint64_t exposureTime)
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::supportsFocusMode(Vuforia::Driver::FocusMode parameter)
{
	return false;
}

Vuforia::Driver::FocusMode VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getFocusMode()
{
	return Vuforia::Driver::FocusMode::UNKNOWN;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::setFocusMode(Vuforia::Driver::FocusMode mode)
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::supportsFocusValue()
{
	return false;
}

float VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getFocusValueMin()
{
	return 0.0f;
}

float VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getFocusValueMax()
{
	return 0.0f;
}

float VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getFocusValue()
{
	return 0.0f;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::setFocusValue(float value)
{
	return false;
}