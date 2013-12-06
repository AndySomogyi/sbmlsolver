/**
 * @file rrPluginParameter.h
 * @brief RoadRunner Parameter class implementation
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/
#ifndef rrPluginParameterH
#define rrPluginParameterH
#include "rrPluginsAPIExporter.h"
#include <string>
#include <ostream>
//---------------------------------------------------------------------------

namespace rrp
{

using std::string;
using std::ostream;

/**
 * The PluginParameter class is an interface for derived Parameters, that can be of any type.
 * A parameter has a name and a hint, encapsulating the the purpose of the actual parameter.
 */
class PLUGINS_API_DECLSPEC PluginParameter
{
    public:
        /**
         * The constructor populates the parameters name and hint.
         */
                                            PluginParameter(const string& name, const string& hint);
        /**
         * De-allocate any memory allocated.
         */
        virtual                            ~PluginParameter();

        /**
         * Return the parameter in a string representation
         */
        string                              asString() const;

        /**
         * Return the parameters name
         */
        string                              getName() const;

        /**
         * Return the parameters hint
         */
        string                              getHint() const;

        //Pure virtuals
        /**
         * Get the value of the parameter, as a string. This function need to be implemented in any derived class.
         \note In the case of a complex type, such as RoadRunnerData for example, expect the returned string to be
         of considerate size.
         */
        virtual string                      getValueAsString() const;

        /**
         * Get the type of the parameter. This function need to be implemented in any derived class.
         */
        virtual string                      getType() const;

        /**
         * Get an opaque pointer to the value of the parameter. This function need to be implemented in any derived class.
         */
        virtual void*                       getValueHandle();

        /**
         * Set the value of the parameter, using string representation. This function need to be implemented in any derived class.
         */
        virtual void                        setValueFromString(const string& value) = 0;

        /**
         * Ouput the parameter to an output stream. This function need to be implemented in any derived class.
         */
        PLUGINS_API_DECLSPEC
        friend ostream&                     operator<<(ostream& stream, const PluginParameter& outMe);

    protected:
        /**
         * The name of the parameter.
         */
        string                              mName;

        /**
         * The hint of the parameter.
         */
        string                              mHint;

};

}
#endif

