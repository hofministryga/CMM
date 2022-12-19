#include "Functions.h"

void correctGuiFunction(SimpleWindow* w)
{
    //Because I'm lazy, this function exists to fix the inconsistencies with the GUI.
    int countForCurrentVars = countResults(w);
    int totalPossiblePages = (int)MathExt::ceil( (double)countForCurrentVars / numOfItemsPerPage );
    
    if(page >= totalPossiblePages)
    {
        //Problem. Move page back to a valid point or back to page 1.
        //Note that the page number shown on screen is increased by 1.
        page = totalPossiblePages-1;
        auto elements = w->getGuiManager()->getInstancesByName("PageNum");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText( StringTools::toString(page+1) );
        }
    }
}

int countResults(SimpleWindow* w)
{
    std::string finalResult = "SELECT COUNT(*) FROM MESSAGES";

    if(searchMode == NONE)
    {
        //do nothing else
    }
    else if(searchMode == TITLE)
    {
        std::string titleStr = "";
        auto elements = w->getGuiManager()->getInstancesByName("MessageTitleID");
        if(elements.size() > 0)
        {
            GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
            if(ins!=nullptr)
                titleStr = ins->getTextBlockElement()->getText();
        }
        
        if(titleStr!="")
            finalResult += " WHERE Title LIKE '%" + titleStr + "%'";
    }
    else if(searchMode == DATE)
    {
        std::string dateStr = "";
        auto elements = w->getGuiManager()->getInstancesByName("MessageDateID");
        if(elements.size() > 0)
        {
            GuiDatePicker* ins = (GuiDatePicker*)elements.front()->data;
            if(ins!=nullptr)
            {
                dateStr = ins->getDateString();

                //swap date format from mm/dd/yyyy to yyyy-mm-dd
                std::vector<std::string> splitDate = StringTools::splitString(dateStr, '/');
                if(splitDate.size() == 3)
                {
                    dateStr = splitDate[2] + "-" + splitDate[0] + "-" + splitDate[1];
                }
            }
        }

        if(dateStr != "")
        {
            finalResult += " WHERE DateRecorded >= '" + dateStr + "'";
        }
    }

    if(sortAsc)
    {
        finalResult += " ORDER BY DateRecorded ASC";
    }
    else
    {
        finalResult += " ORDER BY DateRecorded DESC";
    }
    finalResult += ";";

    //run command
    auto data = database->executeCommand(finalResult, false);
    if(data.size() > 0)
    {
        return StringTools::toInt(data[0][0]);
    }
    return 0;
}

void setEditMode(SimpleWindow* w, bool v)
{
    mode = v;
    if(mode == EDIT)
    {
        auto elements = w->getGuiManager()->getInstancesByName("ModeSwitchID");
        if(elements.size() > 0)
        {
            GuiRectangleButton* ins = (GuiRectangleButton*)elements.front()->data;
            ins->setVisible(true);
            ins->setActive(true);
        }

        elements = w->getGuiManager()->getInstancesByName("WindowBar_EditText");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText("Edit");
        }

        elements = w->getGuiManager()->getInstancesByName("WindowBar_Status1");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText("Editing Entry");
        }
    }
    else
    {
        auto elements = w->getGuiManager()->getInstancesByName("ModeSwitchID");
        if(elements.size() > 0)
        {
            GuiRectangleButton* ins = (GuiRectangleButton*)elements.front()->data;
            ins->setVisible(false);
            ins->setActive(false);
        }
        
        elements = w->getGuiManager()->getInstancesByName("WindowBar_EditText");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText("Insert");
        }

        elements = w->getGuiManager()->getInstancesByName("MessageID");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText("ID: N/A");
        }

        elements = w->getGuiManager()->getInstancesByName("WindowBar_Status1");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText("Inserting Entry");
        }
    }
}

