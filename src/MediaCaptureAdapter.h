#pragma once

#include "DriverUserData.h"
#include "Helper.h"
#include "FormatConverter.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

using namespace Windows::Media::Capture;
using namespace Windows::Media::Capture::Frames;

struct Model {
	Model(MediaFrameFormat format):
		format(format),
		conversionRequired(false)
	{
		mode.width = format.VideoFormat().Width();
		mode.height = format.VideoFormat().Height();
		mode.fps = uint32_t(format.FrameRate().Numerator() / format.FrameRate().Denominator());
	}

	VuforiaDriver::CameraMode mode;
	MediaFrameFormat format;

	bool conversionRequired; 
};

class Camera;

class MediaCaptureAdapter
{
public:
	MediaCaptureAdapter(Camera* camera);
	~MediaCaptureAdapter();

	IAsyncOperation<int> OpenMediaCapture(std::string name);
	IAsyncAction StopMediaCapture();

	void CollectCameraModes(Kind kind);
	std::vector<Model> SupportedModes() const& { return m_supportedCameraModes; }


	IAsyncOperation<bool> StartFrameReaderWithMode(VuforiaDriver::CameraMode const& mode);

	IAsyncAction Cleanup();

private:
	IAsyncAction InitializeMediaCapture();

	void OnFrameArrived(MediaFrameReader const& reader, MediaFrameArrivedEventArgs const& args);
	void ExtractFormats(MediaFrameSource const& source, bool useConversion = false);

	MediaCapture m_mediaCapture;
	MediaFrameReader m_reader;
	MediaFrameSourceGroup m_selectedGroup;
	MediaFrameSource m_source;
	event_token m_frameArrivedToken;

	Camera* m_camera;

	std::vector<Model> m_supportedCameraModes;
	
	const Model* m_selectedModel;
	FormatConverter* m_converter;
};