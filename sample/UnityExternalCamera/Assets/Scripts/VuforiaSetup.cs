using System;
using UnityEngine;
using Vuforia;

public class VuforiaSetup : MonoBehaviour
{
    public string LibraryName;

    private void Awake()
    {

        var userDataPtr = IntPtr.Zero;

        var isDriverLibrarySet = VuforiaUnity.SetDriverLibrary(LibraryName, userDataPtr);
        if (!isDriverLibrarySet)
        {
            Debug.LogError("Failed to set driver library");
        }

        VuforiaRuntime.Instance.InitVuforia();
    }
}
