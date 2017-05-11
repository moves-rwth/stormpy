/*
 * common.h
 *
 *  Created on: 15 Apr 2016
 *      Author: hbruintjes
 */

#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <tuple>
#include <exception>

namespace py = pybind11;

#if PY_MAJOR_VERSION >= 3
#define PY_DIV "__truediv__"
#define PY_RDIV "__rtruediv__"
#else
#define PY_DIV "__div__"
#define PY_RDIV "__rdiv__"
#endif

struct NoPickling: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Pickling support not implemented!";
    }
};

