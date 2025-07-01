#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "../common/whitespace.h"
#include "../common/exec_tree.h"
#include "../common/utility.h"
#include "../common/error.h"

static struct WS_statement* parse_tuple(PyObject* arg) {
    PyObject* prog;
    if (!PyArg_Parse(arg, "O!:exec_tree", &PyTuple_Type, &prog)) {
        return NULL;
    }
    Py_ssize_t size = PyTuple_Size(prog);
    if (size == -1) {
        return NULL; // Exception in Size
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
            // opcode, no param
            arr[i] = (struct WS_statement){
                .op = (enum WS_operation) op,
            };
        } else if (PyLong_Check(arg_obj)) {
            // opcode, numeric param
            arr[i] = (struct WS_statement){
                .op = (enum WS_operation) op,
                .num = PyLong_AsLongLong(arg_obj),
            };
        } else if (PyUnicode_Check(arg_obj)) {
            // opcode, label param
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
    return arr;
    error_occurred:
    free(arr);
    return NULL;
}

static PyObject* 
exec_tree(PyObject* self, PyObject* arg)
{
    PyObject* prog;
    if (!PyArg_Parse(arg, "O!:exec_tree", &PyTuple_Type, &prog)) {
        return NULL;
    }

    Py_ssize_t size = PyTuple_Size(prog);

    struct WS_statement* arr = parse_tuple(arg);

    if (!arr) return NULL;

    Err_clearError();

    if (!WS_execute(arr, size)) {
        struct Err_Error* err = Err_getError();
        PyErr_Format(PyExc_RuntimeError, "ERROR at position %zu: %s", err->index, err->message);
        return NULL;
    }

    free(arr);
    Py_RETURN_NONE;
}

static PyObject*
parse_and_exec(PyObject* self, PyObject* arg)
{
    PyObject* prog_o;
    if (!PyArg_Parse(arg, "O!:parse_and_exec", &PyUnicode_Type, &prog_o)) {
        return NULL;
    }

    const char* prog = PyUnicode_AsUTF8(prog_o);

    Err_clearError();

    size_t size;
    struct WS_statement* tree = WS_parse(prog, &size);
    if (!tree) {
        goto error;
    }

    if (!WS_execute(tree, size)) {
        goto error;
    }
    Py_RETURN_NONE;
    error:
    free(tree);
    struct Err_Error* err = Err_getError();
    PyErr_Format(PyExc_RuntimeError, "ERROR at position %zu: %s", err->index, err->message);
    return NULL;
}

static PyMethodDef whitespace_methods[] = {
    {"exec_tree", exec_tree, METH_O, "Execute the parsed whitespace tree (a tuple)"},
    {"parse_and_exec", parse_and_exec, METH_O, "Parse and execute whitespace source (a str)"},
    {NULL, NULL, 0, NULL} /* sentinel */
};

static struct PyModuleDef whitespace_module = {
    PyModuleDef_HEAD_INIT,
    "whitespace_module",     /* name of module */
    NULL,         /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module,
                     or -1 if the module keeps state in global variables. */
    whitespace_methods
};

PyMODINIT_FUNC
PyInit_whitespace_module(void)
{
    return PyModule_Create(&whitespace_module);
}
