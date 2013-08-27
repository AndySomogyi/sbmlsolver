/*
 * rrCModelDataUtil.h
 *
 *  Created on: Aug 8, 2013
 *      Author: andy
 */

#ifndef RRCMODELDATAUTIL_H_
#define RRCMODELDATAUTIL_H_

#include "rrOSSpecifics.h"
#include "rrModelData.h"

#include <ostream>

namespace rr
{

/**
 * dump the ModelData to an output stream.
 */
RR_DECLSPEC std::ostream& operator<< (std::ostream& os, const ModelData& data);


/**
 * zero out the memory occupied by a ModelData struct, equivalent to
 * memset(&data, 0, sizeof(ModelData));
 *
 * performs NO allocaation of memory.
 *
 */
RR_DECLSPEC void initModelData(ModelData &data);

/**
 * Allocate memory for all the data buffers in a ModelData structure,
 * (with the exception of the stoichiometry struct),
 * based on what each buffer size variable indicates, i.e.
 * if data.ySize is 10, data.y will now point to a length 10 double array.
 * The data.modelName field will be a newly allocated null terminated c string.
 *
 * The ModelData structure buffers should be freed with freeModelDataBuffers.
 *
 * The following arrays are also allocated here, the model
 * populates these with pointers to static strings.
 *
 * char**                              variableTable;
 * char**                              boundaryTable;
 * char**                              globalParameterTable;
 */
void RR_DECLSPEC allocModelDataBuffers(ModelData &data, const std::string& modelName);

/**
 * Free all the data referenced by the ModelData pointers.
 * Does NOT free the ModelData itself, ONLY the buffer data.
 */
void RR_DECLSPEC freeModelDataBuffers(ModelData &data);

} /* namespace rr */
#endif /* RRCMODELDATAUTIL_H_ */
