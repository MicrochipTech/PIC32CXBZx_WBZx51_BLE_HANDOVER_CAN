@echo off 
if "%1" == "pre" goto prebuild
if "%1" == "post" goto postbuild
goto end

:prebuild
@echo "Pre-Build"
goto end

:postbuild
@echo "Post-Build"
copy dist\default\production\central_conn.X.production.signed.hex ..\..\..\..\hex\WBZ351_CentralDevice.hex
goto end

:end