@echo off

rem copy keplermath
copy .\Kepler\Dependencies\KeplerMath\lib\KeplerMath.dll .\bin\Debug-x64\Client
copy .\Kepler\Dependencies\KeplerMath\lib\KeplerMath.dll .\bin\Release-x64\Client

copy .\Kepler\Dependencies\KeplerMath\lib\KeplerMath.dll .\bin\Debug-x64\Telescope
copy .\Kepler\Dependencies\KeplerMath\lib\KeplerMath.dll .\bin\Release-x64\Telescope

rem copy fmod
copy .\Kepler\Dependencies\fmod\lib\x64\fmod.dll .\bin\Debug-x64\Client
copy .\Kepler\Dependencies\fmod\lib\x64\fmodL.dll .\bin\Debug-x64\Client
copy .\Kepler\Dependencies\fmod\lib\x64\fmod.dll .\bin\Release-x64\Client
copy .\Kepler\Dependencies\fmod\lib\x64\fmodL.dll .\bin\Release-x64\Client

copy .\Kepler\Dependencies\fmod\lib\x64\fmod.dll .\bin\Debug-x64\Telescope
copy .\Kepler\Dependencies\fmod\lib\x64\fmodL.dll .\bin\Debug-x64\Telescope
copy .\Kepler\Dependencies\fmod\lib\x64\fmod.dll .\bin\Release-x64\Telescope
copy .\Kepler\Dependencies\fmod\lib\x64\fmodL.dll .\bin\Release-x64\Telescope

rem copy resources(shaders, fonts, etc.)
IF NOT EXIST .\bin\Debug-x64\Resources\ (
    mkdir .\bin\Debug-x64\Resources\
)
xcopy .\Kepler\Resources\*.* .\bin\Debug-x64\Resources /e /h /k
