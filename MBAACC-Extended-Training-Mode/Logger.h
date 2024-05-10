#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <filesystem>
#include <chrono>
#include <format>
#include <fstream>

#include "Constants.h"

static HANDLE m_hConsole;
static std::string m_sLogFile;
static std::ofstream m_fLogFile;

static void LogInfo(std::string sInfo);

static std::string Time()
{
	return std::format("{:%m-%d-%y_%OH-%OM-%OS}", std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() });
}

static void InitializeLogger(HANDLE hConsole)
{
	m_hConsole = hConsole;

	std::string sTempPath = std::filesystem::temp_directory_path().string();
	std::string sTime = Time();

	m_sLogFile = sTempPath + "ExtendedTrainingMode__" + sTime + ".log";

	m_fLogFile.open(m_sLogFile);

	LogInfo(VERSION);
}

static void CloseLogger()
{
	LogInfo("Closed Extended Training Mode");

	m_fLogFile.close();
}

static void LogInfo(std::string sInfo)
{
	m_fLogFile << "\n";
	m_fLogFile << "*** INFO *** Time: " << Time() << "\n";
	m_fLogFile << "\n";
	m_fLogFile << "\t" << sInfo << "\n";
	m_fLogFile << "\n";

	m_fLogFile.flush();
}

static void LogError(std::string sError)
{
	m_fLogFile << "\n";
	m_fLogFile << "*** ERROR ***";
	m_fLogFile << "\tTime: " << Time() << "\n";
	m_fLogFile << "\n";
	m_fLogFile << "\t" << sError << "\n";
	m_fLogFile << "\n";

	m_fLogFile.flush();
}

