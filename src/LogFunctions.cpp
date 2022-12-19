#include "Functions.h"

std::string getDateString(bool useColons, bool getTime)
{
    std::string timeStr;
    time_t timeV = time(NULL);
    tm* tmPoint = localtime(&timeV);
	if(getTime)
	{
		if(useColons)
			timeStr = StringTools::formatString("%02d/%02d/%04d %02d:%02d:%02d", tmPoint->tm_mday, tmPoint->tm_mon+1, tmPoint->tm_year+1900, tmPoint->tm_hour, tmPoint->tm_min, tmPoint->tm_sec);
		else
			timeStr = StringTools::formatString("%02d_%02d_%04d %02d-%02d-%02d", tmPoint->tm_mday, tmPoint->tm_mon+1, tmPoint->tm_year+1900, tmPoint->tm_hour, tmPoint->tm_min, tmPoint->tm_sec);
	}
	else
	{
		if(useColons)
			timeStr = StringTools::formatString("%02d/%02d/%04d", tmPoint->tm_mday, tmPoint->tm_mon+1, tmPoint->tm_year+1900);
		else
			timeStr = StringTools::formatString("%02d_%02d_%04d", tmPoint->tm_mday, tmPoint->tm_mon+1, tmPoint->tm_year+1900);
	}

    return timeStr;
}

void writeStringToLog(std::string s, bool includeDate)
{
    if(writeLogs)
    {
        std::string finalString;

        if(includeDate)
        {
            finalString = getDateString(true) + " - " + s;
        }
        else
        {
            finalString = s;
        }

        SimpleFile logFile = SimpleFile(logFileName, SimpleFile::WRITE_APPEND | SimpleFile::UTF8);
        if(logFile.isOpen())
        {
            logFile.writeString(finalString);
            logFile.writeLineBreak();
            logFile.close();
        }
    }
}

void removeOldLogs()
{
    //remove old logs. Keep a maximum of 30
    SimpleDir d = SimpleDir("./Logs");
    std::vector<std::string> filenames = d.getFiles();
    //sort by date
    Sort::insertionSort<std::string>(filenames.data(), filenames.size(), [&d](std::string a, std::string b) -> bool{
        int id1 = d.getReferenceLocation(a);
        int id2 = d.getReferenceLocation(b);
        if(id1>=0 && id2>=0)
        {
            return d.getLastChangeTime(id1) < d.getLastChangeTime(id2);
        }
        return false;
    });

    for(int i=30; i<filenames.size(); i++)
    {
        int id = d.getReferenceLocation(filenames[i]);
        if(id >= 0)
            d.deleteResource(id);
    }
}
