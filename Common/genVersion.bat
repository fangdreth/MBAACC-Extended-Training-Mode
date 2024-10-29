rem @echo off


for /f "delims=" %%h in ('git rev-parse --short HEAD') do set GIT_HASH=%%h

git diff --quiet
if %errorlevel% neq 0 (
    set GIT_DIRTY=D
) else (
    set GIT_DIRTY=C
)

set GIT_VERSION=

rem Write to version.h
echo #pragma once > version.h
echo #define GIT_HASH "%GIT_HASH%" >> version.h
echo #define GIT_DIRTY "%GIT_DIRTY%" >> version.h
echo #define GIT_VERSION "%GIT_HASH%_%GIT_DIRTY%" >> version.h

rem yes, this was the easiest way to pass params through msbuild. by which i mean to just not pass them through msbuild. omfg

if "%BLEEDING%"=="^%BLEEDING^%" (
	set donothing=
) else (
	if "%BLEEDING%"=="" (
		set donothing=
	) else (
		echo #define BLEEDING 1 >> version.h
	)
)
