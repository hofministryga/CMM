#include "Functions.h"

void initHeader(SimpleWindow* w)
{
    GuiContextMenu* wbSearchContextMenu = nullptr;

	auto elements = w->getGuiManager()->getInstancesByName("WindowBar_Edit");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            if(mode == EDIT)
                editData(w);
            else
                insertData(w);
        });
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_SearchContextMenu");
    if(elements.size() > 0)
    {
        GuiContextMenu* but = (GuiContextMenu*)elements.front()->data;
        but->setShowOnRightClick(false);
        but->setCanHideOnClick(false);
        wbSearchContextMenu = but;
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_Search");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;

        // but->setOnClickFunction( [w, wbSearchContextMenu](GuiInstance* ins) ->void {
        //     wbSearchContextMenu->hideMenu();
        // });

        but->setOnClickFunction( [w, wbSearchContextMenu](GuiInstance* ins) ->void {
            GuiRectangleButton* insAsBut = (GuiRectangleButton*)ins;
            if(!wbSearchContextMenu->isMenuShowing())
            {
                wbSearchContextMenu->showMenu( ins->getX() + insAsBut->getWidth()/3 , ins->getY() + insAsBut->getHeight() + 8);
            }
            else
            {
                wbSearchContextMenu->hideMenu();
            }
        });
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_NoSearch");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            searchMode = NONE;

            auto searchElements = w->getGuiManager()->getInstancesByName("WindowBar_Status2");
            if(searchElements.size() > 0)
            {
                GuiTextBlock* textIns = (GuiTextBlock*)searchElements.front()->data;
                textIns->setText("");
            }

            loadFromDatabase(w);
        });
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_SearchT");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            searchMode = TITLE;

            auto searchElements = w->getGuiManager()->getInstancesByName("WindowBar_Status2");
            if(searchElements.size() > 0)
            {
                GuiTextBlock* textIns = (GuiTextBlock*)searchElements.front()->data;
                textIns->setText("Searching by Title");
            }

            loadFromDatabase(w);
        });
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_SearchD");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            searchMode = DATE;
            
            auto searchElements = w->getGuiManager()->getInstancesByName("WindowBar_Status2");
            if(searchElements.size() > 0)
            {
                GuiTextBlock* textIns = (GuiTextBlock*)searchElements.front()->data;
                textIns->setText("Searching by Date");
            }

            loadFromDatabase(w);
        });
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_Upload");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            System::messageBoxPopup(0, "Status", "Uploading database file");
            writeStringToLog("INFO: Attempting to upload database");
            uploadDatabase();
            System::messageBoxPopup(0, "Status", "Finished database file");
        });
    }

    elements = w->getGuiManager()->getInstancesByName("ModeSwitchID");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            setEditMode(w, INSERT);
        });
    }

    elements = w->getGuiManager()->getInstancesByName("WindowBar_Help");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [](GuiInstance* ins) -> void {
            system("\"readme.pdf\"");
        });
    }
}