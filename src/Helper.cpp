#include "pch.h"

using namespace	Platform;

static void Log(String^ msg) 
{
	auto s = "[UWP-DRIVER]: " + msg + "\n";
	OutputDebugString(s->Data());
}
