#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <format>
#include <fstream>

#include "..\Common\Common.h"

static HANDLE m_hConsole;
static std::string m_sLogFile;
static std::ofstream m_fLogFile;
static bool bLogOpen = false;

static void LogInfo(std::string sInfo);

static std::string Time()
{
	return std::format("{:%m-%d-%y_%OH-%OM-%OS}", std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() });
}

static void InitializeLogger(HANDLE hConsole)
{
	if (bLogOpen)
		return;

	m_hConsole = hConsole;

	std::string sTempPath = std::filesystem::temp_directory_path().string();
	std::string sTime = Time();

	m_sLogFile = sTempPath + "ExtendedTrainingMode__" + sTime + ".log";

	m_fLogFile.open(m_sLogFile);
	
	LogInfo(VERSION);

	bLogOpen = true;
}

static void CloseLogger()
{
	if (!bLogOpen) return;

	LogInfo("Closed Extended Training Mode");

	m_fLogFile.close();
	bLogOpen = false;
}

static void LogInfo(std::string sInfo)
{
	if (!bLogOpen) return;

	m_fLogFile << "\n";
	m_fLogFile << "*** INFO *** Time: " << Time() << "\n";
	m_fLogFile << "\n";
	m_fLogFile << "\t" << sInfo << "\n";
	m_fLogFile << "\n";

	m_fLogFile.flush();
}

static void LogError(std::string sError)
{
	if (!bLogOpen) return;

	m_fLogFile << "\n";
	m_fLogFile << "*** ERROR ***";
	m_fLogFile << "\tTime: " << Time() << "\n";
	m_fLogFile << "\n";
	m_fLogFile << "\t" << sError << "\n";
	m_fLogFile << "\n";

	m_fLogFile.flush();
}