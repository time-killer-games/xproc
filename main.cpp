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

#include "apiprocess/process.h"

using std::string;
using std::wstring;
using std::vector;
using std::to_string;
using std::size_t;

using ngs::proc::PROCID;
using ngs::proc::PROCLIST;
using ngs::proc::PROCINFO;
#if defined(PROCESS_GUIWINDOW_IMPL)
using ngs::proc::WINDOWID;
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
  #if defined(PROCESS_GUIWINDOW_IMPL)
  std::cout << "    --wid-enum                 " << std::endl;
  std::cout << "    --wid-from-pid   pid       " << std::endl;
  std::cout << "    --pid-from-wid   wid       " << std::endl;
  std::cout << "    --wid-exists     wid       " << std::endl;
  std::cout << "    --wid-kill       wid       " << std::endl;
  #endif
}

void PrintPidEnumeration() {
  PROCID *proc_id = nullptr; int size = 0;
  ngs::proc::proc_id_enumerate(&proc_id, &size);
  if (proc_id) {
    for (int i = 0; i < size; i++) {
      std::cout << "proc_id[" << i << "]: " << proc_id[i] << std::endl;
    }
    ngs::proc::free_proc_id(proc_id);
  }
}

void PrintWhetherPidExists(PROCID proc_id) {
  std::cout << "proc_id: " << proc_id << ", process_exists: " << ((ngs::proc::proc_id_exists(proc_id)) ? "true" : "false") << std::endl;
}

void PrintWhetherPidKilled(PROCID proc_id) {
  std::cout << "proc_id: " << proc_id << ", kill_succeeded: " << ((ngs::proc::proc_id_kill(proc_id)) ? "true" : "false") << std::endl;
}

void PrintPpidFromPid(PROCID proc_id) {
  if (!ngs::proc::proc_id_exists(proc_id)) return;
  PROCID parent_proc_id = 0;
  ngs::proc::parent_proc_id_from_proc_id(proc_id, &parent_proc_id);
  std::cout << "proc_id: " << proc_id << ", parent_process_id: " << parent_proc_id << std::endl;
}

void PrintPidFromPpid(PROCID parent_proc_id) {
  if (!ngs::proc::proc_id_exists(parent_proc_id)) return;
  PROCID *proc_id = nullptr; int size = 0;
  ngs::proc::proc_id_from_parent_proc_id(parent_proc_id, &proc_id, &size);
  if (proc_id) {
    for (int i = 0; i < size; i++) {
      std::cout << "proc_id: " << parent_proc_id << ", child_process_id[" << i << "]: " << proc_id[i] << std::endl;
    }
    ngs::proc::free_proc_id(proc_id);
  }
}

void PrintExeFromPid(PROCID proc_id) {
  if (!ngs::proc::proc_id_exists(proc_id)) return;
  char *buffer = nullptr;
  ngs::proc::exe_from_proc_id(proc_id, &buffer);
  if (buffer && strlen(buffer)) {
    std::cout << "proc_id: " << proc_id << ", executable_image_file_path: " << buffer << std::endl;
  }
}

void PrintCwdFromPid(PROCID proc_id) {
  if (!ngs::proc::proc_id_exists(proc_id)) return;
  char *buffer = nullptr;
  ngs::proc::cwd_from_proc_id(proc_id, &buffer);
  if (buffer && strlen(buffer)) {
    std::cout << "proc_id: " << proc_id << ", current_working_directory: " << buffer << std::endl;
  }
}

void PrintCmdFromPid(PROCID proc_id) {
  if (!ngs::proc::proc_id_exists(proc_id)) return;
  char **buffer = nullptr; int size = 0;
  ngs::proc::cmdline_from_proc_id(proc_id, &buffer, &size);
  if (buffer) {
    for (int i = 0; i < size; i++) {
      std::cout << "proc_id: " << proc_id << ", commandline[" << i << "]: " << buffer[i] << std::endl;
    }
    ngs::proc::free_cmdline(buffer);
  }
}

void PrintEnvFromPid(PROCID proc_id, const char *name) {
  if (!ngs::proc::proc_id_exists(proc_id)) return;
  if (name) {
    char *value = nullptr;
    ngs::proc::environ_from_proc_id_ex(proc_id, name, &value);
    if (value) {
      std::cout << value << std::endl;
      return;
    }
  }
  char **buffer = nullptr; int size = 0;
  ngs::proc::environ_from_proc_id(proc_id, &buffer, &size);
  if (buffer) {
    for (int i = 0; i < size; i++) {
      std::cout << "proc_id: " << proc_id << ", environment[" << i << "]: " << buffer[i] << std::endl;
    }
    ngs::proc::free_environ(buffer);
  }
}

#if defined(PROCESS_GUIWINDOW_IMPL)
void PrintWidEnumeration() {
  WINDOWID *win_id = nullptr; int size = 0;
  ngs::proc::window_id_enumerate(&win_id, &size);
  if (win_id) {
    for (int i = 0; i < size; i++) {
      std::cout << "window_id[" << i << "]: " << win_id[i] << std::endl;
    }
    ngs::proc::free_window_id(win_id);
  }
}

