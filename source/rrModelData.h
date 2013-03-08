#ifndef rrModelDataH
#define rrModelDataH

//Data that is used in SBML models

typedef struct
{
	char*		modelName;
    double*     _y;   
    int         _ySize;   

} ModelData;

#endif
