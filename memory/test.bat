g++ -municode -static -std=c++17 -g -O -o monitor.exe monitor.cpp
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor pass true
if %errorlevel% neq 0 exit /b %errorlevel%
g++ -municode -static -std=c++17 -g -O -o testrandomize.cpp randomize.cpp -lbcrypt
.\monitor pass .\testrandomize
g++ -municode -static -std=c++17 -g -O -o testcanary.cpp canary.cpp randomize.cpp -lbcrypt
.\monitor pass .\testcanary --ok=true
.\monitor fail .\testcanary --ok=false
