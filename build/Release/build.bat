@echo OFF
ninja -f ./build/Release/build.ninja -v
rc ./app/CMM.rc
set debugOptions=
set linkOptions=%WLIBPATH64% %WLIBVALUES% -L C:/Libraries/_MyLibraries/GLIB/libs/Release -l GLIB -L C:/SQLITE/lib -l sqlite3_x64
set extraOptions= -O3 -Wl,-subsystem:windows -Wl,-entry:mainCRTStartup
set projectCommand=./bin/Release/obj/*.o ./app/CMM.res -o ./bin/Release/CMM.exe
clang++ %debugOptions% %linkOptions% %extraOptions% %projectCommand%