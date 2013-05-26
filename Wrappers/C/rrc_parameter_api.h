#ifndef rrc_parameter_apiH
#define rrc_parameter_apiH
#include "rrc_exporter.h"
#include "rrc_types.h"
//---------------------------------------------------------------------------

#if defined(__cplusplus)
namespace rrc
{
extern "C"
{
#endif

/*!
 \brief Get string list of parameters for a particular capability
 \param[in] rrHandle, capability_name
 \return Returns RRStringList if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC RRStringArrayHandle rrcCallConv getListOfParameters(RRHandle handle, const char* cap_name);

/*!
 \brief Set a parameter
 \param[in] rrHandle, capability_name and parameter name
 \return Returns true if sucessful, false otherwise
 \ingroup simulation
*/
C_DECL_SPEC bool rrcCallConv setParameter(RRParameterHandle handle, const char* value);

/*!
 \brief Get a parameter handle
 \param[in] rrHandle, capability_name and parameter name
 \return Returns RRParameterHandle if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC RRParameterHandle rrcCallConv getParameter(RRHandle handle, const char* cap_name, const char* parName);

/*!
 \brief Get a parameters value as char*
 \param[in] RRParameterHandle
 \return Returns the parameters value if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getParameterValueAsString(RRParameterHandle handle);

/*!
 \brief Get a parameters value as a void*
 \param[in] RRParameterHandle
 \return Returns the parameters value as a pointer if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC void* rrcCallConv getParameterValueAsPointer(RRParameterHandle handle);

/*!
 \brief Get a parameters name
 \param[in] RRParameterHandle
 \return Returns the parameters name if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getParameterName(RRParameterHandle handle);

/*!
 \brief Get a parameters hint
 \param[in] RRParameterHandle
 \return Returns the parameters hint if sucessful, NULL otherwise
 \ingroup simulation
*/
C_DECL_SPEC char* rrcCallConv getParameterHint(RRParameterHandle handle);


#if defined(__cplusplus)
}	//Extern "C"

}	//rrc namespace
#endif


#endif
