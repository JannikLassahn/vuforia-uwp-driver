#pragma once

enum class Kind {
    Custom = 0,
    Color = 1,
    Infrared = 2,
    Depth = 3,
};

struct DriverUserData {
    char* cameraName;
    Kind cameraKind;
};