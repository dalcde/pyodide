#include "jsimport.h"

#include "hiwire.h"
#include "js2python.h"

static PyObject*
Js2Import_GetAttr(PyObject* self, PyObject* attr)
{
  const char* c = PyUnicode_AsUTF8(attr);
  if (c == NULL) {
    return NULL;
  }
  int idval = hiwire_get_global((int)c);
  if (idval == -1) {
    PyErr_Format(PyExc_AttributeError, "Unknown attribute '%s'", c);
    return NULL;
  }
  PyObject* result = js2python(idval);
  hiwire_decref(idval);
  return result;
}

static PyObject*
Js2Import_Dir()
{
  int idwindow = hiwire_get_global((int)"self");
  int iddir = hiwire_dir(idwindow);
  hiwire_decref(idwindow);
  PyObject* pydir = js2python(iddir);
  hiwire_decref(iddir);
  return pydir;
}

static PyMethodDef Js2Module_Methods[] = {
  { "__getattr__",
    (PyCFunction)Js2Import_GetAttr,
    METH_O,
    "Get an object from the global Javascript namespace" },
  { "__dir__",
    (PyCFunction)Js2Import_Dir,
    METH_NOARGS,
    "Returns a list of object name in the global Javascript namespace" },
  { NULL }
};

static struct PyModuleDef Js2Module = {
  PyModuleDef_HEAD_INIT,
  "js2",
  "Provides access to Javascript global variables from Python",
  0,
  Js2Module_Methods
};

PyMODINIT_FUNC PyInit_js2(void) {
  Py_Initialize();
  return PyModule_Create(&Js2Module);
}
