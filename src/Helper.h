#pragma once

#include "pch.h"

using namespace winrt;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;

struct __declspec(uuid("5b0d3235-4dba-4d44-865e-8f1d0e4fd04d")) __declspec(novtable) IMemoryBufferByteAccess : ::IUnknown
{
	virtual HRESULT __stdcall GetBuffer(uint8_t** value, uint32_t* capacity) = 0;
};

class Helper 
{
public:
	static void GetPointerToBitmapData(SoftwareBitmap bitmap, unsigned char** pPixelData, unsigned int* capacity, int* stride)
	{
		BitmapBuffer buffer = bitmap.LockBuffer(BitmapBufferAccessMode::ReadWrite);
		IMemoryBufferReference reference = buffer.CreateReference();

		auto descr = buffer.GetPlaneDescription(0);
		*stride = descr.Stride;

		auto interop = reference.as<IMemoryBufferByteAccess>();
		check_hresult(interop->GetBuffer(pPixelData, capacity));
	}
};