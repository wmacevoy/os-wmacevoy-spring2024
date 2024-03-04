#include <string>
#include <string.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

int main(int argc, char *argv[]) {
  int returnCode = 1;
  bool ok = (argc > 1 && strcmp(argv[1],"pass") == 0);
  
#ifdef _WIN32
  std::string cmd;
  for (int i=2; i<argc; ++i) {
    if (i > 2) { cmd += " "; }
    cmd += argv[i];
  }

  char *c_cmd = (char*) malloc(cmd.size() + 1);
  strcpy(c_cmd,cmd.c_str());

  
  STARTUPINFO si = { sizeof(si) };
  PROCESS_INFORMATION pi;

  BOOL success = CreateProcess(NULL,c_cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

  if (success) {
    // Wait for the child process to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
      if (exitCode == 3) {
	returnCode = ok ? 1 : 0;
      } else if (exitCode == 0) {
	returnCode = ok ? 0 : 1;
      }
    }
  }
#else
  pid_t childPid = fork();
  if (childPid == 0) {
    execv(argv[2],argv+2);
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
