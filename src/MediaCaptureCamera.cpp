#include "pch.h"
#include "MediaCaptureCamera.h"
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>

using namespace concurrency;

using namespace Platform;
using namespace Platform::Collections;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

using namespace Windows::Media::Capture;
using namespace Windows::Media::Capture::Frames;

#define LOG(s) OutputDebugString(s)

#pragma region Buffer Wrangling

MIDL_INTERFACE("5b0d3235-4dba-4d44-865e-8f1d0e4fd04d")
IMemoryBufferByteAccess : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE GetBuffer(
		BYTE * *value,
		UINT32 * capacity
		);
};

#pragma endregion

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


MediaCaptureCamera::MediaCaptureCamera(Vuforia::Driver::PlatformData* platformData)
{
}

MediaCaptureCamera::~MediaCaptureCamera()
{
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::open()
{

	//TODO: let user choose index either directly or with some ID
	int index = 0;

	auto task = create_task(MediaFrameSourceGroup::FindAllAsync())
		.then([this, index](IVectorView<MediaFrameSourceGroup^>^ allGroups)
			{
				if (allGroups->Size == 0)
				{
					LOG(L"No source groups found");
					return task_from_result(false);
				}

				m_sourceGroup = allGroups->GetAt(index);

				// Initialize MediaCapture with selected group
				return TryInitializeMediaCaptureAsync(m_sourceGroup)
					.then([this](bool initialized)
						{
							if (!initialized)
							{
								return CleanupResources();
							}

							return task_from_result(true);

						}, task_continuation_context::get_current_winrt_context());
			}, task_continuation_context::get_current_winrt_context());


	return task.get();
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::close()
{
	return false;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::start(Vuforia::Driver::CameraMode cameraMode, Vuforia::Driver::CameraCallback* cb)
{
	if (m_mediaCapture == nullptr)
	{
		return false;
	}

	m_callback = cb;

	auto source = GetGroupForCameraMode(cameraMode);
	auto task = create_task(m_mediaCapture->CreateFrameReaderAsync(source))
		.then([this](MediaFrameReader^ frameReader)
			{
				m_reader = frameReader;

				//TODO: remove use of wrapper class
				auto wrapper = ref new Wrapper(this);

				m_token = frameReader->FrameArrived +=
					ref new TypedEventHandler<MediaFrameReader^, MediaFrameArrivedEventArgs^>(wrapper, &Wrapper::FrameReader_FrameArrived);

				return create_task(m_reader->StartAsync());
			}, task_continuation_context::get_current_winrt_context())
		.then([](MediaFrameReaderStartStatus status)
			{
				if (status == MediaFrameReaderStartStatus::Success)
				{
					return task_from_result(true);
				}

				//TODO: do some loggin

				return task_from_result(false);
			}, task_continuation_context::get_current_winrt_context());
	
			
	task.wait();
	return task.get();
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::stop()
{
	return false;
}

uint32_t VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getNumSupportedCameraModes()
{
	if (m_mediaCapture == nullptr)
	{
		return 0;
	}

	m_supportedCameraModes.clear();

	for (auto kvp : m_mediaCapture->FrameSources)
	{
		MediaFrameSource^ source = kvp->Value;
		MediaFrameSourceKind kind = source->Info->SourceKind;

		// only use color frames
		if (kind != MediaFrameSourceKind::Color)
		{
			continue;
		}

		for (auto format : source->SupportedFormats)
		{
			//TODO: figure out pixel format

			Vuforia::Driver::CameraMode cameraMode;
			cameraMode.width = format->VideoFormat->Width;
			cameraMode.height = format->VideoFormat->Height;
			cameraMode.fps = uint32_t(format->FrameRate->Numerator / format->FrameRate->Denominator);

			m_supportedCameraModes.push_back(cameraMode);
		}
	}

	return m_supportedCameraModes.size();
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::getSupportedCameraMode(uint32_t index, Vuforia::Driver::CameraMode* out)
{
	if (index < 0 || index >= m_supportedCameraModes.size())
	{
		return false;
	}

	*out = m_supportedCameraModes[index];
	return true;
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::supportsExposureMode(Vuforia::Driver::ExposureMode parameter)
{
	return false;
}

#pragma region EXPOSURE AND FOCUS

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

#pragma endregion


MediaFrameSource^ MediaCaptureCamera::GetGroupForCameraMode(Vuforia::Driver::CameraMode mode)
{
	return nullptr;
}

task<bool> MediaCaptureCamera::CleanupResources()
{
	//if (m_mediaCapture == nullptr)
	//{
	//	return task_from_result(false);
	//}

	//m_reader->FrameArrived -= m_token;
	//return create_task(m_reader->StopAsync())
	//	.then([this]()
	//		{
	//			m_mediaCapture = nullptr;
	//			return task_from_result(true);
	//		});

	return task_from_result(true);
}

task<bool> MediaCaptureCamera::TryInitializeMediaCaptureAsync(MediaFrameSourceGroup^ group)
{
	if (m_mediaCapture != nullptr)
	{
		// Already initialized.
		return task_from_result(true);
	}

	m_mediaCapture = ref new MediaCapture();

	auto settings = ref new MediaCaptureInitializationSettings();
	settings->SourceGroup = group;
	settings->SharingMode = MediaCaptureSharingMode::SharedReadOnly;
	settings->StreamingCaptureMode = StreamingCaptureMode::Video;
	settings->MemoryPreference = MediaCaptureMemoryPreference::Cpu;
	settings->StreamingCaptureMode = StreamingCaptureMode::Video;

	// Initialization must occur on the UI thread because some device families 
	// will prompt the user to grant consent for the app to access cameras.
	// This can raise an exception if the source no longer exists,
	// or if the source could not be initialized.
	return create_task(m_mediaCapture->InitializeAsync(settings))
		.then([this](task<void> initializeMediaCaptureTask)
			{
				try
				{
					// Get the result of the initialization. This call will throw if initialization failed
					// This pattern is docuemnted at https://msdn.microsoft.com/en-us/library/dd997692.aspx
					initializeMediaCaptureTask.get();
					//LOG("MediaCapture is successfully initialized in shared mode.");
					return true;
				}
				catch (Exception^ exception)
				{
					//LOG(L"Failed to initialize media capture: " + exception->Message);
					return false;
				}
			});
}

void MediaCaptureCamera::FrameReader_FrameArrived(MediaFrameReader^ sender, MediaFrameArrivedEventArgs^ args)
{
	auto frameReference = sender->TryAcquireLatestFrame();
	if (frameReference == nullptr)
	{
		return;
	}

	auto frameBuffer = frameReference->VideoMediaFrame->SoftwareBitmap->LockBuffer(Windows::Graphics::Imaging::BitmapBufferAccessMode::ReadWrite);
	auto reference = frameBuffer->CreateReference();

	Microsoft::WRL::ComPtr<IMemoryBufferByteAccess> bufferByteAccess;
	HRESULT result = reinterpret_cast<IInspectable*>(reference)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));

	if (result == S_OK)
	{
		BYTE* data = nullptr;
		UINT32 capacity = 0;

		result = bufferByteAccess->GetBuffer(&data, &capacity);

		if (result == S_OK)
		{
			Vuforia::Driver::CameraFrame frame;
			frame.buffer = data;
			frame.bufferSize = capacity;
			frame.width = frameReference->VideoMediaFrame->VideoFormat->Width;
			frame.height = frameReference->VideoMediaFrame->VideoFormat->Height;

			//TODO: set other fields, e.g. format, stride, ...

			if (m_callback)
			{
				m_callback->onNewCameraFrame(&frame);
			}
			else
			{
				LOG(L"Camera frame callback to Vuforia is not found");
			}
		}

	}
}
