#include <iostream>
#include <stdio.h>
#include <ctime>
#include <glib/SimpleJSON.h>
#include <glib/StringTools.h>
#include <glib/SimpleDir.h>

void checkForUpdate(time_t buildTime, std::string owner, std::string repo);
void uploadToServer(std::string owner, std::string repo, std::string branch, std::string filename, std::string serverFileName, std::string authorizationKey);