static void CloseLog(bool bPaused, bool bAPressed, bool bOnCSS, bool bOnExtendedSettingsMenu, int nP1CharacterID, int nP2CharacterID, int nP1Moon, int nP2Moon, int nP1CharacterNumber, int nP2CharacterNumber, bool bSwitchToCrouch, int nExGuardSetting, int nCustomGuard, int nReversalPattern, int nReversalDelayFrames, int nTempReversalDelayFrames, bool bDelayingReversal, bool bReversaled, int nReversalType, int nReversalIndex1, int nReversalIndex2, int nReversalIndex3, int nReversalIndex4, std::vector<std::string> vPatternNames, int nMot, int nOldMot, int nP2Y, int nBurstCooldown, int nOldEnemyActionIndex, int nOldPresetIndex, int nOldEnemyDefenseIndex, int nOldEnemyDefenseTypeIndex, int nOldAirRecoveryIndex, int nOldDownRecoveryIndex, int nOldThrowRecoveryIndex, int nOldReduceDamageIndex, int nOldLifeIndex, int nCurrentSubMenu, int nOldCurrentSubMenu, int nFrameCounter, int nOldFrameCounter, int nStoredEnemyAction, int nStoredEnemyDefense, int nStoredEnemyDefenseType, int nStoredAirRecovery, int nStoredDownRecovery, int nStoredThrowRecovery, int nStoredReduceDamage, int nGameCursorIndex, int nOldGameCursorIndex, int nEnemySettingsCursor, int nOldEnemySettingsCursor, int nCustomMeter, int nCustomHealth, int nHealthRefillTimer, int nCharSpecificsRefillTimer, bool bLifeRecover, int nSionBullets, int nRoaVisibleCharge, int nRoaHiddenCharge, int nSionBulletsRefillTimer, int nRoaHiddenChargeRefillTimer, int nRoaVisibleChargeRefillTimer, int nExtendedSettingsPage, bool bPositionsLocked, int nP1X, int nP2X, int nP3X, int nP4X, bool bP3Exists, bool bP4Exists, int nHitsTillBurst, bool bInfGuard, int nGameMode)
{
	m_fLogFile << "\n";
	m_fLogFile << "*** INFO ***";
	m_fLogFile << "MBAA CLOSED " <<"\n";
	m_fLogFile << "\tTime: " << Time() << "\n";
	m_fLogFile << "\n";
	m_fLogFile << "\tbPaused: " << bPaused << "\n";
	m_fLogFile << "\tbAPressed: " << bAPressed << "\n";
	m_fLogFile << "\tbOnCSS: " << bOnCSS << "\n";
	m_fLogFile << "\tbOnExtendedSettingsMenu: " << bOnExtendedSettingsMenu << "\n";
	m_fLogFile << "\tnP1CharacterID: " << nP1CharacterID << "\n";
	m_fLogFile << "\tnP2CharacterID: " << nP2CharacterID << "\n";
	m_fLogFile << "\tnP1Moon: " << nP1Moon << "\n";
	m_fLogFile << "\tnP2Moon: " << nP2Moon << "\n";
	m_fLogFile << "\tnP1CharacterNumber: " << nP1CharacterNumber << "\n";
	m_fLogFile << "\tnP2CharacterNumber: " << nP2CharacterNumber << "\n";
	m_fLogFile << "\tbSwitchToCrouch: " << bSwitchToCrouch << "\n";
	m_fLogFile << "\tnExGuardSetting: " << nExGuardSetting << "\n";
	m_fLogFile << "\tnCustomGuard: " << nCustomGuard << "\n";
	m_fLogFile << "\tnReversalPattern: " << nReversalPattern << "\n";
	m_fLogFile << "\tnReversalDelayFrames: " << nReversalDelayFrames << "\n";
	m_fLogFile << "\tnTempReversalDelayFrames: " << nTempReversalDelayFrames << "\n";
	m_fLogFile << "\tbDelayingReversal: " << bDelayingReversal << "\n";
	m_fLogFile << "\tbReversaled: " << bReversaled << "\n";
	m_fLogFile << "\tnReversalType: " << nReversalType << "\n";
	m_fLogFile << "\tnReversalIndex1: " << nReversalIndex1 << "\n";
	m_fLogFile << "\tnReversalIndex2: " << nReversalIndex2 << "\n";
	m_fLogFile << "\tnReversalIndex3: " << nReversalIndex3 << "\n";
	m_fLogFile << "\tnReversalIndex4: " << nReversalIndex4 << "\n";
	m_fLogFile << "\tnMot: " << nMot << "\n";
	m_fLogFile << "\tnOldMot: " << nOldMot << "\n";
	m_fLogFile << "\tnP2Y: " << nP2Y << "\n";
	m_fLogFile << "\tnBurstCooldown: " << nBurstCooldown << "\n";
	m_fLogFile << "\tnOldEnemyActionIndex: " << nOldEnemyActionIndex << "\n";
	m_fLogFile << "\tnOldPresetIndex: " << nOldPresetIndex << "\n";
	m_fLogFile << "\tnOldEnemyDefenseIndex: " << nOldEnemyDefenseIndex << "\n";
	m_fLogFile << "\tnOldEnemyDefenseTypeIndex: " << nOldEnemyDefenseTypeIndex << "\n";
	m_fLogFile << "\tnOldAirRecoveryIndex: " << nOldAirRecoveryIndex << "\n";
	m_fLogFile << "\tnOldDownRecoveryIndex: " << nOldDownRecoveryIndex << "\n";
	m_fLogFile << "\tnOldThrowRecoveryIndex: " << nOldThrowRecoveryIndex << "\n";
	m_fLogFile << "\tnOldReduceDamageIndex: " << nOldReduceDamageIndex << "\n";
	m_fLogFile << "\tnOldLifeIndex: " << nOldLifeIndex << "\n";
	m_fLogFile << "\tnCurrentSubMenu: " << nCurrentSubMenu << "\n";
	m_fLogFile << "\tnOldCurrentSubMenu: " << nOldCurrentSubMenu << "\n";
	m_fLogFile << "\tnFrameCounter: " << nFrameCounter << "\n";
	m_fLogFile << "\tnOldFrameCounter: " << nOldFrameCounter << "\n";
	m_fLogFile << "\tnStoredEnemyAction: " << nStoredEnemyAction << "\n";
	m_fLogFile << "\tnStoredEnemyDefense: " << nStoredEnemyDefense << "\n";
	m_fLogFile << "\tnStoredEnemyDefenseType: " << nStoredEnemyDefenseType << "\n";
	m_fLogFile << "\tnStoredAirRecovery: " << nStoredAirRecovery << "\n";
	m_fLogFile << "\tnStoredDownRecovery: " << nStoredDownRecovery << "\n";
	m_fLogFile << "\tnStoredThrowRecovery: " << nStoredThrowRecovery << "\n";
	m_fLogFile << "\tnStoredReduceDamage: " << nStoredReduceDamage << "\n";
	m_fLogFile << "\tnGameCursorIndex: " << nGameCursorIndex << "\n";
	m_fLogFile << "\tnOldGameCursorIndex: " << nOldGameCursorIndex << "\n";
	m_fLogFile << "\tnEnemySettingsCursor: " << nEnemySettingsCursor << "\n";
	m_fLogFile << "\tnOldEnemySettingsCursor: " << nOldEnemySettingsCursor << "\n";
	m_fLogFile << "\tnCustomMeter: " << nCustomMeter << "\n";
	m_fLogFile << "\tnCustomHealth: " << nCustomHealth << "\n";
	m_fLogFile << "\tnHealthRefillTimer: " << nHealthRefillTimer << "\n";
	m_fLogFile << "\tnCharSpecificsRefillTimer: " << nCharSpecificsRefillTimer << "\n";
	m_fLogFile << "\tbLifeRecover: " << bLifeRecover << "\n";
	m_fLogFile << "\tnSionBullets: " << nSionBullets << "\n";
	m_fLogFile << "\tnRoaVisibleCharge: " << nRoaVisibleCharge << "\n";
	m_fLogFile << "\tnRoaHiddenCharge: " << nRoaHiddenCharge << "\n";
	m_fLogFile << "\tnSionBulletsRefillTimer: " << nSionBulletsRefillTimer << "\n";
	m_fLogFile << "\tnRoaHiddenChargeRefillTimer: " << nRoaHiddenChargeRefillTimer << "\n";
	m_fLogFile << "\tnRoaVisibleChargeRefillTimer: " << nRoaVisibleChargeRefillTimer << "\n";
	m_fLogFile << "\tnExtendedSettingsPage: " << nExtendedSettingsPage << "\n";
	m_fLogFile << "\tbPositionsLocked: " << bPositionsLocked << "\n";
	m_fLogFile << "\tnP1X: " << nP1X << "\n";
	m_fLogFile << "\tnP2X: " << nP2X << "\n";
	m_fLogFile << "\tnP3X: " << nP3X << "\n";
	m_fLogFile << "\tnP4X: " << nP4X << "\n";
	m_fLogFile << "\tbP3Exists: " << bP3Exists << "\n";
	m_fLogFile << "\tbP4Exists: " << bP4Exists << "\n";
	m_fLogFile << "\tnHitsTillBurst: " << nHitsTillBurst << "\n";
	m_fLogFile << "\tbInfGuard: " << bInfGuard << "\n";
	m_fLogFile << "\tnGameMode: " << nGameMode << "\n";
	m_fLogFile << "\n";

	m_fLogFile.flush();
}