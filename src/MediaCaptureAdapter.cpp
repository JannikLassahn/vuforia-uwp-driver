#include "pch.h"
#include "MediaCaptureAdapter.h"
#include "Camera.h"

bool IsEqualCameraMode(VuforiaDriver::CameraMode const& a, VuforiaDriver::CameraMode const& b)
{
	return a.format == b.format &&
		a.fps == b.fps &&
		a.height == b.height &&
		a.width == b.width;
}

IAsyncAction MediaCaptureAdapter::InitializeMediaCapture()
{
	m_mediaCapture = MediaCapture();

	auto settings = MediaCaptureInitializationSettings();
	settings.SourceGroup(m_selectedGroup);
	settings.SharingMode(MediaCaptureSharingMode::ExclusiveControl);
	settings.StreamingCaptureMode(StreamingCaptureMode::Video);
	settings.MemoryPreference(MediaCaptureMemoryPreference::Cpu);

	return m_mediaCapture.InitializeAsync(settings);
}

MediaCaptureAdapter::MediaCaptureAdapter(Camera* camera) :
	m_camera(camera),
	m_reader(nullptr),
	m_mediaCapture(nullptr),
	m_selectedGroup(nullptr),
	m_source(nullptr),
	m_selectedModel(nullptr)
{
}

MediaCaptureAdapter::~MediaCaptureAdapter()
{
	delete m_converter;
}

IAsyncOperation<int> MediaCaptureAdapter::OpenMediaCapture(std::string name)
{
	auto groups = co_await MediaFrameSourceGroup::FindAllAsync();
	if (groups.Size() == 0)
	{
		co_return 0;
	}

	if (name.empty())
	{
		m_selectedGroup = groups.GetAt(0);
	}

	auto h_name = winrt::to_hstring(name);
	for (auto const& group : groups)
	{
		if (group.DisplayName() == h_name)
		{
			m_selectedGroup = group;
			break;
		}
	}

	try
	{
		co_await InitializeMediaCapture();
		co_return 0;
	}
	catch (const std::exception&)
	{
		co_return 1;
	}

}

IAsyncAction MediaCaptureAdapter::StopMediaCapture()
{
	if (m_reader == nullptr)
	{
		return;
	}

	co_await m_reader.StopAsync();
}

IAsyncOperation<bool> MediaCaptureAdapter::StartFrameReaderWithMode(VuforiaDriver::CameraMode const& mode)
{
	if (m_mediaCapture == nullptr)
	{
		co_return false;
	}

	MediaFrameFormat format{ nullptr };

	for (auto const& e : std::as_const(m_supportedCameraModes)) {
		if (IsEqualCameraMode(e.mode, mode)) {
			m_selectedModel = &e;
			format = e.format;
			break;
		}
	}

	if (m_selectedModel->conversionRequired) 
	{
		m_converter = new FormatConverter(m_selectedModel->format);
	}

	co_await m_source.SetFormatAsync(format);
	m_reader = co_await m_mediaCapture.CreateFrameReaderAsync(m_source);
	m_frameArrivedToken = m_reader.FrameArrived({ this, &MediaCaptureAdapter::OnFrameArrived });
	auto status = co_await m_reader.StartAsync();

	bool result = false;

	switch (status)
	{
	case MediaFrameReaderStartStatus::Success:
		DebugLog("Successfully initialized frame reader");
		result = true;
		break;
	case MediaFrameReaderStartStatus::UnknownFailure:
		DebugLog("Unknown failure");
		break;
	case MediaFrameReaderStartStatus::DeviceNotAvailable:
		DebugLog("The camera device is not available");
		break;
	case MediaFrameReaderStartStatus::OutputFormatNotSupported:
		DebugLog("The ouput format is not supported");
		break;
	case MediaFrameReaderStartStatus::ExclusiveControlNotAvailable:
		DebugLog("Exclusive control is not available");
		break;
	}

	co_return result;
}

void MediaCaptureAdapter::OnFrameArrived(MediaFrameReader const& reader, MediaFrameArrivedEventArgs const&)
{
	auto frameReference = reader.TryAcquireLatestFrame();
	if (frameReference == nullptr)
	{
		return;
	}

	unsigned char* buffer = nullptr;
	unsigned int bufferSize = 0;
	int stride = 0;

	auto bitmap = frameReference.VideoMediaFrame().SoftwareBitmap();
	auto exposureTimeEnd = std::chrono::system_clock::now();

	Helper::GetPointerToBitmapData(bitmap, &buffer, &bufferSize, &stride);

	if (m_selectedModel->conversionRequired)
	{
		buffer = m_converter->Convert(buffer);
		bufferSize = m_converter->BufferSize();
	}

	VuforiaDriver::CameraFrame frame;
	frame.stride = stride;
	frame.buffer = buffer;
	frame.bufferSize = bufferSize;
	frame.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(exposureTimeEnd.time_since_epoch()).count();
	frame.width = frameReference.VideoMediaFrame().VideoFormat().Width();
	frame.height = frameReference.VideoMediaFrame().VideoFormat().Height();

	m_camera->invokeEngineCallback(&frame);

}

IAsyncAction MediaCaptureAdapter::Cleanup()
{
	if (m_reader)
	{
		m_reader.FrameArrived(m_frameArrivedToken);
		co_await m_reader.StopAsync();
	}
}

void MediaCaptureAdapter::CollectCameraModes(Kind kind)
{
	m_supportedCameraModes.clear();

	for (auto kvp : m_mediaCapture.FrameSources())
	{
		MediaFrameSource source = kvp.Value();
		MediaFrameSourceKind sourceKind = source.Info().SourceKind();

		if ((int)(sourceKind) != (int)kind)
		{
			break;
		}

		ExtractFormats(source);


		if (m_supportedCameraModes.empty())
		{
			// no supported formats found, lets check if we provide a converter
			ExtractFormats(source, true);
		}
	}
}

void MediaCaptureAdapter::ExtractFormats(MediaFrameSource const& source, bool withConversion)
{
	for (auto format : source.SupportedFormats())
	{
		auto subtype = winrt::to_string(format.Subtype());

		if (FormatConverter::IsSupportedFormat(subtype, withConversion))
		{
			auto pixelformat = FormatConverter::GetFormat(subtype);

			Model model(format);
			model.mode.format = pixelformat;
			model.conversionRequired = withConversion;

			m_supportedCameraModes.push_back(model);
		}
	}
}
