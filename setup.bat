@echo off

rem copy keplermath
copy .\Kepler\Dependencies\KeplerMath\lib\KeplerMath.dll .\bin\Debug-x64\Client
copy .\Kepler\Dependencies\KeplerMath\lib\KeplerMath.dll .\bin\Release-x64\Client

rem copy fmod
copy .\Kepler\Dependencies\fmod\lib\x64\fmod.dll .\bin\Debug-x64\Client
copy .\Kepler\Dependencies\fmod\lib\x64\fmodL.dll .\bin\Debug-x64\Client
copy .\Kepler\Dependencies\fmod\lib\x64\fmod.dll .\bin\Release-x64\Client
copy .\Kepler\Dependencies\fmod\lib\x64\fmodL.dll .\bin\Release-x64\Client