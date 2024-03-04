#include <string>
#include <string.h>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <sstream>
#include <windows.h>
#include <codecvt>
#else
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


int cli(const std::vector <std::string> &args);

#ifndef _WIN32
int main(int argc, char *argv[]) {
  std::vector<std::string> args;
  for (int argi=0; argi<argc; ++argi) {
    args.push_back(argv[argi]);
  }
  return cli(args);
}
#else
std::wstring wstrFromStr(const std::string &str) {
  size_t size = MultiByteToWideChar( CP_UTF8 , 0 , str.c_str() , -1, NULL , 0 );
  std::wstring ans(size,0);
  MultiByteToWideChar( CP_UTF8 , 0 , str.c_str() , -1, &ans[0] , size);
  return ans;
}

std::string strFromWstr(const std::wstring& wstr)
{
  size_t size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
  std::string ans(size,0);
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),-1, &ans[0], ans.size(), NULL,NULL);
  return ans;
}

#ifdef UNICODE
using sysstring = std::wstring;
sysstring sysstr(const std::wstring &wstr) {
  return wstr;
}
sysstring sysstr(const std::string &str) {
  return wstrFromStr(str);
}
std::string str(const sysstring &sstr) {
  return strFromWstr(sstr);
}
std::wstring wstr(const sysstring &sstr) {
  return sstr;
}

#define WinMain wWinMain
#else
using sysstring = std::string;
sysstring sysstr(const std::wstring &wstr) {
  return strFromWstr(wstr);
}
sysstring sysstr(const std::string &str) {
  return str;
}

std::string str(const sysstring &sstr) {
  return sstr;
}
std::wstring wstr(const sysstring &sstr) {
  return wstrFromStr(sstr);
}

#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, sysstring::value_type *pCmdLine, int nCmdShow) {
  std::vector < std::string > args;
  sysstring sysCmdLine = pCmdLine;
  std::string cmdLine = str(sysCmdLine);

  args.push_back("monitor");
  std::istringstream iss(cmdLine);
  std::string arg;
  while (iss >> arg) {
    std::cout << "win32 arg[" << args.size() << "]=" << arg << std::endl;
    args.push_back(arg);
  }
  return cli(args);
}
#endif

int cli(const std::vector <std::string> &args)
{
  int returnCode = 1;
  bool ok = args.at(1) == "pass";
  
#ifdef _WIN32
  std::cout << "win32 monitor" << std::endl;

  sysstring cmd;
  for (int i=2; i<args.size(); ++i) {
    if (i > 2) { cmd += ' '; }
    cmd += sysstr(args[i]);
  }
  STARTUPINFO si = { sizeof(si) };
  PROCESS_INFORMATION pi;

  std::cout << "win32 create process cmd=" << str(cmd) << std::endl;

  BOOL success = CreateProcess(NULL,(sysstring::value_type*) cmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

  if (success) {
    // Wait for the child process to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
      int expect = ok ? 0 : 3221225781;
      std::cout << "monitor " << pi.hProcess << " code " << exitCode << " (expected " << expect << ")"
		<< " -- " << ( exitCode == expect ? "pass" : "fail" ) << std::endl;
      if (exitCode == expect) {
	returnCode = 0;
      }
    } else {
      std::cout << "win32 get exit code failed" << std::endl;
    }
  } else {
    std::cout << "win32 process creation failed" << std::endl;
  }
#else
  std::cout << "unix monitor" << std::endl;

  pid_t childPid = fork();
  if (childPid == 0) {
    char *arge[args.size()+1-2];
    for (int i=2; i<args.size(); ++i) {
      arge[i-2]=args[i].c_str();
    }
    arge[args.size()-2]=NULL;
    execv(arge[0],arge);
    std::cout << "exec failed." << std::endl;        
    exit(1);
  } else if (childPid > 0) {
    int status;
  
    if ( waitpid(childPid, &status, 0) != -1 ) {
      if ( WIFEXITED(status) ) {
	int exitStatus = WEXITSTATUS(status);
	int expect = ok ? 0 : -1;
	std::cout << "monitor " << childPid << " exit " << exitStatus << " (expected " << expect << ")"
		  << " -- " << ( exitStatus == expect ? "pass" : "fail" ) << std::endl;
	if (exitStatus == expect) {
	  returnCode = 0;
	}
      }
      if ( WIFSIGNALED(status) ) {
	int exitSignal = WTERMSIG(status);
	int expect = !ok ? SIGABRT : -1;
	std::cout << "monitor " << childPid << " signal " << exitSignal << " (expected " << expect << ")"
		  << " -- " << ( exitSignal == expect ? "pass" : "fail" ) << std::endl;	  
	if (exitSignal == expect) {
	  returnCode = 0;
	}
      }
    }
  }
#endif

  return returnCode;
}
