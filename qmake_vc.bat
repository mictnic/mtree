@echo off
call "%VCINSTALLDIR%"\vcvars64.bat
set "targetdir=build"
if not exist "%targetdir%" (
	mkdir "%targetdir%"
	echo ����Ŀ¼ "%targetdir%"
) else (
	echo Ŀ¼"%targetdir%"�Ѿ�����
)

cd /d "%targetdir%"
echo ��ǰĿ¼��%cd%

qmake -t vcapp ..

pause