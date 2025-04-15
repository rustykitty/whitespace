#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ws_operation.h"

#include "exec_tree.c"

#include "debug.h"

static PyObject* 
parse_python_tree(PyObject* self, PyObject* arg) 
{
    PyObject* prog;
    if (!PyArg_Parse(arg, "O!:parse_python_tree", &PyTuple_Type, &prog)) {
        return NULL;
    }
    Py_ssize_t size = PyTuple_Size(prog);
    if (size == -1) {
        return NULL; // Exception in Size
    }
    {
            PyObject* repr = PyObject_Repr(prog);
            if (repr == NULL) {
                return NULL;
            } else {
                Py_DECREF(repr);
            }
        }
    struct WS_statement* arr = malloc(size * sizeof(struct WS_statement));
    for (size_t i = 0; i < (size_t)size; i++) {
        // item: (op, arg?)
        PyObject* item = PyTuple_GetItem(prog, i);
        if (item == NULL) { // Exception in GetItem
            goto error_occurred;
        } else if (!PyTuple_Check(item)) {
            PyErr_Format(PyExc_TypeError, "prog index %zu: expected tuple, got %s instead", 
                         i, Py_TYPE(item)->tp_name);
            goto error_occurred;
        } else if (0 == PyTuple_Size(item) || PyTuple_Size(item) > 2) {
            PyErr_Format(PyExc_TypeError, "prog index %zu, expected tuple of length 1 or 2, but got tuple of length %zu instead", 
                            i, PyTuple_Size(item));
            goto error_occurred;
        }
        enum WS_operation op;
        PyObject* arg_obj = NULL;
        if (!PyArg_ParseTuple(item, "i|O", &op, &arg_obj)) {
            PyErr_Format(PyExc_TypeError, "prog index %zu: expected types (int, int | str?), got (%s, %s) instead", 
                         i, Py_TYPE(item)->tp_name, arg_obj ? Py_TYPE(arg_obj)->tp_name : "nothing");
            goto error_occurred;
        }
        if (arg_obj == NULL || arg_obj == Py_None) {
            arr[i] = (struct WS_statement){
                .op = (enum WS_operation) op,
            };
        } else if (PyLong_Check(arg_obj)) {
            arr[i] = (struct WS_statement){
                .op = (enum WS_operation) op,
                .num = PyLong_AsLongLong(arg_obj),
            };
        } else if (PyUnicode_Check(arg_obj)) {
            arr[i] = (struct WS_statement){
                .op = (enum WS_operation) op,
                .label = PyUnicode_AsUTF8(arg_obj),
            };
        } else {
            PyErr_Format(PyExc_TypeError, "prog index %zu: expected types (int, int | str?), got (%s, %s) instead", 
                         i, Py_TYPE(item)->tp_name, Py_TYPE(arg_obj)->tp_name);
            goto error_occurred;
        }
    }
    // We're good to go!
    DEBUG("Calling wsexecute now");
    struct wstree_err* err = wsexecute(arr, size);
    if (err) {
        PyErr_Format(PyExc_RuntimeError, "ERROR at position %zu: %s", err->index, err->message);
        goto error_occurred;
    }
    DEBUG("return from wsexecute");
    // cleanup
    free(arr);
    Py_RETURN_NONE;
    error_occurred:
    free(arr);
    return NULL;
}

static PyMethodDef wsexecute_methods[] = {
    {"parse_python_tree", parse_python_tree, METH_O, "Execute the parsed whitespace tree"},
    {NULL, NULL, 0, NULL} /* sentinel */
};

static struct PyModuleDef wsexecute_module = {
    PyModuleDef_HEAD_INIT,
    "parse_python_tree",     /* name of module */
    NULL,         /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module,
                     or -1 if the module keeps state in global variables. */
    wsexecute_methods
};

PyMODINIT_FUNC
PyInit_parse_python_tree(void)
{
    return PyModule_Create(&wsexecute_module);
}
