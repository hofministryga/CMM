#include "Functions.h"
#include <glib/BitmapFont.h>

int main()
{
    StringTools::init();
    SimpleGraphics::init();
    GuiManager::initDefaultLoadFunctions();

    BitmapFont bmf = BitmapFont("./Resources/test.fnt");
    SimpleGraphics::setFont(&bmf);

    setBuildTime();
    removeOldLogs();
    checkUpdate();

    WindowOptions options;
    options.threadManaged = false;
    options.iconIsFile = false;
    options.iconFileString = "myIcon";
    options.initFunction = initFunction;

    SimpleWindow w = SimpleWindow("CMM", 1280, 720, -1, -1, options);
    
    int timeTillCheck = 1000;
    bool invalidFix = true;
    while(w.getRunning())
    {
        size_t t1 = System::getCurrentTimeMillis();

        w.update();
        w.guiUpdate();

        if(w.getRepaint())
            w.repaint();
        size_t t2 = System::getCurrentTimeMillis();

        if(t2-t1 <= frameTime)
        {
            size_t waitTime = t2-t1;
            System::sleep(waitTime);
            timeTillCheck -= frameTime;
        }
        else
        {
            timeTillCheck -= t2-t1;
        }

        if(timeTillCheck <= 0)
        {
            checkDatabaseFile(&w);
            timeTillCheck = 1000;
        }
        
        if(invalidFix)
        {
            w.getGuiManager()->invalidateImage();
            invalidFix = false;
        }
    }

    dispose();
    return 0;
}
