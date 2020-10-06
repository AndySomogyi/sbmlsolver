/**
 * @file telCPlugin.h
 * @brief RoadRunner C plugin class
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
#ifndef telCPluginH
#define telCPluginH
#include "telPlugin.h"
#include "telStringList.h"
//---------------------------------------------------------------------------

namespace tlp
{

/**
Typedef for the plugins execute function
*/
typedef bool  (*executeF)(bool);

/**
Typedefs for various required plugin functions
*/
typedef bool  (*destroyF)();
typedef char* (*getACharStarF)();
typedef void* (*getAPropertyF)(const char*);
/**
 * The CPlugin class is containing the framework to load plugins that are written in pure C.
 * The shared library need to export an execute and destroy function.
 */
class CORE_DECLSPEC CPlugin : public Plugin
{
    friend class PluginManager;
    public:
                                //!Create a plugin written in C
        CPlugin(const string& name, const string& cat = gNoneString);// , rrc::THostInterface* aRR = NULL);

                                //!De allocate the plugin
        virtual                ~CPlugin();
        string                  getImplementationLanguage();
        virtual bool            execute(bool useThread = false);
        tlp::StringList         getPropertyNames();
        PropertyBase*           getProperty(const string& param);

        /** \brief Function to retrieve last error in a C plugin
        */
        string                  getLastError();

    protected:

        /**
          Function pointer to a execute function. The shared library need to provide this function.
            This function pointer is assigned as the plugin is loaded.
         */
        executeF                executeFunction;

        /**
          Function pointer to a destroy function. The shared library need to provide this function.
            This function pointer is assigned as the plugin is loaded.
         */
        destroyF                destroyFunction;

        /**
          Function pointer to retrieve the C plugins list of properties.
         */
        getACharStarF           getCPropertyNames;

        getAPropertyF           getCProperty;

        getACharStarF           getCLastError;
};

}
#endif
