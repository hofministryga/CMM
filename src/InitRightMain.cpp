#include "Functions.h"

void initRightMain(SimpleWindow* w)
{
    auto elements = w->getGuiManager()->getInstancesByName("SortStyle");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) ->void {
            sortAsc = !sortAsc;
            if(sortAsc)
            {
                ins->getChildrenRef()[0]->setVisible(true);
                ins->getChildrenRef()[1]->setVisible(false);
            }
            else
            {
                ins->getChildrenRef()[0]->setVisible(false);
                ins->getChildrenRef()[1]->setVisible(true);
            }
            loadFromDatabase(w);
        });
    }

    elements = w->getGuiManager()->getInstancesByName("PageMinusBut");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) -> void{
            if(page > 0)
            {
                page--;
                auto moreElements = w->getGuiManager()->getInstancesByName("PageNum");
                if(moreElements.size() > 0)
                {
                    GuiTextBlock* textIns = (GuiTextBlock*)moreElements.front()->data;
                    textIns->setText( StringTools::toString(page+1) );
                }

                loadFromDatabase(w);
            }
        });
    }

    elements = w->getGuiManager()->getInstancesByName("PagePlusBut");
    if(elements.size() > 0)
    {
        GuiRectangleButton* but = (GuiRectangleButton*)elements.front()->data;
        but->setOnClickReleaseFunction( [w](GuiInstance* ins) -> void{
            if(canGoToNext)
            {
                page++;
                auto moreElements = w->getGuiManager()->getInstancesByName("PageNum");
                if(moreElements.size() > 0)
                {
                    GuiTextBlock* textIns = (GuiTextBlock*)moreElements.front()->data;
                    textIns->setText( StringTools::toString(page+1) );
                }

                loadFromDatabase(w);
            }
        });
    }

    elements = w->getGuiManager()->getInstancesByName("DatabaseElementsList");
    GuiList* databaseList = nullptr;
    if(elements.size() > 0)
        databaseList = (GuiList*)elements.front()->data;
    else
    {
        writeStringToLog("ERROR: Could not find GuiInstance ID=DatabaseElementsList", true);
        writeStringToLog("\tCritical Error. Exiting program");

        exit(-1);
    }
}