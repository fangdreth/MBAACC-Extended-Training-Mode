echo #include "version.h" > version.cpp

for /f "delims=" %%h in ('git rev-parse --short HEAD') do set GIT_HASH=%%h

git diff --quiet
if %errorlevel% neq 0 (
    set GIT_DIRTY=D
) else (
    set GIT_DIRTY=C
)

echo const char* GIT_HASH = "%GIT_HASH%"; >> version.cpp
echo const char* GIT_DIRTY = "%GIT_DIRTY%"; >> version.cpp
echo const char* GIT_VERSION = "%GIT_HASH%_%GIT_DIRTY%"; >> version.cpp

if "%BLEEDING%"=="^%BLEEDING^%" (
	echo const bool BLEEDING = false; >> version.cpp
) else (
	if "%BLEEDING%"=="" (
		echo const bool BLEEDING = false; >> version.cpp
	) else (
		echo const bool BLEEDING = true; >> version.cpp
	)
)
