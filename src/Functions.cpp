#include "Functions.h"

std::string fpsString = "";
std::string logFileName = "";
SqliteDatabase* database = nullptr;
std::vector<GuiInstance*> stuffToDelete;
time_t lastUpdateTime = 0;
bool mode = INSERT;
int frameTime = 33;
bool writeLogs = false;

int page = 0;
int numOfItemsPerPage = 5;
bool canGoToNext = false;

bool sortAsc = true;
int searchMode = NONE;

time_t buildTime = 0;
std::string versionString = "v1.0";
std::string authorizationKey = "";