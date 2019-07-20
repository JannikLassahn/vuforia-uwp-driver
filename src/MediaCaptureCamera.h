#pragma once
#include <Vuforia/Driver/Driver.h>

class MediaCaptureCamera final : public Vuforia::Driver::ExternalCamera
{
public:
	MediaCaptureCamera(Vuforia::Driver::PlatformData* platformData);
	~MediaCaptureCamera();

	bool VUFORIA_DRIVER_CALLING_CONVENTION open() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION close() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION start(Vuforia::Driver::CameraMode cameraMode, Vuforia::Driver::CameraCallback* cb) override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION stop() override;

	uint32_t VUFORIA_DRIVER_CALLING_CONVENTION getNumSupportedCameraModes() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION getSupportedCameraMode(uint32_t index, Vuforia::Driver::CameraMode* out) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsExposureMode(Vuforia::Driver::ExposureMode parameter) override;
	Vuforia::Driver::ExposureMode VUFORIA_DRIVER_CALLING_CONVENTION getExposureMode() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setExposureMode(Vuforia::Driver::ExposureMode mode) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsExposureValue() override;
	uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValueMin() override;
	uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValueMax() override;
	uint64_t VUFORIA_DRIVER_CALLING_CONVENTION getExposureValue() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setExposureValue(uint64_t exposureTime) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsFocusMode(Vuforia::Driver::FocusMode parameter) override;
	Vuforia::Driver::FocusMode VUFORIA_DRIVER_CALLING_CONVENTION getFocusMode() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setFocusMode(Vuforia::Driver::FocusMode mode) override;

	bool VUFORIA_DRIVER_CALLING_CONVENTION supportsFocusValue() override;
	float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValueMin() override;
	float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValueMax() override;
	float VUFORIA_DRIVER_CALLING_CONVENTION getFocusValue() override;
	bool VUFORIA_DRIVER_CALLING_CONVENTION setFocusValue(float value) override;
};

