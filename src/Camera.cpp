#include "pch.h"
#include "Camera.h"

Camera::Camera(VuforiaDriver::PlatformData* platformData, DriverUserData* userData) :
	m_PlatformData(platformData),
	m_UserData(userData),
	m_Adapter(this)
{
}

bool Camera::open()
{
	std::string name = (m_UserData == nullptr || m_UserData->cameraName == nullptr) ? "" : m_UserData->cameraName;
	return m_Adapter.OpenMediaCapture(name).get();
}

bool Camera::close()
{
	m_Adapter.StopMediaCapture().get();
	return true;
}

bool Camera::start(VuforiaDriver::CameraMode mode, VuforiaDriver::CameraCallback* cb)
{
	m_EngineCameraCallback = cb;
	return m_Adapter.StartFrameReaderWithMode(mode).get();
}

bool Camera::stop()
{
	m_Adapter.StopMediaCapture().get();
	return true;
}

uint32_t Camera::getNumSupportedCameraModes()
{
	auto kind = (m_UserData == nullptr) ? Kind::Color : m_UserData->cameraKind;
	m_Adapter.CollectCameraModes(kind);
	return m_Adapter.SupportedModes().size();
}

bool Camera::getSupportedCameraMode(uint32_t index, VuforiaDriver::CameraMode* out)
{
	if (index < 0 || index >= m_Adapter.SupportedModes().size())
	{
		return false;
	}

	*out = m_Adapter.SupportedModes()[index].mode;
	return true;
}

bool Camera::supportsExposureMode(VuforiaDriver::ExposureMode)
{
	return false;
}

VuforiaDriver::ExposureMode Camera::getExposureMode()
{
	return VuforiaDriver::ExposureMode::UNKNOWN;
}

bool Camera::setExposureMode(VuforiaDriver::ExposureMode)
{
	return false;
}

bool Camera::supportsExposureValue()
{
	return false;
}

uint64_t Camera::getExposureValueMin()
{
	return 0;
}

uint64_t Camera::getExposureValueMax()
{
	return 0;
}

uint64_t Camera::getExposureValue()
{
	return 0;
}

bool Camera::setExposureValue(uint64_t)
{
	return false;
}

bool Camera::supportsFocusMode(VuforiaDriver::FocusMode)
{
	return false;
}

VuforiaDriver::FocusMode Camera::getFocusMode()
{
	return VuforiaDriver::FocusMode::UNKNOWN;
}

bool Camera::setFocusMode(VuforiaDriver::FocusMode)
{
	return false;
}

bool Camera::supportsFocusValue()
{
	return false;
}

float Camera::getFocusValueMin()
{
	return 0.0f;
}

float Camera::getFocusValueMax()
{
	return 0.0f;
}

float Camera::getFocusValue()
{
	return 0.0f;
}

bool Camera::setFocusValue(float)
{
	return false;
}

void Camera::invokeEngineCallback(VuforiaDriver::CameraFrame* frame)
{
	if (m_EngineCameraCallback)
	{
		m_EngineCameraCallback->onNewCameraFrame(frame);
	}
}
