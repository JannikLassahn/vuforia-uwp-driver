#pragma once

/// <summary>
/// This is an optional parameter that can be passed as the 'void* userData' 
/// parameter of Vuforia::setDriverLibrary() call.
/// </summary>
struct DriverUserData
{
	/// <summary>
	// The name of the camera to use. 
	/// If there is no camera with the specified name or no name is provided, the first camera will be used.
	/// </summary>
	const char* cameraName;

};

