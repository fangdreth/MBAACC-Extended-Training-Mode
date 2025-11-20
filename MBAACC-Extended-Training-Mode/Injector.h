#pragma once

#include <string>

#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define CYAN "\x1b[36;1m"
#define WHITE "\x1b[37;1m"

#define RESET "\x1b[0m"

int inject(unsigned long procID, std::wstring dllPath);

bool InjectIntoMBAA(unsigned long nPID, std::wstring sDLLPath);
