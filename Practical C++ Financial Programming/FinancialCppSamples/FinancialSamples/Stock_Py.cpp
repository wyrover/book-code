//
//  Stock_Py.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Stock_Py.h"
#include "Stock.h"

#if 0
#include <Python.h>
#include <pycapsule.h>

#include <stdio.h>

namespace {

void stock_destructor(PyObject *capsule)
{
    printf("calling destructor\n");
    Stock *stock = reinterpret_cast<Stock*>(PyCapsule_GetPointer(capsule, NULL));
    delete stock;
}

PyObject *stock_create(PyObject *self, PyObject *args)
{
    char *ticker;
    double price;
    double dividend;
    if (!PyArg_ParseTuple(args, "sdd", &ticker, &price, &dividend))
        return NULL;

    printf("ticker: %s, price: %lf, dividend: %lf\n", ticker, price, dividend);

    Stock *stock = new Stock(ticker, price, dividend);

    PyObject* stockObj = PyCapsule_New(stock, NULL,  stock_destructor);
    return stockObj;
}

Stock *getStock(PyObject *obj)
{
    if (!PyCapsule_CheckExact(obj))
        printf("error: not a stock object\n");
    fflush(stdout);
    return reinterpret_cast<Stock*>(PyCapsule_GetPointer(obj, NULL));
}

PyObject *returnNone()
{
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *stock_ticker(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O!", &PyCapsule_Type, &obj))
        return NULL;

    Stock *stock = getStock(obj);
    return Py_BuildValue("s", stock->ticker().c_str());
}

PyObject *stock_price(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O!", &PyCapsule_Type, &obj))
        return NULL;

    Stock *stock = getStock(obj);
    return Py_BuildValue("d", stock->price());
}

PyObject *stock_setPrice(PyObject *self, PyObject *args)
{
    double price;
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O!d", &PyCapsule_Type, &obj, &price))
        return NULL;

    Stock *stock = getStock(obj);
    if (!stock)
        return NULL;
    stock->setPrice(price);
    return returnNone();
}

PyObject *stock_dividend(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O!", &PyCapsule_Type, &obj))
        return NULL;

    Stock *stock = getStock(obj);
    if (!stock)
        return NULL;
    return Py_BuildValue("d", stock->dividend());
}

PyObject *stock_setDividend(PyObject *self, PyObject *args)
{
    double dividend;
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O!d", &PyCapsule_Type, &obj, &dividend))
        return NULL;

    Stock *stock = getStock(obj);

    stock->setDividend(dividend);
    return returnNone();
}

PyObject *stock_dividendYield(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O!", &PyCapsule_Type, &obj))
        return NULL;
    Stock *stock = getStock(obj);
    return Py_BuildValue("d", stock->dividendYield());
}

PyMethodDef stockMethods[] = {
    {"new",  stock_create, METH_VARARGS, "Create a new stock object."},
    {"ticker",  stock_ticker, METH_VARARGS, "get ticker for a stock object."},
    {"price",  stock_price, METH_VARARGS, "get price for stock."},
    {"setPrice",  stock_setPrice, METH_VARARGS, "set price for a stock object."},
    {"dividend",  stock_dividend, METH_VARARGS, "get dividend for stock."},
    {"setDividend",  stock_setDividend, METH_VARARGS, "set dividend for a stock object."},
    {"dividendYield",  stock_dividendYield, METH_VARARGS, "get dividend yield for stock."},
    {NULL, NULL, 0, NULL}
};

}

PyMODINIT_FUNC initstock()
{
    Py_InitModule("stock", stockMethods);

}

#endif