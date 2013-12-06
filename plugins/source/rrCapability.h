/**
 * @file rrCapability.h
 * @brief RoadRunner Capability plugin class
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
#ifndef rrCapabilityH
#define rrCapabilityH
#include <string>
#include <vector>
#include "rrPluginsAPIExporter.h"
#include "rrParameters.h"
//---------------------------------------------------------------------------

namespace rrp
{

/**
    A Capability. A capability is a basically a container of parameters, with the addition of a name and description.
    \todo Might be better just inherit from Parameters and add the name, descr variables.
*/
class PLUGINS_API_DECLSPEC Capability
{
    public:
        /**
            Create a capability
        */
                                            Capability(const string& name, const string& descr);

        /**
            De allocate any memory associated with the capability
        */
                                           ~Capability();

        /**
            Create a capability from another capability (copy ctor)
        */
                                            Capability(const Capability& fromMe);

        /**
            Get capability name
        */
        string                              getName() const;

        /**
            Get capability description
        */
        string                              getDescription() const;
//        string                              getMethod() const;

        /**
            Get a pointers to the underlying parameter container
        */
        Parameters*                         getParameters();

        /**
            Get a pointer to a parameter with a specific name
        */
        PluginParameter*                    getParameter(const string& paraName);

        /**
            Assign name and description
        */
        void                                setup(const string& name, const string& descr);

        /**
            Add a parameter to the capability
        */
        void                                addParameter(PluginParameter* me);

        /**
            Get the capability as a string
        */
        string                              asString() const;

        /**
            Get number of parameters
        */
        u_int                               nrOfParameters() const;

        /**
            Get a reference to a parameter
        */
        PluginParameter&                    operator[](const int& i);

        /**
            Output the capability to a output stream
        */
        PLUGINS_API_DECLSPEC
        friend ostream&                     operator <<(ostream& os, const Capability& caps);

    protected:
        /**
            Name of capability
        */
        string                              mName;

        /**
            A description of the capability
        */
        string                              mDescription;
//        string                              mMethod;

        /**
            Container for parameters associated with the capability
        */
        Parameters                          mParameters;

};

}
#endif
