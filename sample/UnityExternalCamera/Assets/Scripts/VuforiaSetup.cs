using System;
using System.Runtime.InteropServices;
using UnityEngine;
using Vuforia;

public enum CameraKind
{
    Custom = 0,
    Color = 1,
    Infrared = 2,
    Depth = 3,
}

struct DriverUserData
{
    public string CameraName;
    public CameraKind CameraKind;
}

public class VuforiaSetup : MonoBehaviour
{
    public string LibraryName;
    public bool ConfigureDriver;   

    public string CameraName;
    public CameraKind CameraKind = CameraKind.Color;

    private static DriverUserData _userData;
    private static IntPtr _userDataPointer = IntPtr.Zero;

    private void Awake()
    {
        if (ConfigureDriver)
        {
            _userData = new DriverUserData
            {
                CameraName = CameraName,
                CameraKind = CameraKind
            };

            _userDataPointer = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(DriverUserData)));
            Marshal.StructureToPtr(_userData, _userDataPointer, false);
        }

#if UNITY_EDITOR
        VuforiaApplication.Instance.Initialize();
#else
        VuforiaApplication.Instance.Initialize(LibraryName, _userDataPointer);
#endif
    }
}
