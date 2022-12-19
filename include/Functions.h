#pragma once

#define _WIN32_WINNT 0x0500

#include <glib/StringTools.h>
#include <glib/SimpleWindow.h>
#include <glib/SimpleDir.h>
#include <glib/System.h>
#include <glib/IniFile.h>
#include <glib/Input.h>
#include <glib/SimpleJSON.h>
#include <SqliteDatabase.h>
#include "Updater.h"

#pragma region VARIABLES

//change these for build time stuff
#ifndef YEAR
	#define YEAR 2022
#endif

#ifndef MON
	#define MON 1
#endif

#ifndef DAY
	#define DAY 1
#endif

#define EDIT true
#define INSERT false

#define NONE 0
#define TITLE 1
#define DATE 2

using namespace glib;

extern std::string fpsString;
extern std::string logFileName;
extern SqliteDatabase* database;
extern std::vector<GuiInstance*> stuffToDelete;
extern time_t lastUpdateTime;
extern bool mode;
extern int frameTime;
extern bool writeLogs;

extern int page;
extern int numOfItemsPerPage;
extern bool canGoToNext;

extern bool sortAsc;
extern int searchMode;

extern time_t buildTime;
extern std::string versionString;
extern std::string authorizationKey;

#pragma endregion

#pragma region UPDATE_FUNCTIONS

void setBuildTime();
void checkUpdate();
void uploadDatabase();

#pragma endregion

#pragma region LOG_FUNCTIONS

std::string getDateString(bool useColons = true, bool getTime = true);
void writeStringToLog(std::string s, bool includeDate = false);
void removeOldLogs();

#pragma endregion

#pragma region INIT_DISPOSE_FUNCTIONS

void initFunction(SimpleWindow* w);
void readConfig();
void dispose();
void deleteExtraData();

	#pragma region SPECIAL_INIT_FUNCTIONS

	void initHeader(SimpleWindow* w);
	void initLeftMain(SimpleWindow* w);
	void initRightMain(SimpleWindow* w);

	#pragma endregion

#pragma endregion

#pragma region DB_FUNCTIONS

void setEditMode(SimpleWindow* w, bool v);
void insertData(SimpleWindow* w);
void editData(SimpleWindow* w);
void fillWithID(SimpleWindow* w, std::string id);
std::string getSelectCommand(SimpleWindow* w);
void loadFromDatabase(SimpleWindow* w);
void checkDatabaseFile(SimpleWindow* w);

int countResults(SimpleWindow* w);
void correctGuiFunction(SimpleWindow* w);

#pragma endregion
