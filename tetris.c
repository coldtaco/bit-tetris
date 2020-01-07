
#include "helper.h"
#include <Python.h>


void playGame(struct tetris *game){
    while (!game->board[0]){
        bestMove(game);
        game->bagind++;
        if (game->bagind >= 14){
            game->bagind = 0;
            game->bag = newBag();
        }
        //printBoard(game->board);
    }
}
PyObject* eval(PyObject* self, PyObject* args){
    PyObject * listObj;
    if (! PyArg_ParseTuple( args, "O", &listObj)){return NULL;}
    long length = PyList_Size(listObj);
    struct tetris *game = calloc(1,sizeof(tetris));
    initializeTetris(game);
    float *sVector = calloc(length, sizeof(float));
    for (int i = 0; i < length; i++){
        sVector[i] = PyFloat_AsDouble(PyList_GetItem(listObj,i));
    }
    game->sVector = sVector;
    playGame(game);
    long cleared = game->cleared;
    freeGame(game);
    return Py_BuildValue("i", -cleared);
}
/*    
addList is the module name, and this is the initialization block of the module.
<desired module name>, <the-info-table>, <module's-docstring>
*/
static PyMethodDef TetrisMethods[] = {
    {"eval",  eval, METH_VARARGS,
     "Evaluates tetris score"},
};
static struct PyModuleDef tetrismodule = {
    PyModuleDef_HEAD_INIT,
    "tetris",   /* name of module */
    "tetris in c", /* module documentation, may b   e NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    TetrisMethods
};

PyMODINIT_FUNC PyInit_tetris(void) {
    return PyModule_Create(&tetrismodule);
}
