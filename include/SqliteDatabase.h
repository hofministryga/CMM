#pragma once

#include<iostream>
#include<sqlite3.h>
#include<vector>

class SqliteDatabase
{
public:
    static const int STATUS_OK = 0;
    static const int STATUS_FILE_ERROR = -1;
    static const int STATUS_SQL_ERROR = -2;
    static const int STATUS_INVALID = -3;
    static const int STATUS_NOT_FOUND = -4;
    static const int STATUS_CREATED = 1;
    
    SqliteDatabase();
    SqliteDatabase(std::string filename);
    SqliteDatabase(std::wstring filename);
    ~SqliteDatabase();

    void close();
    bool valid();
    int getStatusCode();

    std::vector<std::vector<std::string>> executeCommandFromFile(std::string file, bool includeHeader = false);
    std::vector<std::vector<std::string>> executeCommandFromFile(std::wstring file, bool includeHeader = false);
    std::vector<std::vector<std::string>> executeCommand(std::string command, bool includeHeader = false);
    std::vector<std::vector<std::string>> executeCommand(std::wstring command, bool includeHeader = false);

private:
    bool openDatabase(std::string filename);
    void closeDatabase();

    static int callback(void* data, int argc, char** argv, char** azColName);

    sqlite3* databaseConnection = nullptr;
    bool isValid = false;
    int statusCode = 0;
};