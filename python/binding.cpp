/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <Dab/Command.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

int Process(const std::vector<std::string>& args) {
  int argc = args.size();
  char** argv = new char*[argc + 1];

  for (int i = 0; i < argc; ++i) {
    argv[i] = const_cast<char*>(args[i].c_str());
  }
  argv[argc] = nullptr;

  int result = dab::Process(argc, argv);
  delete[] argv;

  return result;
}

PYBIND11_MODULE(PyDab, m) {
  m.doc() = "Dots and Boxes game Python binding";

  m.attr("DefaultPlayerType") = dab::DefaultPlayerType;
  py::list player_type_options;
  for (size_t i = 0; i < std::size(dab::PlayerTypeOptionStrings); ++i) {
    std::string str = dab::PlayerTypeOptionStrings[i];
    if (str.size() > 5) {
      str[5] = '_';
    }
    m.attr(str.c_str()) = dab::PlayerTypeOptionStrings[i];
  }

  m.def("Process", Process, "Process command line arguments");
}