void insertData(SimpleWindow* w)
{
    std::string comm = "INSERT INTO MESSAGES (Title, VideoLink, AudioLink, Description, DateRecorded) VALUES('%s', '%s', '%s', '%s', '%s');";
    std::string title, date, desc, link1, link2;

    auto elements = w->getGuiManager()->getInstancesByName("MessageTitleID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        title = ins->getTextBlockElement()->getText();
    }

    elements = w->getGuiManager()->getInstancesByName("MessageDateID");
    if(elements.size() > 0)
    {
        GuiDatePicker* ins = (GuiDatePicker*)elements.front()->data;
        date = ins->getDateString();
        //swap date format from mm/dd/yyyy to yyyy-mm-dd
        std::vector<std::string> splitDate = StringTools::splitString(date, '/');
        if(splitDate.size() == 3)
        {
            date = splitDate[2] + "-" + splitDate[0] + "-" + splitDate[1];
        }
    }

    elements = w->getGuiManager()->getInstancesByName("VideoMessageLinkID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        link1 = ins->getTextBlockElement()->getText();
    }

    elements = w->getGuiManager()->getInstancesByName("AudioMessageLinkID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        link2 = ins->getTextBlockElement()->getText();
    }

    elements = w->getGuiManager()->getInstancesByName("MessageDescriptionID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        desc = ins->getTextBlockElement()->getText();
    }

    //double single quote since single quote is an escape char
    std::string nTitle, nLink1, nLink2, nDesc, nDate;
    for(char& c : title)
    {
        nTitle += c;
        if(c == '\'')
            nTitle += '\'';
    }
    for(char& c : link1)
    {
        nLink1 += c;
        if(c == '\'')
            nLink1 += '\'';
    }
    for(char& c : link2)
    {
        nLink2 += c;
        if(c == '\'')
            nLink2 += '\'';
    }
    for(char& c : desc)
    {
        nDesc += c;
        if(c == '\'')
            nDesc += '\'';
    }
    for(char& c : date)
    {
        nDate += c;
        if(c == '\'')
            nDate += '\'';
    }

    if(nTitle.empty())
    {
        System::messageBoxPopup(0, "ERROR", "Title can not be empty");
        writeStringToLog("Error: Attempted Insert with no title", true);
        return;
    }

    if(nLink1.empty() && nLink2.empty())
    {
        System::messageBoxPopup(0, "ERROR", "Must have one link filled out");
        writeStringToLog("Error: Attempted Insert with no link", true);
        return;
    }
    
    if(nDate.empty())
    {
        System::messageBoxPopup(0, "ERROR", "Date can not be empty");
        writeStringToLog("Error: Attempted Insert with no date", true);
        return;
    }

    comm = StringTools::formatString(comm, nTitle.c_str(), nLink1.c_str(), nLink2.c_str(), nDesc.c_str(), nDate.c_str());
    database->executeCommand(comm);
    System::messageBoxPopup(0, "Status", "Insert Successful");
    writeStringToLog("INFO: Attempted Insert with title="+title, true);
}

void editData(SimpleWindow* w)
{
    std::string comm = "UPDATE MESSAGES SET Title = '%s', VideoLink = '%s', AudioLink = '%s', Description = '%s', DateRecorded = '%s' WHERE MessageID = %s;";
    std::string id, title, date, desc, link1, link2;

    auto elements = w->getGuiManager()->getInstancesByName("MessageTitleID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        title = ins->getTextBlockElement()->getText();
    }

    elements = w->getGuiManager()->getInstancesByName("MessageID");
    if(elements.size() > 0)
    {
        GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
        id = ins->getText().substr(4); //Removes 'ID: ' which is 4 characters in size
    }

    elements = w->getGuiManager()->getInstancesByName("MessageDateID");
    if(elements.size() > 0)
    {
        GuiDatePicker* ins = (GuiDatePicker*)elements.front()->data;
        date = ins->getDateString();
        //swap date format from mm/dd/yyyy to yyyy-mm-dd
        std::vector<std::string> splitDate = StringTools::splitString(date, '/');
        if(splitDate.size() == 3)
        {
            date = splitDate[2] + "-" + splitDate[0] + "-" + splitDate[1];
        }
    }

    elements = w->getGuiManager()->getInstancesByName("VideoMessageLinkID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        link1 = ins->getTextBlockElement()->getText();
    }

    elements = w->getGuiManager()->getInstancesByName("AudioMessageLinkID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        link2 = ins->getTextBlockElement()->getText();
    }

    elements = w->getGuiManager()->getInstancesByName("MessageDescriptionID");
    if(elements.size() > 0)
    {
        GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
        desc = ins->getTextBlockElement()->getText();
    }

    //double single quote since single quote is an escape char
    std::string nTitle, nLink1, nLink2, nDesc, nDate;
    for(char& c : title)
    {
        nTitle += c;
        if(c == '\'')
            nTitle += '\'';
    }
    for(char& c : link1)
    {
        nLink1 += c;
        if(c == '\'')
            nLink1 += '\'';
    }
    for(char& c : link2)
    {
        nLink2 += c;
        if(c == '\'')
            nLink2 += '\'';
    }
    for(char& c : desc)
    {
        nDesc += c;
        if(c == '\'')
            nDesc += '\'';
    }
    for(char& c : date)
    {
        nDate += c;
        if(c == '\'')
            nDate += '\'';
    }

    if(nTitle.empty())
    {
        System::messageBoxPopup(0, "ERROR", "Title can not be empty");
        writeStringToLog("Error: Attempted Insert with no title", true);
        return;
    }

    if(nLink1.empty() && nLink2.empty())
    {
        System::messageBoxPopup(0, "ERROR", "Must have one link filled out");
        writeStringToLog("Error: Attempted Insert with no link", true);
        return;
    }
    
    if(nDate.empty())
    {
        System::messageBoxPopup(0, "ERROR", "Date can not be empty");
        writeStringToLog("Error: Attempted Insert with no date", true);
        return;
    }

    comm = StringTools::formatString(comm, nTitle.c_str(), nLink1.c_str(), nLink2.c_str(), nDesc.c_str(), nDate.c_str(), id.c_str());
    database->executeCommand(comm);
    System::messageBoxPopup(0, "Status", "Edit Successful");
    writeStringToLog("INFO: Attempted Edit to ID="+id, true);
}

