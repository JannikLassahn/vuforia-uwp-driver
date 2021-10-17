#pragma once
#include <VuforiaEngine/Driver/Driver.h>
#include "DriverUserData.h"

ref class Wrapper;

class  MediaCaptureCamera : public VuforiaDriver::ExternalCamera
{
public:
	MediaCaptureCamera(VuforiaDriver::PlatformData* platformData, DriverUserData* userData);
	~MediaCaptureCamera();

	bool VUFORIA_DRIVER_CALLING_CONVENTION open() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION close() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION start(VuforiaDriver::CameraMode cameraMode, VuforiaDriver::CameraCallback* cb) override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION stop() override;

	uint32_t VUFORIA_DRIVER_CALLING_CONVENTION getNumSupportedCameraModes() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION getSupportedCameraMode(uint32_t index, VuforiaDriver::CameraMode* out) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsExposureMode(VuforiaDriver::ExposureMode parameter) override;
	VuforiaDriver::ExposureMode VUFORIA_DRIVER_CALLING_CONVENTION getExposureMode() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setExposureMode(VuforiaDriver::ExposureMode mode) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsExposureValue() override;
	uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValueMin() override;
	uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValueMax() override;
	uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValue() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setExposureValue(uint64_t exposureTime) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsFocusMode(VuforiaDriver::FocusMode parameter) override;
	VuforiaDriver::FocusMode VUFORIA_DRIVER_CALLING_CONVENTION getFocusMode() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setFocusMode(VuforiaDriver::FocusMode mode) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsFocusValue() override;
	float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValueMin() override;
	float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValueMax() override;
	float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValue() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setFocusValue(float value) override;

	void FrameReader_FrameArrived(
		Windows::Media::Capture::Frames::MediaFrameReader^ sender,
		Windows::Media::Capture::Frames::MediaFrameArrivedEventArgs^ args);


private:
	Windows::Media::Capture::Frames::MediaFrameSource^ GetGroupForCameraMode(VuforiaDriver::CameraMode mode);
	bool GetPointerToPixelData(Windows::Graphics::Imaging::SoftwareBitmap^ bitmap, unsigned char** pPixelData, unsigned int* capacity, int* stride);

	concurrency::task<bool> CleanupResources();
	concurrency::task<bool> TryInitializeMediaCaptureAsync(
		Windows::Media::Capture::Frames::MediaFrameSourceGroup^ group);

	VuforiaDriver::CameraCallback* m_callback{ nullptr };
	
	DriverUserData* m_userData{ nullptr };

	std::vector<VuforiaDriver::CameraMode> m_supportedCameraModes;

	Windows::Foundation::EventRegistrationToken m_token;
	Windows::Media::Capture::Frames::MediaFrameReader^ m_reader;
	Windows::Media::Capture::Frames::MediaFrameSourceGroup^ m_selectedSourceGroup;

	Platform::Agile<Windows::Media::Capture::MediaCapture> m_mediaCapture;
	Wrapper^ m_wrapper;
};

#pragma region Workaround for WinRT callback

//TODO: remove this wrapper and use smart pointers if possible

ref class Wrapper sealed
{
internal:
	Wrapper(MediaCaptureCamera* mediaCaptureCamera)
		: m_mediaCaptureCamera(mediaCaptureCamera)
	{}

	void FrameReader_FrameArrived(
		Windows::Media::Capture::Frames::MediaFrameReader^ sender,
		Windows::Media::Capture::Frames::MediaFrameArrivedEventArgs^ args) {
		m_mediaCaptureCamera->FrameReader_FrameArrived(sender, args);
	}
private:
	MediaCaptureCamera* m_mediaCaptureCamera;

};

#pragma endregion