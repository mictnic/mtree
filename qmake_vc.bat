@echo off
call "%VCINSTALLDIR%"\vcvars64.bat
set "targetdir=build"
if not exist "%targetdir%" (
	mkdir "%targetdir%"
	echo 创建目录 "%targetdir%"
) else (
	echo 目录"%targetdir%"已经存在
)

cd /d "%targetdir%"
echo 当前目录：%cd%

qmake -t vcapp ..

pause