void fillWithID(SimpleWindow* w, std::string id)
{
    std::string command = "SELECT * FROM MESSAGES m WHERE m.MessageID = ";
    command += id + ";";

    auto data = database->executeCommand(command, false);
    if(data.size() > 0)
    {
        if(data[0].size() != 7)
        {
            writeStringToLog("ERROR SELECTING ID: Expected row to have 7 columns but got " + StringTools::toString(data[0].size()), true);
            writeStringToLog("\tID = " + id);
            return;
        }

        setEditMode(w, EDIT);
        auto elements = w->getGuiManager()->getInstancesByName("MessageID");
        if(elements.size() > 0)
        {
            GuiTextBlock* ins = (GuiTextBlock*)elements.front()->data;
            ins->setText("ID: " + id);
        }

        elements = w->getGuiManager()->getInstancesByName("MessageTitleID");
        if(elements.size() > 0)
        {
            GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
            ins->getTextBlockElement()->setText(data[0][1]);
        }

        elements = w->getGuiManager()->getInstancesByName("MessageDateID");
        if(elements.size() > 0)
        {
            GuiDatePicker* ins = (GuiDatePicker*)elements.front()->data;
            std::string date = (data[0][5]);
            
            //swap date format from mm/dd/yyyy to yyyy-mm-dd
            std::vector<std::string> splitDate = StringTools::splitString(date, '-');
            if(splitDate.size() == 3)
            {
                date = splitDate[1] + "/" + splitDate[2] + "/" + splitDate[0];
            }
            
            ins->setDateString(date);
        }

        elements = w->getGuiManager()->getInstancesByName("VideoMessageLinkID");
        if(elements.size() > 0)
        {
            GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
            ins->getTextBlockElement()->setText(data[0][2]);
        }

        elements = w->getGuiManager()->getInstancesByName("AudioMessageLinkID");
        if(elements.size() > 0)
        {
            GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
            ins->getTextBlockElement()->setText(data[0][3]);
        }

        elements = w->getGuiManager()->getInstancesByName("MessageDescriptionID");
        if(elements.size() > 0)
        {
            GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
            ins->getTextBlockElement()->setText(data[0][4]);
        }
    }
}

std::string getSelectCommand(SimpleWindow* w)
{
    std::string finalResult = "SELECT * FROM MESSAGES";

    if(searchMode == NONE)
    {
        //do nothing else
    }
    else if(searchMode == TITLE)
    {
        std::string titleStr = "";
        auto elements = w->getGuiManager()->getInstancesByName("MessageTitleID");
        if(elements.size() > 0)
        {
            GuiTextBox* ins = (GuiTextBox*)elements.front()->data;
            if(ins!=nullptr)
                titleStr = ins->getTextBlockElement()->getText();
        }
        
        if(titleStr!="")
            finalResult += " WHERE Title LIKE '%" + titleStr + "%'";
    }
    else if(searchMode == DATE)
    {
        std::string dateStr = "";
        auto elements = w->getGuiManager()->getInstancesByName("MessageDateID");
        if(elements.size() > 0)
        {
            GuiDatePicker* ins = (GuiDatePicker*)elements.front()->data;
            if(ins!=nullptr)
            {
                dateStr = ins->getDateString();

                //swap date format from mm/dd/yyyy to yyyy-mm-dd
                std::vector<std::string> splitDate = StringTools::splitString(dateStr, '/');
                if(splitDate.size() == 3)
                {
                    dateStr = splitDate[2] + "-" + splitDate[0] + "-" + splitDate[1];
                }
            }
        }

        if(dateStr != "")
        {
            finalResult += " WHERE DateRecorded >= '" + dateStr + "'";
        }
    }

    if(sortAsc)
    {
        finalResult += " ORDER BY DateRecorded ASC";
    }
    else
    {
        finalResult += " ORDER BY DateRecorded DESC";
    }

    finalResult += " LIMIT " + StringTools::toString(numOfItemsPerPage+1) + " OFFSET " + StringTools::toString( page*numOfItemsPerPage );
    
    finalResult += ';';
    return finalResult;
}

