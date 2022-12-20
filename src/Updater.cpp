#include "Updater.h"

glib::SimpleJSON j;
glib::SimpleDir d = glib::SimpleDir("./");
bool curlAvaliable = false;

time_t convertGitDateTime(std::string input)
{
    //Format : YYYY-MM-DDTHH:MM:SSZ
    tm timeVal = {};
    timeVal.tm_year = stoi(input.substr(0, 4)) - 1900;
    timeVal.tm_mon = stoi(input.substr(5, 2)) - 1;
    timeVal.tm_mday = stoi(input.substr(8, 2));
    timeVal.tm_hour = stoi(input.substr(11, 2));
    timeVal.tm_min = stoi(input.substr(14, 2));
    timeVal.tm_sec = stoi(input.substr(17, 2));
    timeVal.tm_isdst = -1;
    
    return mktime(&timeVal);
}

bool checkCurl()
{
    int errVal = system("curl --version");
    return (errVal == 0);
}

void init()
{
    if(!curlAvaliable)
        curlAvaliable = checkCurl();
}

int downloadWebpageInfo(std::string owner, std::string repo)
{
    std::string repoName = owner + "/" + repo;
    std::string command = "start /min /wait /b curl -H \"Accept: application/vnd.github+json\" https://api.github.com/repos/" + repoName + "/releases/latest";
    return system((command + " >> temp.json").c_str());
}

int downloadFileInfo(std::string owner, std::string repo, std::string branch, std::string filepath)
{
    std::string repoName = owner + "/" + repo;
    std::string command = "start /min /wait /b curl -H \"Accept: application/vnd.github+json\" https://api.github.com/repos/" + repoName + "/contents/" + filepath;
    if(!branch.empty())
        command += "?ref=" + branch;
    return system((command + " >> temp.json").c_str());
}

bool checkWebpageForUpdate(time_t buildTime)
{
    //tag_name should be in the first node
    for(glib::JNode* node : j.getRootNode()->getNodes())
    {
        if(node->getType() == 1)
        {
            glib::JPair* pair = (glib::JPair*)node;
            if(pair->getName() == "published_at")
            {
                std::string timeString = pair->getValue();
                time_t timeVal = convertGitDateTime(timeString);
                if(timeVal > buildTime)
                {
                    return true;
                }
                else
                {
                    return false;
                }

            }
        }
    }
    return false;
}

void update(std::string nameOfInstaller)
{
    std::string downloadURL = "";
    bool validOption = false;
    //looking for download_url in assets
    for(glib::JNode* node : j.getRootNode()->getNodes())
    {
        if(node->getName() == "assets")
        {
            glib::JArray* arr = (glib::JArray*)node;
            for(glib::JNode* assetObjs : arr->getNodes())
            {
                if(assetObjs->getType() == glib::SimpleJSON::TYPE_OBJECT)
                {
                    for(glib::JNode* assetObjNode : ((glib::JObject*)assetObjs)->getNodes())
                    {
                        if(assetObjNode->getType() == glib::SimpleJSON::TYPE_PAIR)
                        {
                            if(assetObjNode->getName() == "name")
                            {
                                if(((glib::JPair*)assetObjNode)->getValue() == nameOfInstaller)
                                {
                                    validOption = true;
                                }
                            }
                            else if(assetObjNode->getName() == "browser_download_url")
                            {
                                downloadURL = ((glib::JPair*)assetObjNode)->getValue();
                                break;
                            }
                        }
                    }

                    if(!downloadURL.empty())
                    {
                        break;
                    }
                }
            }

            if(!downloadURL.empty())
            {
                break;
            }
        }
    }

    if(downloadURL != "")
    {
        //Use start to launch minimized.
        std::string command = "start /min /wait /b curl -OL " + downloadURL;
        system(command.c_str());

        //assumes it was an installer
        //Use start to launch and return immediately.
        command = "start " + nameOfInstaller;
        system(command.c_str());
        
        //Close since the installer is running.
        exit(0);
    }
}

