# Vuforia Driver for UWP

This project demonstrates a way to use cameras that are not recognized or supported by Vuforia (UWP only). 
This is made possible by the [External Camera](https://library.vuforia.com/content/vuforia-library/en/articles/Solution/external-camera.html) feature from Vuforia and the lower-level [MediaCapture APIs](https://docs.microsoft.com/en-us/uwp/api/windows.media.capture.mediacapture).

The code is based on Vuforia SDK 8.3.8.

## Build

1. Download the Vuforia SDK for UWP [here](https://developer.vuforia.com/downloads/sdk)
2. Add an environment variable *VUFORIA_SDK* that points to the *\build\include* directoy inside the extracted SDK files, e.g. *C:\SDK\vuforia-sdk-uwp-8-3-8\build\include*
3. Open *VuforiaUwpDriver.sln* in Visual Studio and start building