void PrintWhetherWidExists(WINDOWID win_id) {
  std::cout << "window_id: " << win_id << ", window_exists: " << ((ngs::proc::window_id_exists(win_id)) ? "true" : "false") << std::endl;
}

void PrintWhetherWidKilled(WINDOWID win_id) {
  std::cout << "window_id: " << win_id << ", kill_succeeded: " << ((ngs::proc::window_id_kill(win_id)) ? "true" : "false") << std::endl;
}

void PrintWidFromPid(PROCID proc_id) {
  if (!ngs::proc::proc_id_exists(proc_id)) return;
  WINDOWID *win_id = nullptr; int size = 0;
  ngs::proc::window_id_from_proc_id(proc_id, &win_id, &size);
  if (win_id) {
    for (int i = 0; i < size; i++) {
      std::cout << "proc_id: " << proc_id << ", owned_window_id[" << i << "]: " << win_id[i] << std::endl;
    }
    ngs::proc::free_window_id(win_id);
  }
}

void PrintPidFromWid(WINDOWID win_id) {
  PROCID proc_id = 0;
  ngs::proc::proc_id_from_window_id(win_id, &proc_id);
  if (proc_id) {
    std::cout << "window_id: " << win_id << ", owned_window_id: " << proc_id << std::endl;
  }
}
#endif

void PrintAllProcInfo(PROCID proc_id) {
  PROCINFO procInfo = ngs::proc::proc_info_from_proc_id(proc_id);
  if (ngs::proc::proc_id_exists(proc_id)) {
    if (ngs::proc::executable_image_file_path(procInfo) && strlen(ngs::proc::executable_image_file_path(procInfo))) {
      std::cout << "proc_id: " << proc_id << ", executable_image_file_path: " << ngs::proc::executable_image_file_path(procInfo) << std::endl;
    }
    if (ngs::proc::current_working_directory(procInfo) && strlen(ngs::proc::current_working_directory(procInfo))) {
      std::cout << "proc_id: " << proc_id << ", current_working_directory: " << ngs::proc::current_working_directory(procInfo) << std::endl;
    }
    if (ngs::proc::parent_process_id(procInfo)) {
      std::cout << "proc_id: " << proc_id << ", parent_process_id: " << ngs::proc::parent_process_id(procInfo) << std::endl;
    }
    if (ngs::proc::child_process_id(procInfo)) {
      for (int i = 0; i < ngs::proc::child_process_id_length(procInfo); i++) {
        std::cout << "proc_id: " << proc_id << ", child_process_id[" << i << "]: " << ngs::proc::child_process_id(procInfo, i) << std::endl;
      }
    }
    if (ngs::proc::commandline(procInfo)) {
      for (int i = 0; i < ngs::proc::commandline_length(procInfo); i++) {
        std::cout << "proc_id: " << proc_id << ", commandline[" << i << "]: " << ngs::proc::commandline(procInfo, i) << std::endl;
      }
    }
    if (ngs::proc::environment(procInfo)) {
      for (int i = 0; i < ngs::proc::environment_length(procInfo); i++) {
        std::cout << "proc_id: " << proc_id << ", environment[" << i << "]: " << ngs::proc::environment(procInfo, i) << std::endl;
      }
    }
    #if defined(PROCESS_GUIWINDOW_IMPL)
    if (ngs::proc::owned_window_id(procInfo)) {
      for (int i = 0; i < ngs::proc::owned_window_id_length(procInfo); i++) {
        std::cout << "proc_id: " << proc_id << ", owned_window_id[" << i << "]: " << ngs::proc::owned_window_id(procInfo, i) << std::endl;
      }
    }
    #endif
  }
  ngs::proc::free_proc_info(procInfo);
}

void PrintAllInfoForAllProcessIds() {
  PROCLIST proc_list = ngs::proc::proc_list_create();
  for (int i = 0; i < ngs::proc::process_id_length(proc_list); i++) {
    PrintAllProcInfo(ngs::proc::process_id(proc_list, i));
  }
  ngs::proc::free_proc_list(proc_list);
}	

} // namespace XProcPrint

int main(int argc, char **argv) {
  if (argc == 1 || (argc >= 2 && strcmp(argv[1], "--help") == 0)) {
    XProcPrint::PrintXProcHelp();
  } else if (argc == 2) {
    if (strcmp(argv[1], "--pid-enum") == 0) {
      XProcPrint::PrintPidEnumeration();
    } else if (strcmp(argv[1], "--info-from-all") == 0) {
      XProcPrint::PrintAllInfoForAllProcessIds();
    }
    #if defined(PROCESS_GUIWINDOW_IMPL)
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
    }
    #if defined(PROCESS_GUIWINDOW_IMPL)
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
