#pragma once

extern const char* GIT_HASH;
extern const char* GIT_DIRTY;
extern const char* GIT_VERSION;
extern const bool BLEEDING;
extern const char* GIT_FULL_VERSION;

extern "C" __declspec(dllexport) const char* getETMVersion();
