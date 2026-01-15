@echo off
if not exist build mkdir build
if not exist build\objects mkdir build\objects
if not exist build\sdl3.dll cp lib\SDL3\SDL3.dll build\sdl3.dll
if not exist build\SDL3_image.dll cp lib\SDL3\SDL3_image.dll build\SDL3_image.dll
if exist build\assets call rm -r build\assets
cp -r assets build\assets
setlocal enabledelayedexpansion

call node ./build.js
endlocal

if errorlevel 1 (
  echo Build failed.
  exit /b %errorlevel%
) else (
  echo Build succeeded. 
  cls
)

