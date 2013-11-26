#pragma hdrstop
#include "rrp_cpp_support.h"
#include "rrRoadRunnerData.h"
#include "rrException.h"
#include "rrStringUtils.h"

namespace rrp
{
using namespace rr;

Parameters* castToParameters(RRParametersHandle handle)
{
    Parameters* para = (Parameters*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameters handle");
        throw(ex);
    }
}

BaseParameter* castToParameter(RRParameterHandle handle)
{
    BaseParameter* para = (BaseParameter*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameter handle");
        throw(ex);
    }
}

Parameter<int>* castToIntParameter(RRParameterHandle handle)
{
    Parameter<int>* para = (Parameter<int>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameter handle");
        throw(ex);
    }
}

Parameter<double>* castToDoubleParameter(RRParameterHandle handle)
{
    Parameter<double>* para = (Parameter<double>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameter handle");
        throw(ex);
    }
}

Parameter<char*>* castToStringParameter(RRParameterHandle handle)
{
    Parameter<char*>* para = (Parameter<char*>*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid Parameter handle");
        throw(ex);
    }
}

MinimizationData* castToMinimizationData(RRMinimizationDataHandle handle)
{
    MinimizationData* para = (MinimizationData*) handle;
    if(para) //Will only fail if handle is NULL...
    {
        return para;
    }
    else
    {
        Exception ex("Failed to cast to a valid MinimizationData handle");
        throw(ex);
    }
}

RRCDataPtr createRRCData(const RoadRunnerData& result)
{
    RRCData* rrCData  = new RRCData;
    memset(rrCData, 0, sizeof(RRCData));

    rrCData->ColumnHeaders = new char*[result.cSize()];
    for(int i = 0; i < result.cSize(); i++)
    {
        rrCData->ColumnHeaders[i] = rr::createText(result.getColumnNames()[i]);
    }

    rrCData->RSize = result.rSize();
    rrCData->CSize = result.cSize();
    int size = rrCData->RSize*rrCData->CSize;
    rrCData->Data = new double[size];

    if(result.hasWeights())
    {
        rrCData->Weights = new double[size];
    }

    int index = 0;
    //The data layout is simple row after row, in one single long row...
    for(int row = 0; row < rrCData->RSize; row++)
    {
        for(int col = 0; col < rrCData->CSize; col++)
        {
            rrCData->Data[index] = result(row, col);
            if(result.hasWeights())
            {
                rrCData->Weights[index] = result.weight(row, col);
            }
            ++index;
        }
    }
    return rrCData;
}


}