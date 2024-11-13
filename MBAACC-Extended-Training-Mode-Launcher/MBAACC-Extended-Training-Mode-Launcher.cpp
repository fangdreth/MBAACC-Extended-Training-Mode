
#include <stdio.h>
#include <windows.h>
#include <cstdarg>
#include <string>
#include <regex>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <wininet.h>
#include <fstream>
#include <time.h>
#include "resource.h"
#pragma comment(lib, "wininet.lib")

std::wstring getLogPath() {
	wchar_t buffer[1024];
	if (!GetTempPathW(1024, buffer)) {
		return L"";
	}
	std::wstring res = std::wstring(buffer) + std::wstring(L"\\Extended-Training-Mode-Launcher.log");

	return res;
}

void __stdcall log(const char* format, ...) {

	char buffer[4096];
	va_list args;
	va_start(args, format);
	int n = vsnprintf(buffer, 4095, format, args);
	va_end(args);
	
	if (n < 0) {
		return;
	}

	buffer[n] = '\n';
	buffer[n+1] = '\0';

	OutputDebugStringA(buffer);

	std::ofstream logFile(getLogPath(), std::ios::app);
	logFile << buffer;
	logFile.close();

}

void initLog() {

	std::wstring logFilePath = getLogPath();
	DeleteFileW(logFilePath.c_str());
	std::ofstream outFile(logFilePath);
	outFile << "log inited\n";
	outFile.close();

}

time_t toTimestamp(const std::string& dateTimeStr) {
	std::tm tm = {};
	std::istringstream ss(dateTimeStr);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%SZ");


	time_t timestamp = mktime(&tm);

	return timestamp;
}

extern const char* compileTimestring;

std::wstring getExePath() {
	wchar_t buffer[1024];
	if (!GetTempPathW(1024, buffer)) {
		log("Failed to get temp path\n");
		return L"";
	}
	std::wstring dllPath = std::wstring(buffer) + std::wstring(L"\\Extended-Training-Mode.exe");
	return dllPath;
}

bool writeExe() {

	std::wstring dllPath = getExePath();

	HINSTANCE hInstance = GetModuleHandle(NULL);

	HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(IDR_EXE1), L"EXE");
	if (!hRes) {
		log("Failed to FindResource %d\n", GetLastError());
		return false;
	}

	HGLOBAL hData = LoadResource(hInstance, hRes);
	if (!hData) {
		printf("Failed to LoadResource\n");
		return false;
	}
	void* pData = LockResource(hData);
	if (!pData) {
		printf("Failed to LockResource\n");
		return false;
	}

	DWORD dwSize = SizeofResource(hInstance, hRes);

	std::ofstream outFile(dllPath, std::ios::binary);
	if (outFile) {
		outFile.write(reinterpret_cast<char*>(pData), dwSize);
	} else {
		printf("Failed to open dll file\n");
		return false;
	}

	return true;
}

// -----

bool old = false;

std::wstring getFilePath() {
	wchar_t pathBuffer[1024];
	GetModuleFileNameW(NULL, pathBuffer, 1024);
	std::wstring path = std::wstring(pathBuffer);
	return path;
}

int needUpdate() {

	// https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/tags/bleeding-edge

	HINTERNET hInternet = InternetOpen(L"GitHubAPI", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		log("InternetOpen failed: % d", GetLastError());
		return -1;
	}

	HINTERNET hConnect = InternetOpenUrlW(hInternet, L"https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/tags/bleeding-edge", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hConnect == NULL) {
		log("InternetOpenUrlW failed: %d", GetLastError());
		InternetCloseHandle(hInternet);
		return -1;
	}

	char buffer[8192];
	DWORD bytesRead;
	std::string response;

	while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
		response.append(buffer, bytesRead);
	}

	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	std::regex pattern(R"("published_at"\s*:\s*"[^"]*")");

	std::smatch match;
	if (!std::regex_search(response, match, pattern)) {
		log("couldnt find published_at in json: %s", response.c_str());
	}

	std::string temp = match.str();

	pattern = std::regex(R"(\"published_at\":\"(.*?)\")");

	if (!std::regex_search(temp, match, pattern)) {
		log("couldnt find datetime in json: %s", response.c_str());
	}

	std::string githubTimestamp = match[1].str();

	githubTimestamp[10] = ' ';

	time_t compileTime = toTimestamp(compileTimestring);

	time_t releaseTime = toTimestamp(githubTimestamp);

	log("release time: %lld", releaseTime);
	log("compile time: %lld", compileTime);

	//return true;
	return releaseTime > compileTime;
}

bool update() {

	std::wstring path = getFilePath();
	std::wstring newPath = path + L".old";

	if (!MoveFileW(path.c_str(), newPath.c_str())) {
		log("failed to rename file\n");
		return false;
	}

	// download the file 
	// https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases/download/bleeding-edge/MBAACC-Extended-Training-Mode.exe

	HINTERNET hInternet = InternetOpen(L"Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		log("InternetOpen failed: % d", GetLastError());
		return false;
	}

	HINTERNET hConnect = InternetOpenUrlW(hInternet, L"https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases/download/bleeding-edge/MBAACC-Extended-Training-Mode-Launcher.exe", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hConnect == NULL) {
		log("InternetOpenUrlW failed: %d", GetLastError());
		InternetCloseHandle(hInternet);
		return false;
	}

	std::ofstream outFile(path, std::ios::binary);
	if (!outFile.is_open()) {
		log("failed to open updated file for writing");
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return false;
	}

	size_t bufferSize = 4096;
	std::vector<char> buffer(bufferSize);

	DWORD bytesRead;
	while (InternetReadFile(hConnect, buffer.data(), bufferSize, &bytesRead) && bytesRead > 0) {
		if (bytesRead == bufferSize) {
			bufferSize *= 2;
			buffer.resize(bufferSize);
		}
		outFile.write(buffer.data(), bytesRead);
	}

	outFile.close();
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	return true;
}

void launchTrainingMode() {

	writeExe();

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;

	// launch the updated launcher.
	CreateProcessW(
		getExePath().c_str(),
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_PROCESS_GROUP,
		NULL,
		NULL,
		&si,
		&pi
	);

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	log("\nPROGRAM STARTED");

	initLog();

	std::wstring path = getFilePath();
	if (path.substr(path.size() - 4) == L".old") {
		old = true;
	}

	if (!old) {
		std::wstring oldPath = path + L".old";
		DeleteFileW(oldPath.c_str());
	}
	
	int updateStatus = needUpdate();

	if (updateStatus == -1) {
		log("needUpdate failed with %d", updateStatus);
	} else if (updateStatus == 0) {
		log("no update needed");
	} else if (updateStatus == 1) {
		log("attempting an update");
		if (!update()) {
			log("update failed");
			return 1;
		}

		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi;
		
		// launch the updated launcher.
		CreateProcessW(
			path.c_str(),
			NULL,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_PROCESS_GROUP,
			NULL,
			NULL,
			&si,
			&pi
		);

		log("update successful, exiting gracefully\n");
		return 0;
	}

	launchTrainingMode();
	log("exiting gracefully\n");
	return 0;
}
