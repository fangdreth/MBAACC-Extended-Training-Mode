@echo off
rem this was the easiest way :(

for /f "delims=" %%i in ('powershell -Command "[DateTime]::UtcNow.ToString('u')"') do set utcTime=%%i
echo const char* compileTimestring = "%utcTime%"; > Time.cpp
