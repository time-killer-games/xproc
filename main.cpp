/*

 MIT License
 
 Copyright © 2021-2022 Samuel Venable
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

#include <iostream>

#include <cstddef>

#include "apiprocess/xproc.h"

int main() {
  std::vector<ngs::xproc::PROCID> pid = ngs::xproc::proc_id_enum();
  for (std::size_t i = 0; i < pid.size(); i++) {

    std::string exe = ngs::xproc::exe_from_proc_id(pid[i]);
    if (!exe.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", exe: " << exe << std::endl;

    std::string cwd = ngs::xproc::cwd_from_proc_id(pid[i]);
    if (!cwd.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", cwd: " << cwd << std::endl;
  
    std::vector<ngs::xproc::PROCID> ppid = ngs::xproc::parent_proc_id_from_proc_id(pid[i]);
    if (!ppid.empty()) std::cout << "pid[" << i << "]: " << pid[i] << ", ppid: " << ppid[0] << std::endl;

    std::vector<ngs::xproc::PROCID> cpid = ngs::xproc::proc_id_from_parent_proc_id(pid[i]);
    for (std::size_t j = 0; j < cpid.size(); j++)
      std::cout << "pid[" << i << "]: " << pid[i] << ", cpid[" << j << "]: " << cpid[j] << std::endl;

    std::vector<std::string> cmd = ngs::xproc::cmdline_from_proc_id(pid[i]);
    for (std::size_t j = 0; j < cmd.size(); j++)
      std::cout << "pid[" << i << "]: " << pid[i] << ", cmd[" << j << "]: " << cmd[j] << std::endl;

    std::vector<std::string> env = ngs::xproc::environ_from_proc_id(pid[i]);
    for (std::size_t j = 0; j < env.size(); j++)
      std::cout << "pid[" << i << "]: " << pid[i] << ", env[" << j << "]: " << env[j] << std::endl;

  }
  return 0;
}