void loadFromDatabase(SimpleWindow* w)
{
    deleteExtraData();
    correctGuiFunction(w);

    GuiList* listForData = nullptr;
    auto temp = w->getGuiManager()->getInstancesByName("DatabaseElementsList");
    if(temp.size() > 0)
    {
        listForData = (GuiList*)temp[0]->data;
    }
    if(listForData == nullptr)
    {
        StringTools::println("ERROR FINDING THE LIST WITH ID=\"DatabaseElementsList\"");
        return;
    }

    auto data = database->executeCommand(getSelectCommand(w), false);
    //format the data
    int rowIndex = 0;
    for(std::vector<std::string>& row : data)
    {
        if(row.size() != 7)
        {
            writeStringToLog("ERROR loading data: Expected row to have 7 columns but got " + StringTools::toString(row.size()), true);
            writeStringToLog("\tRowIndex = " + StringTools::toString(rowIndex));
            break;
        }
        rowIndex++;
        std::string id = row[0];

        GuiList* mainContainer = new GuiList(32, 32, true);
        GuiList* buttonList = new GuiList(0, 0, false);
        GuiList* dataContainer = new GuiList(0, 0, true);

        GuiRectangleButton* editButton = new GuiRectangleButton(0, 0, 64, 24);
        GuiRectangleButton* deleteButton = new GuiRectangleButton(0, 0, 64, 24);
        
        GuiTextBlock* editText = new  GuiTextBlock(0, 0, 64, 24);
        editText->setText("Edit");
        GuiTextBlock* deleteText = new GuiTextBlock(0, 0, 64, 24);
        deleteText->setText("Delete");

        editButton->addChild(editText);
        deleteButton->addChild(deleteText);

        
        editButton->setOnClickReleaseFunction( [w, id](GuiInstance* ins) -> void{
            //Do something
            setEditMode(w, true);
            fillWithID(w, id);
        });

        deleteButton->setOnClickReleaseFunction( [w, id](GuiInstance* ins) -> void{
            //Do something
            setEditMode(w, false);
            database->executeCommand("DELETE FROM MESSAGES WHERE MessageID = " + id);
            System::messageBoxPopup(0, "Status", "Delete Successful");
            
            writeStringToLog("INFO: Attempted Delete to ID="+id, true);
        });
        
        
        GuiTextBlock* idText = new GuiTextBlock(0, 0, -1, -1);
        GuiTextBlock* titleText = new  GuiTextBlock(0, 0, -1, -1);
        GuiTextBlock* dateText = new GuiTextBlock(0, 0, -1, -1);
        
        idText->setText("ID: " + row[0]);
        titleText->setText(row[1]);

        std::string dateString = row[5];
        //swap date format from yyyy-mm-dd to mm/dd/yyyy
        std::vector<std::string> splitDate = StringTools::splitString(dateString, '-');
        if(splitDate.size() == 3)
        {
            dateString = splitDate[1] + "/" + splitDate[2] + "/" + splitDate[0];
        }
        
        dateText->setText("RecordedDate: " + dateString);
        
        dataContainer->setElementSpacing(8);
        dataContainer->addChild(idText);
        dataContainer->addChild(titleText);
        dataContainer->addChild(dateText);
        
        buttonList->setElementSpacing(64);
        buttonList->addChild(editButton);
        buttonList->addChild(deleteButton);

        mainContainer->setElementSpacing(16);
        mainContainer->addChild(dataContainer);
        mainContainer->addChild(buttonList);
        
        //Insert into the layout
        listForData->addChild(mainContainer);
        
        //add all of the created memory to delete later
        stuffToDelete.push_back(mainContainer);
        stuffToDelete.push_back(dataContainer);
        stuffToDelete.push_back(buttonList);

        stuffToDelete.push_back(idText);
        stuffToDelete.push_back(titleText);
        stuffToDelete.push_back(dateText);
        
        stuffToDelete.push_back(editButton);
        stuffToDelete.push_back(editText);
        stuffToDelete.push_back(deleteButton);
        stuffToDelete.push_back(deleteText);
        
        if(rowIndex >= numOfItemsPerPage)
            break; //Only show a few at a time.
    }
    
    if(data.size() >= numOfItemsPerPage+1)
    {
        canGoToNext = true;
    }
    else
    {
        canGoToNext = false;
    }
}

void checkDatabaseFile(SimpleWindow* w)
{
    SimpleDir d = SimpleDir("./sqlStuff");
    int index = d.getReferenceLocation("CMM.db");
    if(index < 0)
    {
        //no file
        return;
    }
    else
    {
        time_t changeTime = d.getLastChangeTime(index);

        if(changeTime != lastUpdateTime)
        {
            loadFromDatabase(w);
            lastUpdateTime = changeTime;
        }
    }
}
