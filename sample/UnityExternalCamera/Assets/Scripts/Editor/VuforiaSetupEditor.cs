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

    void OnEnable()
    {
        libraryProp = serializedObject.FindProperty(nameof(VuforiaSetup.LibraryName));
        cameraNameProp = serializedObject.FindProperty(nameof(VuforiaSetup.CameraName));
        configureDriverProp = serializedObject.FindProperty(nameof(VuforiaSetup.ConfigureDriver));
    }

    public override void OnInspectorGUI()
    {
        serializedObject.Update();
        var script = target as VuforiaSetup;

        EditorGUILayout.PropertyField(libraryProp, new GUIContent(nameof(VuforiaSetup.LibraryName)));
        EditorGUILayout.PropertyField(configureDriverProp, new GUIContent(nameof(VuforiaSetup.ConfigureDriver)));

        if(configureDriverProp.boolValue)
        {
            EditorGUI.indentLevel++;
            EditorGUILayout.PropertyField(cameraNameProp, new GUIContent(nameof(VuforiaSetup.CameraName)));
            EditorGUI.indentLevel--;
        }

        serializedObject.ApplyModifiedProperties();
    }
}
