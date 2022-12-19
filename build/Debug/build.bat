@echo OFF
ninja -f ./build/Debug/build.ninja -v
rc ./app/CMM.rc
set debugOptions=-g
set linkOptions=%WLIBPATH64% %WLIBVALUES% -L C:/Libraries/_MyLibraries/GLIB/libs/Release -l GLIB -L C:/SQLITE/lib -l sqlite3_x64
set extraOptions= 
set projectCommand=./bin/Debug/obj/*.o ./app/CMM.res -o ./bin/Debug/CMM.exe
clang++ %debugOptions% %linkOptions% %extraOptions% %projectCommand%