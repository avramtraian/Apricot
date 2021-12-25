@ECHO OFF
PUSHD ..\
call Vendor\premake\premake5.exe vs2022
POPD
PAUSE