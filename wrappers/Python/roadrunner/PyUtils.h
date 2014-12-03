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

class Dictionary;

PyObject *Variant_to_py(const Variant& var);

Variant Variant_from_py(PyObject *py);

PyObject *dictionary_keys(const Dictionary* dict);

PyObject *dictionary_values(const Dictionary* dict);

PyObject *dictionary_items(const Dictionary* dict);

PyObject *dictionary_getitem(const Dictionary* dict, const char* key);

PyObject *dictionary_setitem(Dictionary* dict, const char* key, PyObject* value);

void dictionary_delitem(Dictionary* dict, const char* key);

PyObject *dictionary_contains(const Dictionary* dict, const char* key);

} /* namespace rr */

#endif /* PYUTILS_H_ */
