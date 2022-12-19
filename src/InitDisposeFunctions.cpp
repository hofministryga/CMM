#include "Functions.h"

void deleteExtraData()
{
    for(int i=0; i<stuffToDelete.size(); i++)
    {
        if(stuffToDelete[i]!=nullptr)
            delete stuffToDelete[i];
    }
    stuffToDelete.clear();
}

void readConfig()
{
    IniFile ini = IniFile();
    bool valid = ini.load("Config.ini");
    if(valid)
    {
        std::string frameTimeStr = ini.readValue("Settings", "FrameTime");
        frameTime = StringTools::toInt(frameTimeStr);

        std::string writeLogEnabled = ini.readValue("Settings", "EnableLogs");
        if(StringTools::equalsIgnoreCase<char>(writeLogEnabled, "true"))
            writeLogs = true;
        else
            writeLogs = false;

        logFileName = ini.readValue("Settings", "LogDirectory");
        authorizationKey = ini.readValue("Settings", "AuthorizationKey");
    }
    else
    {
        //write default values
        ini.writeValue("Settings", "FrameTime", "16");
        ini.writeValue("Settings", "EnableLogs", "false");
        ini.writeValue("Settings", "LogDirectory", "Logs/");
        ini.writeValue("Settings", "AuthorizationKey", "");
        ini.save("Config.ini");

        SimpleDir d = SimpleDir("./Logs");
        if(!d.doesExist())
            d.createDirectory();
    }

    
    if(logFileName.back() != '/' && logFileName.back() != '\\')
        logFileName += '/';
    
    logFileName += getDateString(false, false) + ".log";
    writeStringToLog("VERSION: " + versionString, false);
}

void initFunction(SimpleWindow* w)
{
    w->setWindowAsInputFocus();
    readConfig();
    database = new SqliteDatabase("sqlStuff/CMM.db");
    if(database->getStatusCode() == SqliteDatabase::STATUS_CREATED)
        database->executeCommandFromFile("sqlStuff/CreateDatabase.sql"); //Newly created database file. Add the main table.

    w->getGuiManager()->loadElementsFromFile("GuiStuff/layout.xml");

    initHeader(w);
    initLeftMain(w);
    initRightMain(w);

    // loadFromDatabase(w);
}

void dispose()
{
    deleteExtraData();
    if(database != nullptr)
        delete database;
    database = nullptr;
}
