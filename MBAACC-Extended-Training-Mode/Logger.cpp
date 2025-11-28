
#include "Logger.h"
#include <vector>
#include <format>
#include <fstream>
#include <chrono>
#include <filesystem>


//#include <ws2tcpip.h>
//#include <winsock2.h>

#include <Windows.h>

#include "..\Common\Common.h"

HANDLE m_hConsole;
std::string m_sLogFile;
std::ofstream m_fLogFile;
bool bLogOpen = false;

std::string Time()
{
	return std::format("{:%m-%d-%y_%OH-%OM-%OS}", std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() });
}

void InitializeLogger(HANDLE hConsole)
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

void CloseLogger()
{
	if (!bLogOpen) return;

	LogInfo("Closed Extended Training Mode");

	m_fLogFile.close();
	bLogOpen = false;
}

void LogInfo(const std::string& sInfo)
{
	if (!bLogOpen) return;

	m_fLogFile << "\n";
	m_fLogFile << "*** INFO *** Time: " << Time() << "\n";
	m_fLogFile << "\n";
	m_fLogFile << "\t" << sInfo << "\n";
	m_fLogFile << "\n";

	m_fLogFile.flush();
}

void LogError(const std::string& sError)
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

