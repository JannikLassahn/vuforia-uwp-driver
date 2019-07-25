#include "pch.h"
#include "MediaCaptureCamera.h"
#include <Helper.cpp>

#include "MemoryBuffer.h"

#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>

using namespace Microsoft::WRL;

using namespace concurrency;

using namespace Platform;
using namespace Platform::Collections;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;

using namespace Windows::Media::Capture;
using namespace Windows::Media::Capture::Frames;


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

	auto findAllGroupsTask = create_task(MediaFrameSourceGroup::FindAllAsync());
	auto initCaptureTask = findAllGroupsTask.then([this, index](IVectorView<MediaFrameSourceGroup^>^ allGroups)
		{
			if (allGroups->Size == 0)
			{
				Log("DRIVER", "No source groups found");
				return task_from_result(false);
			}

			m_selectedSourceGroup = allGroups->GetAt(index);

			Log("DRIVER", "Trying source group '" + m_selectedSourceGroup->DisplayName + "'");

			// Initialize MediaCapture with selected group
			return TryInitializeMediaCaptureAsync(m_selectedSourceGroup);
		}, task_continuation_context::get_current_winrt_context());
	auto initCleanupTask = initCaptureTask.then([this](bool initialized)
		{
			if (!initialized)
			{
				return CleanupResources();
			}

			Log("DRIVER", "Successfully initialized camera");

			return task_from_result(true);

		}, task_continuation_context::get_current_winrt_context());

	initCleanupTask.wait();
	return initCleanupTask.get();
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

	if (source == nullptr)
	{
		return false;
	}

	auto task = create_task(m_mediaCapture->CreateFrameReaderAsync(source));
	auto task2 = task.then([this](MediaFrameReader^ frameReader)
		{
			m_reader = frameReader;

			//TODO: remove use of wrapper class
			m_wrapper = ref new Wrapper(this);

			m_token = frameReader->FrameArrived +=
				ref new TypedEventHandler<MediaFrameReader^, MediaFrameArrivedEventArgs^>(m_wrapper, &Wrapper::FrameReader_FrameArrived);

			return create_task(m_reader->StartAsync());
		}, task_continuation_context::get_current_winrt_context());
	auto task3 = task2.then([](MediaFrameReaderStartStatus status)
		{
			bool result = false;

			switch (status)
			{
			case MediaFrameReaderStartStatus::Success:
				result = true;
				Log("DRIVER", "Successfully initialized frame reader");
				break;
			case MediaFrameReaderStartStatus::UnknownFailure:
				Log("DRIVER", "Unknown failure");
				break;
			case MediaFrameReaderStartStatus::DeviceNotAvailable:
				Log("DRIVER", "The camera device is not available");
				break;
			case MediaFrameReaderStartStatus::OutputFormatNotSupported:
				Log("DRIVER", "The ouput format is not supported");
				break;
			case MediaFrameReaderStartStatus::ExclusiveControlNotAvailable:
				Log("DRIVER", "Exclusive control is not available");
				break;
			}

			return task_from_result(result);
		}, task_continuation_context::get_current_winrt_context());

	task3.wait();
	return task3.get();
}

bool VUFORIA_DRIVER_CALLING_CONVENTION MediaCaptureCamera::stop()
{
	if (m_reader == nullptr)
	{
		return false;
	}

	auto task = create_task(m_reader->StopAsync());
	task.wait();
	return true;
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
			auto pixelFormat = Vuforia::Driver::PixelFormat::UNKNOWN;
			auto subtype = format->Subtype;

			if (subtype == "YUYV")
			{
				pixelFormat = Vuforia::Driver::PixelFormat::YUYV;
			}
			else if (subtype == "NV12")
			{
				pixelFormat = Vuforia::Driver::PixelFormat::NV12;
			}
			else if (subtype == "NV21")
			{
				pixelFormat = Vuforia::Driver::PixelFormat::NV21;
			}

			Vuforia::Driver::CameraMode cameraMode;
			cameraMode.format = pixelFormat;
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
	auto isFormatSet = mode.format != Vuforia::Driver::PixelFormat::UNKNOWN;
	String^ subtype;

	if (isFormatSet)
	{
		if (mode.format == Vuforia::Driver::PixelFormat::NV12)
			subtype = "NV12";
		else if (mode.format == Vuforia::Driver::PixelFormat::NV21)
			subtype = "NV21";
		else if (mode.format == Vuforia::Driver::PixelFormat::YUYV)
			subtype = "YUYV";
	}

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
			if (format->VideoFormat->Width == mode.width &&
				format->VideoFormat->Height == mode.height)
			{
				if (isFormatSet && subtype != format->Subtype)
					continue;

				auto task = create_task(source->SetFormatAsync(format));

				try
				{
					task.wait();
					return source;
				}
				catch (Exception^ ex)
				{
					return nullptr;
				}
			}
		}
	}

	return nullptr;
}

bool MediaCaptureCamera::GetPointerToPixelData(Windows::Graphics::Imaging::SoftwareBitmap^ bitmap, unsigned char** pPixelData, unsigned int* capacity, int *stride)
{
	BitmapBuffer^ bmpBuffer = bitmap->LockBuffer(BitmapBufferAccessMode::ReadWrite);
	IMemoryBufferReference^ reference = bmpBuffer->CreateReference();

	auto descr = bmpBuffer->GetPlaneDescription(0);
	*stride = descr.Stride;

	ComPtr<IMemoryBufferByteAccess> pBufferByteAccess;
	if ((reinterpret_cast<IInspectable*>(reference)->QueryInterface(IID_PPV_ARGS(&pBufferByteAccess))) != S_OK)
	{
		return false;
	}

	if (pBufferByteAccess->GetBuffer(pPixelData, capacity) != S_OK)
	{
		return false;
	}
	return true;
}

task<bool> MediaCaptureCamera::CleanupResources()
{
	if (m_mediaCapture == nullptr)
	{
		return task_from_result(false);
	}

	m_reader->FrameArrived -= m_token;
	return create_task(m_reader->StopAsync())
		.then([this]()
			{
				m_mediaCapture = nullptr;
				return task_from_result(true);
			});

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
	settings->SharingMode = MediaCaptureSharingMode::ExclusiveControl;
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
					Log("DRIVER", "Failed to initialize media capture: " + exception->Message->ToString());
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

	unsigned char* pixelData = nullptr;
	unsigned int capacity = 0;
	int stride = 0;

	if (!GetPointerToPixelData(frameReference->VideoMediaFrame->SoftwareBitmap, &pixelData, &capacity, &stride))
	{
		Log("DRIVER", "Could not get pixel data from current frame");
		return;
	}

	auto exposureTimeEnd = std::chrono::system_clock::now();

	Vuforia::Driver::CameraFrame frame;
	frame.stride = stride;
	frame.buffer = pixelData;
	frame.bufferSize = capacity;
	frame.width = frameReference->VideoMediaFrame->VideoFormat->Width;
	frame.height = frameReference->VideoMediaFrame->VideoFormat->Height;
	frame.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(exposureTimeEnd.time_since_epoch()).count();


	//TODO: set other fields, e.g. format, stride, ...

	if (m_callback)
	{
		m_callback->onNewCameraFrame(&frame);
	}
	else
	{
		Log("DRIVER", "Camera frame callback to Vuforia is not found");
	}
}
