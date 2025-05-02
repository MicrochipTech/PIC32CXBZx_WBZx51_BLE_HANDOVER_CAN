@echo off 
if "%1" == "pre" goto prebuild
if "%1" == "post" goto postbuild
goto end

:prebuild
@echo "Pre-Build"
goto end

:postbuild
@echo "Post-Build"
copy dist\default\production\WBZ451HPE.X.production.hex ..\..\..\..\hex\WBZ451HPE_PeripheralDevice.hex
goto end

:end