#include "pch.h"

void DebugLog(const std::string& message)
{
    std::string messageWithTag = "[VuforiaUwpDriver] ";
    messageWithTag += message;
    messageWithTag += "\n";
    OutputDebugStringA(messageWithTag.c_str());
}
