devenv /build "Release|x86" src\VuforiaUwpDriver.sln
devenv /build "Release|x64" src\VuforiaUwpDriver.sln
devenv /build "Release|ARM64" src\VuforiaUwpDriver.sln

copy /Y src\Release\VuforiaUwpDriver\VuforiaUwpDriver.dll sample\UnityExternalCamera\Assets\Plugins\WSA\x86\
copy /Y src\x64\Release\VuforiaUwpDriver\VuforiaUwpDriver.dll sample\UnityExternalCamera\Assets\Plugins\WSA\x64\