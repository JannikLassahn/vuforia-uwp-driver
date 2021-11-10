using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(VuforiaSetup))]
public class VuforiaSetupEditor : Editor
{
    private SerializedProperty libraryProp;
    private SerializedProperty configureDriverProp;
    private SerializedProperty cameraNameProp;
    private SerializedProperty cameraKindProp;

    void OnEnable()
    {
        libraryProp = serializedObject.FindProperty(nameof(VuforiaSetup.LibraryName));
        cameraNameProp = serializedObject.FindProperty(nameof(VuforiaSetup.CameraName));
        cameraKindProp = serializedObject.FindProperty(nameof(VuforiaSetup.CameraKind));
        configureDriverProp = serializedObject.FindProperty(nameof(VuforiaSetup.ConfigureDriver));
    }

    public override void OnInspectorGUI()
    {
        serializedObject.Update();

        EditorGUILayout.PropertyField(libraryProp, new GUIContent("Custom Driver", "The name of the custom driver file including extension, e.g. 'driver.dll'"));
        EditorGUILayout.PropertyField(configureDriverProp, new GUIContent("Configure driver", "Enable to configure driver features"));

        if(configureDriverProp.boolValue)
        {
            EditorGUI.indentLevel++;
            EditorGUILayout.PropertyField(cameraNameProp, new GUIContent("Camera name", "The name of the camera to use. Defaults to the first camera the driver can find"));
            EditorGUILayout.PropertyField(cameraKindProp, new GUIContent("Camera kind", "Set this field when your camera provides multiple video streams, e.g. color and depth"));
            EditorGUI.indentLevel--;
        }

        serializedObject.ApplyModifiedProperties();
    }
}
