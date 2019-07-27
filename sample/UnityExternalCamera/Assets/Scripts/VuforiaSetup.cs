using System;
using System.Runtime.InteropServices;
using UnityEngine;
using Vuforia;

struct DriverUserData
{
    public string cameraName;
}

public class VuforiaSetup : MonoBehaviour
{
    public string LibraryName;
    public bool ConfigureDriver;   

    public string CameraName;

    private static DriverUserData _userData;
    private static IntPtr _userDataPointer = IntPtr.Zero;

    private void Awake()
    {
        if(ConfigureDriver)
        {

            _userData = new DriverUserData
            {
                cameraName = CameraName 
            };

            _userDataPointer = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DriverUserData)));
            Marshal.StructureToPtr(_userData, _userDataPointer, false);
        }
        else
        {
            _userDataPointer = IntPtr.Zero;
        }

        var isDriverLibrarySet = VuforiaUnity.SetDriverLibrary(LibraryName, _userDataPointer);
        if (!isDriverLibrarySet)
        {
            Debug.LogError("Failed to set driver library");
        }

        VuforiaRuntime.Instance.InitVuforia();
    }
}
