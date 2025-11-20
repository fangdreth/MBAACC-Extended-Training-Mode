#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <format>
#include <fstream>

#include <ws2tcpip.h>
#include <winsock2.h>
#include <Windows.h>

/*
static HANDLE m_hConsole;
static std::string m_sLogFile;
static std::ofstream m_fLogFile;
static bool bLogOpen = false;
*/

extern HANDLE m_hConsole;
extern std::string m_sLogFile;
extern std::ofstream m_fLogFile;
extern bool bLogOpen;

void LogInfo(const std::string& sInfo);

std::string Time();

void InitializeLogger(HANDLE hConsole);

void CloseLogger();

void LogInfo(const std::string& sInfo);

void LogError(const std::string& sError);

