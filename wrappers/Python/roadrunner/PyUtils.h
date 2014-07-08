/*
 * PyUtils.h
 *
 *  Created on: Apr 27, 2014
 *      Author: andy
 */

#ifndef PYUTILS_H_
#define PYUTILS_H_

#include "Variant.h"
#include <Python.h>

namespace rr
{

PyObject *Variant_to_py(const Variant& var);

Variant Variant_from_py(PyObject *py);


} /* namespace rr */

#endif /* PYUTILS_H_ */
