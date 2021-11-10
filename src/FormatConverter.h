#pragma once
#include "pch.h"
#include <string>
#include <map>

typedef unsigned char uchar;

using namespace winrt;

class FormatConverter
{

public:
	static inline const std::map<std::string, VuforiaDriver::PixelFormat> supportedFormats = { 
		{ "YUYV", VuforiaDriver::PixelFormat::YUYV},
		{ "YUY2", VuforiaDriver::PixelFormat::YUYV},
		{ "NV12", VuforiaDriver::PixelFormat::NV12},
		{ "NV21", VuforiaDriver::PixelFormat::NV21},
	};

	static inline const std::map<std::string, VuforiaDriver::PixelFormat> supportedFormatsByConversion = {
		{ "L8", VuforiaDriver::PixelFormat::NV12 }
	};
	
	static bool IsSupportedFormat(const Windows::Media::Capture::Frames::MediaFrameFormat& format, bool includeConversion = false)
	{
		auto subtype = to_string(format.Subtype());
		return IsSupportedFormat(subtype, includeConversion);
	}

	static bool IsSupportedFormat(const std::string& format, bool includeConversion = false)
	{
		auto supported = supportedFormats.count(format);
		if (!supported && includeConversion)
		{
			supported = supportedFormatsByConversion.count(format);
		}
		return supported;
	}

	static VuforiaDriver::PixelFormat GetFormat(const std::string& format)
	{
		if (supportedFormats.count(format))
		{
			return supportedFormats.at(format);
		}

		if(supportedFormatsByConversion.count(format))
		{
			return supportedFormatsByConversion.at(format);
		}

		return VuforiaDriver::PixelFormat::UNKNOWN;
	}

	FormatConverter(const Windows::Media::Capture::Frames::MediaFrameFormat& format)
	{
		auto width = format.VideoFormat().Width();
		auto height = format.VideoFormat().Height();
		auto subtype = format.Subtype();

		if(subtype != L"L8") 
		{
			throw std::exception("L8 is currently the only supported format");
		}

		auto pixels = width * height * 3 / 2 ;
		m_buffer = calloc(pixels, sizeof(uchar));
		m_bufferSize = pixels * sizeof(uchar);
		m_sourceSize = width * height * sizeof(uchar);

		if (m_buffer == nullptr)
		{
			throw std::exception("Error allocating buffer");
		}
	}

	~FormatConverter()
	{
		free(m_buffer);
	}

	virtual uchar* Convert(void* frame)
	{
		memcpy(m_buffer, frame, m_sourceSize);
		return static_cast<uchar*>(m_buffer);
	}

	virtual uint32_t BufferSize()
	{
		return m_bufferSize;
	}

private:
	int m_bufferSize;
	int m_sourceSize;
	void* m_buffer;
};
