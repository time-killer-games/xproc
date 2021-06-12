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

#include <unordered_map>
#if defined(XPROCESS_GUIWINDOW_IMPL)
#if defined(_WIN32)
#include <windows.h>
#else
#if (defined(__APPLE__) && defined(__MACH__)) && !defined(XPROCESS_XQUARTZ_IMPL)
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#elif (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__) || defined(XPROCESS_XQUARTZ_IMPL)
#include <X11/Xlib.h>
#endif
#endif
#endif

namespace CrossProcess {

#if !defined(_WIN32)
typedef int PROCID;
#else
typedef unsigned long PROCID;
#endif
typedef PROCID PROCESS;
#if defined(XPROCESS_GUIWINDOW_IMPL)
#if defined(_WIN32)
typedef HWND WINDOW;
#elif (defined(__APPLE__) && defined(__MACH__)) && !defined(XPROCESS_XQUARTZ_IMPL)
typedef CGWindowID WINDOW;
#elif (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__) || defined(XPROCESS_XQUARTZ_IMPL)
typedef Window WINDOW;
#endif
typedef char *WINDOWID;
#endif
typedef int PROCLIST;
typedef int PROCINFO;
#if !defined(_MSC_VER)
#pragma pack(push, 8)
#else
#include <pshpack8.h>
#endif
typedef struct {
  PROCID *ProcessId;
  int ProcessIdLength;
} _PROCLIST;
typedef struct {
  PROCID ProcessId;
  char *ExecutableImageFilePath;
  char *CurrentWorkingDirectory;
  PROCID ParentProcessId;
  PROCID *ChildProcessId;
  int ChildProcessIdLength;
  char **CommandLine;
  int CommandLineLength;
  char **Environment;
  int EnvironmentLength;
  #if defined(XPROCESS_GUIWINDOW_IMPL)
  WINDOWID *OwnedWindowId;
  int OwnedWindowIdLength;
  #endif
} _PROCINFO;
#if !defined(_MSC_VER)
#pragma pack(pop)
#else
#include <poppack.h>
#endif

void ProcIdEnumerate(PROCID **procId, int *size);
void FreeProcId(PROCID *procId);
void ProcIdFromSelf(PROCID *procId);
void ParentProcIdFromSelf(PROCID *parentProcId);
bool ProcIdExists(PROCID procId);
bool ProcIdKill(PROCID procId);
void ParentProcIdFromProcId(PROCID procId, PROCID *parentProcId);
void ProcIdFromParentProcId(PROCID parentProcId, PROCID **procId, int *size);
void ExeFromProcId(PROCID procId, char **buffer);
const char *DirectoryGetCurrentWorking();
bool DirectorySetCurrentWorking(const char *dname);
void CwdFromProcId(PROCID procId, char **buffer);
void FreeCmdline(char **buffer);
void CmdlineFromProcId(PROCID procId, char ***buffer, int *size);
void ParentProcIdFromProcIdSkipSh(PROCID procId, PROCID *parentProcId);
void ProcIdFromParentProcIdSkipSh(PROCID parentProcId, PROCID **procId, int *size);
const char *EnvironmentGetVariable(const char *name);
bool EnvironmentSetVariable(const char *name, const char *value);
void FreeEnviron(char **buffer);
void EnvironFromProcId(PROCID procId, char ***buffer, int *size);
void EnvironFromProcIdEx(PROCID procId, const char *name, char **value);
PROCINFO ProcInfoFromProcId(PROCID procId);
void FreeProcInfo(PROCINFO procInfo);
PROCLIST ProcListCreate();
PROCINFO ProcessInfo(PROCLIST procList, int i);
int ProcessInfoLength(PROCLIST procList);
void FreeProcList(PROCINFO procInfo);
#if defined(XPROCESS_GUIWINDOW_IMPL)
WINDOWID WindowIdFromNativeWindow(WINDOW window);
WINDOW NativeWindowFromWindowId(WINDOWID winid);
void WindowIdEnumerate(WINDOWID **winId, int *size);
void ProcIdFromWindowId(WINDOWID winId, PROCID *procId);
void WindowIdFromProcId(PROCID procId, WINDOWID **winId, int *size);
void FreeWindowId(WINDOWID *winId);
bool WindowIdExists(WINDOWID winId);
bool WindowIdKill(WINDOWID winId);
#endif

static int procInfoIndex = -1;
static int procListIndex = -1;
static std::unordered_map<PROCINFO, _PROCINFO *> procInfoMap;
static std::unordered_map<PROCLIST, _PROCLIST *> procListMap;

inline PROCID ProcessId(PROCINFO procInfo) { return procInfoMap[procInfo]->ProcessId; }
inline char *ExecutableImageFilePath(PROCINFO procInfo) { return procInfoMap[procInfo]->ExecutableImageFilePath; }
inline char *CurrentWorkingDirectory(PROCINFO procInfo) { return procInfoMap[procInfo]->CurrentWorkingDirectory; }
inline PROCID ParentProcessId(PROCINFO procInfo) { return procInfoMap[procInfo]->ParentProcessId; }
inline PROCID *ChildProcessId(PROCINFO procInfo) { return procInfoMap[procInfo]->ChildProcessId; }
inline PROCID ChildProcessId(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->ChildProcessId[i]; }
inline int ChildProcessIdLength(PROCINFO procInfo) { return procInfoMap[procInfo]->ChildProcessIdLength; }
inline char **CommandLine(PROCINFO procInfo) { return procInfoMap[procInfo]->CommandLine; }
inline char *CommandLine(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->CommandLine[i]; }
inline int CommandLineLength(PROCINFO procInfo) { return procInfoMap[procInfo]->CommandLineLength; }
inline char **Environment(PROCINFO procInfo) { return procInfoMap[procInfo]->Environment; }
inline char *Environment(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->Environment[i]; }
inline int EnvironmentLength(PROCINFO procInfo) { return procInfoMap[procInfo]->EnvironmentLength; }
#if defined(XPROCESS_GUIWINDOW_IMPL)
inline WINDOWID *OwnedWindowId(PROCINFO procInfo) { return procInfoMap[procInfo]->OwnedWindowId; }
inline WINDOWID OwnedWindowId(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->OwnedWindowId[i]; }
inline int OwnedWindowIdLength(PROCINFO procInfo) { return procInfoMap[procInfo]->OwnedWindowIdLength; }
#endif

PROCESS ProcessExecute(const char *command);
PROCESS ProcessExecuteAsync(const char *command);
void ExecutedProcessWriteToStandardInput(PROCESS procIndex, const char *input);
const char *ExecutedProcessReadFromStandardOutput(PROCESS procIndex);
void FreeExecutedProcessStandardInput(PROCESS procIndex);
void FreeExecutedProcessStandardOutput(PROCESS procIndex);
bool CompletionStatusFromExecutedProcess(PROCESS procIndex);

} // namespace CrossProcess

