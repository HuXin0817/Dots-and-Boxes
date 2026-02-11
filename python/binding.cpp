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
#include <Python.h>

#include <string>
#include <vector>

static PyObject* PyDab_Process(PyObject* self, PyObject* args) {
  PyObject* py_list;
  if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &py_list)) {
    return nullptr;
  }

  Py_ssize_t size = PyList_Size(py_list);
  std::vector<std::string> cpp_args;
  std::vector<char*> argv;

  cpp_args.reserve(size);
  argv.reserve(size + 1);

  for (Py_ssize_t i = 0; i < size; ++i) {
    PyObject* item = PyList_GetItem(py_list, i);
    if (!PyUnicode_Check(item)) {
      PyErr_SetString(PyExc_TypeError, "All arguments must be strings");
      return nullptr;
    }
    Py_ssize_t len;
    const char* str = PyUnicode_AsUTF8AndSize(item, &len);
    if (!str) {
      return nullptr;
    }
    cpp_args.emplace_back(str, len);
    argv.push_back(const_cast<char*>(cpp_args.back().c_str()));
  }
  argv.push_back(nullptr);

  int result = dab::Process(static_cast<int>(size), argv.data());

  return PyLong_FromLong(result);
}

static PyMethodDef PyDab_methods[] = {{"Process", PyDab_Process, METH_VARARGS, "Process command line arguments"},
                                      {nullptr, nullptr, 0, nullptr}};

static struct PyModuleDef PyDab_module = {
    PyModuleDef_HEAD_INIT, "PyDab", "Dots and Boxes game Python binding", -1, PyDab_methods};

PyMODINIT_FUNC PyInit_PyDab(void) {
  PyObject* module = PyModule_Create(&PyDab_module);
  if (!module) {
    return nullptr;
  }

  PyObject* default_player_type = PyLong_FromLong(dab::DefaultPlayerType);
  if (!default_player_type) {
    Py_DECREF(module);
    return nullptr;
  }
  if (PyModule_AddObject(module, "DefaultPlayerType", default_player_type) < 0) {
    Py_DECREF(default_player_type);
    Py_DECREF(module);
    return nullptr;
  }

  for (size_t i = 0; i < std::size(dab::PlayerTypeOptionStrings); ++i) {
    std::string str = dab::PlayerTypeOptionStrings[i];
    if (str.size() > 5) {
      str[5] = '_';
    }
    PyObject* player_type = PyUnicode_FromString(dab::PlayerTypeOptionStrings[i]);
    if (!player_type) {
      Py_DECREF(module);
      return nullptr;
    }
    if (PyModule_AddObject(module, str.c_str(), player_type) < 0) {
      Py_DECREF(player_type);
      Py_DECREF(module);
      return nullptr;
    }
  }

  return module;
}
