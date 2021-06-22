/*

 MIT License
 
 Copyright © 2021 Samuel Venable
 Copyright © 2021 Lars Nilsson
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "crossprocess.h"

using std::string;
using std::wstring;
using std::vector;
using std::to_string;
using std::size_t;

using CrossProcess::PROCID;
using CrossProcess::PROCLIST;
using CrossProcess::PROCINFO;
#if defined(XPROCESS_GUIWINDOW_IMPL)
using CrossProcess::WINDOWID;
#endif

namespace XProcPrint {

void PrintXProcHelp() {
  std::cout << "usage: xproc [options]         " << std::endl;
  std::cout << "  options:                     " << std::endl;
  std::cout << "    --help                     " << std::endl;
  std::cout << "    --pid-enum                 " << std::endl;
  std::cout << "    --pid-exists     pid       " << std::endl;
  std::cout << "    --pid-kill       pid       " << std::endl;
  std::cout << "    --ppid-from-pid  pid       " << std::endl;
  std::cout << "    --pid-from-ppid  pid       " << std::endl;
  std::cout << "    --exe-from-pid   pid       " << std::endl;
  std::cout << "    --cwd-from-pid   pid       " << std::endl;
  std::cout << "    --cmd-from-pid   pid       " << std::endl;
  std::cout << "    --env-from-pid   pid [name]" << std::endl;
  std::cout << "    --info-from-pid  pid       " << std::endl;
  std::cout << "    --info-from-all            " << std::endl;
  #if defined(XPROCESS_GUIWINDOW_IMPL)
  std::cout << "    --wid-enum                 " << std::endl;
  std::cout << "    --wid-from-pid   pid       " << std::endl;
  std::cout << "    --pid-from-wid   wid       " << std::endl;
  std::cout << "    --wid-exists     wid       " << std::endl;
  std::cout << "    --wid-kill       wid       " << std::endl;
  #endif
}

void PrintPidEnumeration() {
  PROCID *procId = nullptr; int size = 0;
  CrossProcess::ProcIdEnumerate(&procId, &size);
  if (procId) {
    for (int i = 0; i < size; i++) {
      std::cout << "ProcessId[" << i << "]: " << procId[i] << std::endl;
    }
    CrossProcess::FreeProcId(procId);
  }
}

void PrintWhetherPidExists(PROCID procId) {
  std::cout << "ProcessId: " << procId << ", ProcessExists: " << ((CrossProcess::ProcIdExists(procId)) ? "True" : "False") << std::endl;
}

void PrintWhetherPidKilled(PROCID procId) {
  std::cout << "ProcessId: " << procId << ", KillSucceeded: " << ((CrossProcess::ProcIdKill(procId)) ? "True" : "False") << std::endl;
}

void PrintPpidFromPid(PROCID procId) {
  if (!CrossProcess::ProcIdExists(procId)) return;
  PROCID parentProcId = 0;
  CrossProcess::ParentProcIdFromProcId(procId, &parentProcId);
  std::cout << "ProcessId: " << procId << ", ParentProcessId: " << parentProcId << std::endl;
}

void PrintPidFromPpid(PROCID parentProcId) {
  if (!CrossProcess::ProcIdExists(parentProcId)) return;
  PROCID *procId = nullptr; int size = 0;
  CrossProcess::ProcIdFromParentProcId(parentProcId, &procId, &size);
  if (procId) {
    for (int i = 0; i < size; i++) {
      std::cout << "ProcessId: " << parentProcId << ", ChildProcessId[" << i << "]: " << procId[i] << std::endl;
    }
    CrossProcess::FreeProcId(procId);
  }
}

void PrintExeFromPid(PROCID procId) {
  if (!CrossProcess::ProcIdExists(procId)) return;
  char *buffer = nullptr;
  CrossProcess::ExeFromProcId(procId, &buffer);
  if (buffer) {
    std::cout << "ProcessId: " << procId << ", ExecutableImageFilePath: " << buffer << std::endl;
  }
}

void PrintCwdFromPid(PROCID procId) {
  if (!CrossProcess::ProcIdExists(procId)) return;
  char *buffer = nullptr;
  CrossProcess::CwdFromProcId(procId, &buffer);
  if (buffer) {
    std::cout << "ProcessId: " << procId << ", CurrentWorkingDirectory: " << buffer << std::endl;
  }
}

void PrintCmdFromPid(PROCID procId) {
  if (!CrossProcess::ProcIdExists(procId)) return;
  char **buffer = nullptr; int size = 0;
  CrossProcess::CmdlineFromProcId(procId, &buffer, &size);
  if (buffer) {
    for (int i = 0; i < size; i++) {
      std::cout << "ProcessId: " << procId << ", CommandLine[" << i << "]: " << buffer[i] << std::endl;
    }
    CrossProcess::FreeCmdline(buffer);
  }
}

void PrintEnvFromPid(PROCID procId, const char *name) {
  if (!CrossProcess::ProcIdExists(procId)) return;
  if (name) {
    char *value = nullptr;
    CrossProcess::EnvironFromProcIdEx(procId, name, &value);
    if (value) {
      std::cout << value << std::endl;
      return;
    }
  }
  char **buffer = nullptr; int size = 0;
  CrossProcess::EnvironFromProcId(procId, &buffer, &size);
  if (buffer) {
    for (int i = 0; i < size; i++) {
      std::cout << "ProcessId: " << procId << ", Environment[" << i << "]: " << buffer[i] << std::endl;
    }
    CrossProcess::FreeEnviron(buffer);
  }
}

#if defined(XPROCESS_GUIWINDOW_IMPL)
void PrintWidEnumeration() {
  WINDOWID *winId = nullptr; int size = 0;
  CrossProcess::WindowIdEnumerate(&winId, &size);
  if (winId) {
    for (int i = 0; i < size; i++) {
      std::cout << "WindowId[" << i << "]: " << winId[i] << std::endl;
    }
    CrossProcess::FreeWindowId(winId);
  }
}

void PrintWhetherWidExists(WINDOWID winId) {
  std::cout << "WindowId: " << winId << ", WindowExists: " << ((CrossProcess::WindowIdExists(winId)) ? "True" : "False") << std::endl;
}

void PrintWhetherWidKilled(WINDOWID winId) {
  std::cout << "WindowId: " << winId << ", KillSucceeded: " << ((CrossProcess::WindowIdKill(winId)) ? "True" : "False") << std::endl;
}

void PrintWidFromPid(PROCID procId) {
  if (!CrossProcess::ProcIdExists(procId)) return;
  WINDOWID *winId = nullptr; int size = 0;
  CrossProcess::WindowIdFromProcId(procId, &winId, &size);
  if (winId) {
    for (int i = 0; i < size; i++) {
      std::cout << "ProcessId: " << procId << ", OwnedWindowId[" << i << "]: " << winId[i] << std::endl;
    }
    CrossProcess::FreeWindowId(winId);
  }
}

void PrintPidFromWid(WINDOWID winId) {
  PROCID procId = 0;
  CrossProcess::ProcIdFromWindowId(winId, &procId);
  if (procId) {
    std::cout << "WindowId: " << winId << ", OwnerProcessId: " << procId << std::endl;
  }
}
#endif

void PrintAllProcInfo(PROCID procId) {
  PROCINFO procInfo = CrossProcess::ProcInfoFromProcId(procId);
  if (CrossProcess::ProcIdExists(procId)) {
    if (CrossProcess::ExecutableImageFilePath(procInfo)) {
      std::cout << "ProcessId: " << procId << ", ExecutableImageFilePath:\n" << CrossProcess::ExecutableImageFilePath(procInfo) << std::endl;
    }
    if (CrossProcess::CurrentWorkingDirectory(procInfo)) {
      std::cout << "ProcessId: " << procId << ", CurrentWorkingDirectory:\n" << CrossProcess::CurrentWorkingDirectory(procInfo) << std::endl;
    }
    if (CrossProcess::ParentProcessId(procInfo)) {
      std::cout << "ProcessId: " << procId << ", ParentProcessId:\n" << CrossProcess::ParentProcessId(procInfo) << std::endl;
    }
    if (CrossProcess::ChildProcessId(procInfo)) {
      for (int i = 0; i < CrossProcess::ChildProcessIdLength(procInfo); i++) {
        std::cout << "ProcessId: " << procId << ", ChildProcessId[" << i << "]: " << CrossProcess::ChildProcessId(procInfo, i) << std::endl;
      }
    }
    if (CrossProcess::CommandLine(procInfo)) {
      for (int i = 0; i < CrossProcess::CommandLineLength(procInfo); i++) {
        std::cout << "ProcessId: " << procId << ", CommandLine[" << i << "]: " << CrossProcess::CommandLine(procInfo, i) << std::endl;
      }
    }
    if (CrossProcess::Environment(procInfo)) {
      for (int i = 0; i < CrossProcess::EnvironmentLength(procInfo); i++) {
        std::cout << "ProcessId: " << procId << ", Environment[" << i << "]: " << CrossProcess::Environment(procInfo, i) << std::endl;
      }
    }
    #if defined(XPROCESS_GUIWINDOW_IMPL)
    if (CrossProcess::OwnedWindowId(procInfo)) {
      for (int i = 0; i < CrossProcess::OwnedWindowIdLength(procInfo); i++) {
        std::cout << "ProcessId: " << procId << ", OwnedWindowId[" << i << "]: " << CrossProcess::OwnedWindowId(procInfo, i) << std::endl;
      }
    }
    #endif
  }
  CrossProcess::FreeProcInfo(procInfo);
}

void PrintAllInfoForAllProcessIds() {
  PROCLIST procList = CrossProcess::ProcListCreate();
  for (int i = 0; i < CrossProcess::ProcessIdLength(procList); i++) {
    PrintAllProcInfo(CrossProcess::ProcessId(procList, i));
  }
}	

} // namespace XProcPrint

int main(int argc, char **argv) {
  if (argc == 1 || (argc >= 2 && strcmp(argv[1], "--help") == 0)) {
    XProcPrint::PrintXProcHelp();
  } else if (argc == 2) {
    if (strcmp(argv[1], "--pid-enum") == 0) {
      XProcPrint::PrintPidEnumeration();
    }
    #if defined(XPROCESS_GUIWINDOW_IMPL)
    else if (strcmp(argv[1], "--wid-enum") == 0) {
      XProcPrint::PrintWidEnumeration();
    }
    #endif
  } else if (argc >= 3) {
    PROCID pid = (PROCID)strtoul(argv[2], nullptr, 10);
    if (strcmp(argv[1], "--pid-exists") == 0) {
      XProcPrint::PrintWhetherPidExists(pid);
    } else if (strcmp(argv[1], "--pid-kill") == 0) {
      XProcPrint::PrintWhetherPidKilled(pid);
    } else if (strcmp(argv[1], "--ppid-from-pid") == 0) {
      XProcPrint::PrintPpidFromPid(pid);
    } else if (strcmp(argv[1], "--pid-from-ppid") == 0) {
      XProcPrint::PrintPidFromPpid(pid);
    } else if (strcmp(argv[1], "--exe-from-pid") == 0) {
      XProcPrint::PrintExeFromPid(pid);
    } else if (strcmp(argv[1], "--cwd-from-pid") == 0) {
      XProcPrint::PrintCwdFromPid(pid);
    } else if (strcmp(argv[1], "--cmd-from-pid") == 0) {
      XProcPrint::PrintCmdFromPid(pid);
    } else if (strcmp(argv[1], "--env-from-pid") == 0) {
      if (argc == 3) {
        XProcPrint::PrintEnvFromPid(pid, nullptr);
      } else if (argc > 3) {
        XProcPrint::PrintEnvFromPid(pid, argv[3]);
      }
    } else if (strcmp(argv[1], "--info-from-pid") == 0) {
      XProcPrint::PrintAllProcInfo(pid);
    } else if (strcmp(argv[1], "--info-from-all") == 0) {
      XProcPrint::PrintAllInfoForAllProcessIds();
    }
    #if defined(XPROCESS_GUIWINDOW_IMPL)
    else if (strcmp(argv[1], "--wid-from-pid") == 0) {
      XProcPrint::PrintWidFromPid(pid);
    } else if (strcmp(argv[1], "--pid-from-wid") == 0) {
      XProcPrint::PrintPidFromWid((WINDOWID)argv[2]);
    } else if (strcmp(argv[1], "--wid-exists") == 0) {
      XProcPrint::PrintWhetherWidExists((WINDOWID)argv[2]);	
    } else if (strcmp(argv[1], "--wid-kill") == 0) {
      XProcPrint::PrintWhetherWidKilled((WINDOWID)argv[2]);	
    }
    #endif
  }
  return 0;
}
