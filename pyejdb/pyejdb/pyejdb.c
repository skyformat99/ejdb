#include "pyejdb.h"

/* HDB */
typedef struct {
    PyObject_HEAD
    EJDB *ejdb;
} PEJDB;

#include "EJDB.c"

PyDoc_STRVAR(ejdb_m_doc, "EJDB http://ejdb.org");
PyDoc_STRVAR(ejdb_version_doc, "version() -> str\n\n Returns the version string of the underlying EJDB library.");

static PyObject* ejdb_version(PyObject *module) {
    return PyString_FromString(tcversion);
}

/* cabinet_module.m_methods */
static PyMethodDef pyejdb_m_methods[] = {
    {"version", (PyCFunction) ejdb_version, METH_NOARGS, ejdb_version_doc},
    {NULL} /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3
/* pyejdb_module */
static PyModuleDef pyejdb_module = {
    PyModuleDef_HEAD_INIT,
    "pyejdb", /*m_name*/
    ejdb_m_doc, /*m_doc*/
    -1, /*m_size*/
    pyejdb_m_methods, /*m_methods*/
};
#endif

PyObject* init_pyejdb() {
    PyObject *pyejdb;
    if (PyType_Ready(&EJDBType)) {
        return NULL;
    }
#if PY_MAJOR_VERSION >= 3
    pyejdb = PyModule_Create(&pyejdb_module);
#else
    pyejdb = Py_InitModule3("pyejdb", pyejdb_m_methods, pyejdb_m_doc);
#endif
    if (!pyejdb) {
        return NULL;
    }
    Error = PyErr_NewException("pyejdb.Error", NULL, NULL);
    Py_XINCREF(Error);
    if (!Error || PyModule_AddObject(pyejdb, "Error", Error)) {
        Py_XDECREF(Error);
        goto fail;
    }

    /* adding types and constants */
    if (
            PyModule_AddType(pyejdb, "EJDB", &EJDBType) ||

            PyModule_AddIntMacro(pyejdb, JBOREADER) ||
            PyModule_AddIntMacro(pyejdb, JBOWRITER) ||
            PyModule_AddIntMacro(pyejdb, JBOCREAT) ||
            PyModule_AddIntMacro(pyejdb, JBOTRUNC) ||
            PyModule_AddIntMacro(pyejdb, JBONOLCK) ||
            PyModule_AddIntMacro(pyejdb, JBOLCKNB) ||
            PyModule_AddIntMacro(pyejdb, JBOTSYNC) ||

            PyModule_AddIntMacro(pyejdb, JBIDXDROP) ||
            PyModule_AddIntMacro(pyejdb, JBIDXDROPALL) ||
            PyModule_AddIntMacro(pyejdb, JBIDXOP) ||
            PyModule_AddIntMacro(pyejdb, JBIDXOP) ||
            PyModule_AddIntMacro(pyejdb, JBIDXREBLD) ||
            PyModule_AddIntMacro(pyejdb, JBIDXNUM) ||
            PyModule_AddIntMacro(pyejdb, JBIDXSTR) ||
            PyModule_AddIntMacro(pyejdb, JBIDXARR) ||
            PyModule_AddIntMacro(pyejdb, JBIDXISTR) ||

            PyModule_AddIntMacro(pyejdb, JBQRYCOUNT)
            ) {

        goto fail;
    }

    return pyejdb;

fail:
#if PY_MAJOR_VERSION >= 3
    Py_DECREF(pyejdb);
#endif
    return NULL;
}

#if PY_MAJOR_VERSION >= 3

PyMODINIT_FUNC PyInit_pyejdb(void) {
    return init_pyejdb();
}
#else

PyMODINIT_FUNC initcabinet(void) {
    init_pyejdb();
}
#endif

