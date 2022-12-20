#include "Functions.h"

void setBuildTime()
{
    tm actualBuildTime;
    memset(&actualBuildTime, 0, sizeof(tm));
    
    actualBuildTime.tm_year = YEAR-1900;
    actualBuildTime.tm_mon = MON-1;
    actualBuildTime.tm_mday = DAY;

    buildTime = mktime(&actualBuildTime);
}

void checkUpdate()
{
	checkForUpdate(buildTime, "hofministryga", "CMM");
    // checkForUpdate(buildTime, "https://api.github.com/repos/hofministryga/CMM/releases/latest");
}

void uploadDatabase()
{
	uploadToServer("hofministryga", "hofministryga.github.io", "db", "./sqlStuff/CMM.db", "CMM.db", authorizationKey);
}