std::string parseForSHA()
{
    //looking for sha
    for(glib::JNode* node : j.getRootNode()->getNodes())
    {
        if(node->getName() == "sha")
        {
            return ((glib::JPair*)node)->getValue();
        }
    }

    return "";
}

void uploadFile(std::string owner, std::string repo, std::string branch, std::string filename, std::string serverFileName, std::string sha, std::string token)
{
    std::string fileAsBase64;
    std::vector<unsigned char> fileBytes;
    std::string quoteMark = "\"";
    std::string escapedQuote = "\\\"";

    std::fstream f = std::fstream(filename, std::fstream::in | std::fstream::binary);
    if(f.is_open())
    {
        while(true)
        {
            unsigned char c = f.get();
            if(f.eof())
                break;
            fileBytes.push_back(c);
        }
        f.close();
    }
    
    fileAsBase64 = glib::StringTools::base64Encode(fileBytes, false);

    std::string dataString = "";
    dataString += "{\"message\":\"commitFromCMM\", ";
    if(!branch.empty())
        dataString += "\"branch\":\"" + branch + "\", ";
    
    dataString += "\"content\":\"" + fileAsBase64 + "\"";

    if(sha.empty())
    {
        dataString += "}";
    }
    else
    {
        dataString += ", \"sha\":\"" + sha + "\"}";
    }

    //Save to file uploadTmp due to size limits of the command line. Note that we can't just upload the file since it has to be base64.
    f = std::fstream("uploadTmp", std::fstream::out | std::fstream::binary);
    f.write(dataString.c_str(), dataString.size());
    f.close();

    //adjust so that all data is in a file that curl will read. So everything after -d should go into a separate string which gets written to a file instead.
    
    std::string command = "start /min /wait /b ";
    command = "curl -X PUT -H \"Accept: application/vnd.github+json\" -H \"Authorization: token " + token + "\" -d @uploadTmp ";
    command += "https://api.github.com/repos/"+owner+"/"+repo+"/contents/"+serverFileName;
    if(!branch.empty())
    {
        command += "?ref="+branch;
    }

    system((command + " -v >> uploadLog.json").c_str());

    //delete temporary file.
    d.update();
    d.deleteResource( d.getReferenceLocation("uploadTmp") );
}

void checkJSONStuff(time_t buildTime, std::string owner, std::string repo)
{
    //check for curl if not already done.
    init();

    
    d.update();
    d.deleteResource( d.getReferenceLocation("temp.json") );
    d.deleteResource( d.getReferenceLocation("Installer.exe") );
    if(curlAvaliable)
    {
        int error = -1;
        bool shouldUpdate = false;

        error = downloadWebpageInfo(owner, repo);
        
        if(error == 0)
        {
            j.load("temp.json");
            shouldUpdate = checkWebpageForUpdate(buildTime);
        }
        
        
        d.update();
        d.deleteResource( d.getReferenceLocation("temp.json") );
        
        if(shouldUpdate)
            update("Installer.exe");
    }
}

void checkForUpdate(time_t buildTime, std::string owner, std::string repo)
{
    checkJSONStuff(buildTime, owner, repo);
}

void uploadToServer(std::string owner, std::string repo, std::string branch, std::string filename, std::string serverFileName, std::string authorizationKey)
{
    //check for curl if not already done.
    init();

    
    d.update();
    d.deleteResource( d.getReferenceLocation("temp.json") );
    d.deleteResource( d.getReferenceLocation("uploadLog.json") );
    if(curlAvaliable)
    {
        int error = -1;
        error = downloadFileInfo(owner, repo, branch, serverFileName);

        if(error == 0)
        {
            j.load("temp.json");
            std::string sha = parseForSHA();
            uploadFile(owner, repo, branch, filename, serverFileName, sha, authorizationKey);
        }

        
        d.update();
        d.deleteResource( d.getReferenceLocation("temp.json") );
        //can delete upload log if not needed.
        // d.deleteResource( d.getReferenceLocation("uploadLog.json") );
    }
}