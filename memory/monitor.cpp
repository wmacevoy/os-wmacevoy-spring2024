#include <string>
#include <string.h>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


int cli(const std::vector <std::string> &args);

int main(int argc, char *argv[]) {
  std::vector<std::string> args;
  for (int argi=0; argi<argc; ++argi) {
    args.push_back(argv[argi]);
  }
  return cli(args);
}


int cli(const std::vector <std::string> &args) {
    if (args.size() < 3) {
        std::cerr << "Insufficient arguments" << std::endl;
        return 2;
    }
    int returnCode = 1;
    bool ok = args.at(1) == "pass";

#ifdef _WIN32

    // Concatenate arguments from args[2] onwards
    std::string cmdLine;
    for (size_t i = 2; i < args.size(); ++i) {
        cmdLine += args[i];
        if (i < args.size() - 1) {
            cmdLine += " "; // Add space between arguments
        }
    }

    // Convert command line to wide string for CreateProcess
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, cmdLine.c_str(), -1, nullptr, 0);
    wchar_t* wCmdLine = new wchar_t[bufferSize];
    MultiByteToWideChar(CP_UTF8, 0, cmdLine.c_str(), -1, wCmdLine, bufferSize);

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    // Create the process
    BOOL success = CreateProcessW(NULL,   // Application name
                                   wCmdLine, // Command line (arguments)
                                   NULL,   // Process handle not inheritable
                                   NULL,   // Thread handle not inheritable
                                   FALSE,  // Set handle inheritance to FALSE
                                   0,      // No creation flags
                                   NULL,   // Use parent's environment block
                                   NULL,   // Use parent's starting directory 
                                   &si,    // Pointer to STARTUPINFO structure
                                   &pi);   // Pointer to PROCESS_INFORMATION structure

    delete[] wCmdLine; // Clean up the wide string

    if (!success) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
        return 2;
    }

    // Wait for the process to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get the exit code
    DWORD exitCode;
    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        std::cerr << "GetExitCodeProcess failed (" << GetLastError() << ")" << std::endl;
    } else {
        std::cout << "Process exited with code " << exitCode << " ok=" << ok << std::endl;
        returnCode = ((exitCode == 0 && ok) || (exitCode != 0 && !ok)) ? 0 : 1;
        std::cout << "returnCode=" << returnCode << std::endl;
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#else
  std::cout << "unix monitor" << std::endl;

  pid_t childPid = fork();
  if (childPid == 0) {
    char *arge[args.size()+1-2];
    for (int i=2; i<args.size(); ++i) {
      arge[i-2]=(char *) args[i].c_str();
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
