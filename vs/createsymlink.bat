:: The VS project is setup to be used in the "Show All Files"-mode, meaning it Solution Explorer
:: reflects the actual project directory.
:: To make sure the 'src' folder is included, we must create a symbolic link to the 'src' folder
:: which is what this script does

:: Script must be run in adminstrative mode

:: Make sure bat file is executed in its own directory, when running in admin mode
@setlocal enableextensions
@cd /d "%~dp0"

:: Create the symbolic link to the src file
mklink /D src ..\src & pause