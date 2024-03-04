set CXXFLAGS=-std=c++17 -g -O
set LDFLAGS=-lbcrypt -Wl,--subsystem,console
g++ %CXXFLAGS% -o monitor.exe monitor.cpp %LDFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%

g++ %CXXFLAGS% -o testrandomize.exe testrandomize.cpp randomize.cpp  %LDFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor pass .\testrandomize
if %errorlevel% neq 0 exit /b %errorlevel%

g++ %CXXFLAGS% -o testcanary.exe testcanary.cpp canary.cpp randomize.cpp %LDFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor pass .\testcanary --ok=true
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor fail .\testcanary --ok=false
if %errorlevel% neq 0 exit /b %errorlevel%

g++ %CXXFLAGS% -o testsecuredata.exe testsecuredata.cpp securedata.cpp canary.cpp randomize.cpp %LDFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor pass .\testsecuredata --ok=true
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor fail .\testsecuredata --ok=false
if %errorlevel% neq 0 exit /b %errorlevel%

g++ %CXXFLAGS% -o testsecurearray.exe testsecurearray.cpp securedata.cpp canary.cpp randomize.cpp %LDFLAGS%
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor pass .\testsecurearray --ok=true
if %errorlevel% neq 0 exit /b %errorlevel%
.\monitor fail .\testsecurearray --ok=false
if %errorlevel% neq 0 exit /b %errorlevel%
