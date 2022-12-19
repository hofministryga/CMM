#include"SqliteDatabase.h"
#include<fstream>
#include"UTFConversion.h"

struct DatabaseStructThing
{
    bool shouldIncludeHeader = false;
    std::vector<std::vector<std::string>>* data;
};

int SqliteDatabase::callback(void* data, int argc, char** argv, char** azColName)
{
    //assume data is a pointer to a std::vector<std::vector<std::string>>.
    //assume utf8
    DatabaseStructThing* k = (DatabaseStructThing*)data;
    std::vector<std::vector<std::string>>* table = k->data;

    if(table->size() == 0 && k->shouldIncludeHeader)
    {
        table->push_back( std::vector<std::string>() );
        for(int i=0; i<argc; i++)
        {
            table->back().push_back( azColName[i] );
        }
    }

    table->push_back( std::vector<std::string>() );
    for(int i=0; i<argc; i++)
    {
        if(argv[i] != nullptr)
            table->back().push_back( argv[i] );
        else
            table->back().push_back( "NULL" );
    }

    return 0;
}
    
SqliteDatabase::SqliteDatabase()
{
    isValid = false;
    statusCode = STATUS_INVALID;
}

SqliteDatabase::SqliteDatabase(std::string filename)
{
    isValid = openDatabase(filename);
}

SqliteDatabase::SqliteDatabase(std::wstring filename)
{
    isValid = openDatabase(toUTF8(filename));
}

SqliteDatabase::~SqliteDatabase()
{
    close();
}

void SqliteDatabase::close()
{
    isValid = false;
    statusCode = STATUS_INVALID;
    if(databaseConnection!=nullptr)
        sqlite3_close(databaseConnection);
    databaseConnection = nullptr;
}

bool SqliteDatabase::valid()
{
    return isValid;
}

int SqliteDatabase::getStatusCode()
{
    return statusCode;
}

bool SqliteDatabase::openDatabase(std::string filename)
{
    bool hadToCreate = true;
    std::fstream temp = std::fstream(filename, std::fstream::in | std::fstream::binary);
    if(temp.is_open())
        hadToCreate = false;
    temp.close();

    int error = sqlite3_open(filename.c_str(), &databaseConnection);
    if(error == SQLITE_OK)
    {
        if(hadToCreate)
            statusCode = STATUS_CREATED;
        else
            statusCode = STATUS_OK;
    }
    else
        statusCode = STATUS_INVALID;
    return (error >= SQLITE_OK);
}

std::vector<std::vector<std::string>> SqliteDatabase::executeCommandFromFile(std::string filename, bool includeHeader)
{
    if(!isValid)
        return {};
    std::fstream file = std::fstream(filename, std::fstream::binary | std::fstream::in);
    if(file.is_open())
    {
        std::string command = "";
        while (true)
        {
            int byte = file.get();
            if(!file.eof())
                command += byte;
            else
                break;
        }
        
        file.close();
        return executeCommand(command, includeHeader);
    }

    statusCode = STATUS_FILE_ERROR;
    return {};
}

std::vector<std::vector<std::string>> SqliteDatabase::executeCommandFromFile(std::wstring filename, bool includeHeader)
{
    if(!isValid)
        return {};
    std::fstream file = std::fstream(filename, std::fstream::binary | std::fstream::in);
    if(file.is_open())
    {
        std::string command = "";
        while (true)
        {
            int byte = file.get();
            if(!file.eof())
                command += byte;
            else
                break;
        }
        
        file.close();
        return executeCommand(command, includeHeader);
    }

    statusCode = STATUS_FILE_ERROR;
    return {};
}

std::vector<std::vector<std::string>> SqliteDatabase::executeCommand(std::string command, bool includeHeader)
{
    return executeCommand(toWideString(command), includeHeader);
}

std::vector<std::vector<std::string>> SqliteDatabase::executeCommand(std::wstring command, bool includeHeader)
{
    if(!isValid)
        return {};
    
    std::vector<std::vector<std::string>> results;
    std::string utfString = toUTF8(command);

    DatabaseStructThing k;
    k.shouldIncludeHeader = includeHeader;
    k.data = &results;

    int status = sqlite3_exec(databaseConnection, utfString.c_str(), SqliteDatabase::callback, &k, nullptr);
    if(status == SQLITE_OK)
        statusCode = STATUS_OK;
    else
        statusCode = STATUS_SQL_ERROR;
    
    return results;
}