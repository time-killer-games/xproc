/*

 MIT License

 Copyright © 2021-2023 Samuel Venable

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

#include <iostream>

#include <cctype>
#include <cstdio>
#include <cstddef>
#include <cstring>

#include "../process.hpp"

int main(int argc, char **argv) {
  std::vector<ngs::ps::NGS_PROCID> pid;
  if (argc >= 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0)) {
    printf("usage: xproc <options>\n  options:\n    -h or -help\n    -e or -exec <command>\n    -f or -file <filename>\n");
    return 0;
  } else if (argc >= 3 && (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-exec") == 0)) {
    std::string command;
    for (int i = 2; i < argc; i++)
      command += std::string(argv[i]) + " ";
    while (!command.empty() && command.back() == ' ')
      command.pop_back();
    ngs::ps::NGS_PROCID proc_id = ngs::ps::spawn_child_proc_id(command, false);
    while (proc_id != 0 && !ngs::ps::child_proc_id_is_complete(proc_id));
    printf("%s", ngs::ps::read_from_stdout_for_child_proc_id(proc_id).c_str());
    ngs::ps::free_stdout_for_child_proc_id(proc_id);
    ngs::ps::free_stdin_for_child_proc_id(proc_id);
    return 0;
  } else if (argc >= 2 && (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-exec") == 0)) {
    std::string command = ngs::ps::read_from_stdin_for_self();
    while (!command.empty() && (command.back() == '\r' || command.back() == '\n' || command.back() == ' '))
      command.pop_back();
    ngs::ps::NGS_PROCID proc_id = ngs::ps::spawn_child_proc_id(command, false);
    while (proc_id != 0 && !ngs::ps::child_proc_id_is_complete(proc_id));
    printf("%s", ngs::ps::read_from_stdout_for_child_proc_id(proc_id).c_str());
    ngs::ps::free_stdout_for_child_proc_id(proc_id);
    ngs::ps::free_stdin_for_child_proc_id(proc_id);
    return 0;
  } else if (argc == 1) {
    pid = ngs::ps::proc_id_enum();
  } else if (argc >= 3) {
    if (!(strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "-file") == 0))
      return 0;
  }
  for (int i = 2; i < argc; i++) {
    std::vector<ngs::ps::NGS_PROCID> exe = ngs::ps::proc_id_from_exe(argv[i]);
    std::vector<ngs::ps::NGS_PROCID> cwd = ngs::ps::proc_id_from_cwd(argv[i]);
    pid.insert(pid.end(), exe.begin(), exe.end());
    pid.insert(pid.end(), cwd.begin(), cwd.end());
  }
  for (std::size_t i = 0; i < pid.size(); i++) {
    std::string exe = ngs::ps::exe_from_proc_id(pid[i]);
    if (!exe.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", exe: " << exe << "\n";
    std::string cwd = ngs::ps::cwd_from_proc_id(pid[i]);
    if (!cwd.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", cwd: " << cwd << "\n";
    std::string comm = ngs::ps::comm_from_proc_id(pid[i]);
    if (!comm.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", comm: " << comm << "\n";
    std::vector<ngs::ps::NGS_PROCID> ppid = ngs::ps::parent_proc_id_from_proc_id(pid[i]);
    if (!ppid.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", ppid: " << ppid[0] << "\n";
    std::vector<ngs::ps::NGS_PROCID> cpid = ngs::ps::proc_id_from_parent_proc_id(pid[i]);
    for (std::size_t j = 0; j < cpid.size(); j++)
      std::cout << "pid[" << i << "]: " << pid[i] << ", cpid[" << j << "]: " << cpid[j] << "\n";
    std::vector<std::string> cmd = ngs::ps::cmdline_from_proc_id(pid[i]);
    for (std::size_t j = 0; j < cmd.size(); j++)
      std::cout << "pid[" << i << "]: " << pid[i] << ", cmd[" << j << "]: " << cmd[j] << "\n";
    std::vector<std::string> env = ngs::ps::environ_from_proc_id(pid[i]);
    for (std::size_t j = 0; j < env.size(); j++)
      std::cout << "pid[" << i << "]: " << pid[i] << ", env[" << j << "]: " << env[j] << "\n";
  }
  return 0;
}
