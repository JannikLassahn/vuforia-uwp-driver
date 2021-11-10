# Vuforia Driver for UWP

This project demonstrates a way to use cameras that are not recognized or supported by Vuforia (UWP only). 
This is made possible by the [External Camera](https://library.vuforia.com/content/vuforia-library/en/articles/Solution/external-camera.html) feature from Vuforia and the lower-level [MediaCapture APIs](https://docs.microsoft.com/en-us/uwp/api/windows.media.capture.mediacapture). The driver is written in standard C++ thanks to [C++/WinRT](https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/).

## Requirements
* at least Vuforia SDK 10.2.5
* at least Windows 10 SDK 17134
* Visual Studio 2019 with workload for UWP development with C++

## Build

1. Download the Vuforia SDK for UWP [here](https://developer.vuforia.com/downloads/sdk)
2. Add an environment variable *VUFORIA_SDK* that points to the *\build\\* directoy inside the extracted SDK files, e.g. *C:\SDK\vuforia-sdk-uwp-10-2-5\build\\*
3. Open *VuforiaUwpDriver.sln* in Visual Studio, make sure to select *Release* and the correct platform (x86, x64 or ARM64) then start building (CTRL+SHIFT+B) or run *build.cmd* in the Developer Command Prompt for VS2019.

## Usage

You can configure the library through the [DriverUserData](./src/DriverUserData.h) struct.
Use it to chose a specific camera or camera kind, e.g. color or depth.
Per default the driver will choose the first color camera it can find.
In C++:
```cpp
// declare it
DriverUserData userData;

// and use it like this to set your camera
userData.cameraName = "Microsoft LifeCam Rear";
userData.cameraKind = Kind::Color;
Vuforia::setDriverLibrary("VuforiaUwpDriver.dll", &userData);
```

Take a look at the [VuforiaSetup.cs](./sample/UnityExternalCamera/Assets/Scripts/VuforiaSetup.cs) script for usage with Unity and C#. 

## Sample

The sample folder contains a project for Unity 2020.3 that shows how to setup the driver library. 

Steps to run this sample:
1. Make sure you have the correct version of the Vuforia SDK for Unity installed. You can check the version by selecting the VuforiaConfiguration.asset in the project explorer.
2. Enter a valid license key for Vuforia to enable the custom driver feature
3. Copy the DLLs you built to the respective plugin folders, e.g. the VuforiaUwpDriver.dll for x86 has to be copied to *Plugins/WSA/x86*
4. Build the project (CTRL+B)