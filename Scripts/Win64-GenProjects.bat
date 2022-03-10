@ECHO OFF
PUSHD ..\
call ThirdParty\premake\premake5.exe vs2022
POPD
PAUSE