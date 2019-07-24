#include "pch.h"

using namespace	Platform;

static void Log(String^ tag, String^ msg) 
{
	auto s = tag + " : " + msg + "\n";
	OutputDebugString(s->Data());
}
