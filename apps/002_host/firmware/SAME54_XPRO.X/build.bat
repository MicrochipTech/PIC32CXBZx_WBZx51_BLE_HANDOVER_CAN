@echo off 
if "%1" == "pre" goto prebuild
if "%1" == "post" goto postbuild
goto end

:prebuild
@echo "Pre-Build"
goto end

:postbuild
@echo "Post-Build"
copy dist\default\production\SAME54_XPRO.X.production.hex ..\..\..\..\hex\SAME54_XPRO_Host.hex
goto end

:end