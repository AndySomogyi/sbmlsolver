The LibRoadRunner C Library
============================

RoadRunner is a SBML compliant high performance and portable simulation engine
for systems and synthetic biology. To run a simple SBML model
and generate time series data we would write the following code:

.. code-block:: C

    #undef __cplusplus
    #define STATIC_RRC
    #include <stdio.h>
    #include <stdlib.h>
    #include "rrc_api.h"
    #include "rrc_types.h"
    #include "rrc_utilities.h"
    int main (int argc, char *argv[]) {
        RRHandle rrHandle;
        RRCDataPtr result;

        printf ("Starting Test Program %s\n", argv[0]);
        rrHandle = createRRInstance();
        if (!loadSBMLFromFile (rrHandle, "feedback.xml")) {
            printf ("Failed to load model: %s\n", getLastError ());
            getchar ();
            exit (0);
        }
        result = simulateEx (rrHandle, 0, 10, 100);
        printf (rrCDataToString (result));
        freeRRCData(result);

        getchar ();
        exit (0);
    }

More complex example, using C wrappers:

.. code-block:: C

    #undef __cplusplus
    #define STATIC_RRC
    #include <stdio.h>
    #include <stdlib.h>
    #include "rrc_api.h"
    #include "rrc_types.h"
    #include "rrc_utilities.h"
    int main (int argc, char *argv[]) {
       RRHandle rrHandle;
       RRCDataPtr result;
       int index;
       int col;
       int row;
       printf ("Starting Test Program %s\n", argv[0]);
       rrHandle = createRRInstance();
       if (!loadSBML (rrHandle, "feedback.xml")) {
          printf ("Error while loading SBML file\n");
          printf ("Error message: %s\n", getLastError());
          getchar ();
          exit (0);
       }
       result = simulateEx (rrHandle, 0, 10, 10);  // start time, end time, and number of points
       index = 0;
       // Print out column headers... typically time and species.
       for (col = 0; col < result->CSize; col++)
       {
          printf ("%10s", result->ColumnHeaders[index++]);
          if (col < result->CSize - 1)
          {
             printf ("\t");
          }
       }
       printf ("\n");
       index = 0;
       // Print out the data
       for (row = 0; row < result->RSize; row++)
       {
          for (col = 0; col < result->CSize; col++)
          {
             printf ("%10f", result->Data[index++]);
             if (col < result->CSize -1)
             {
                printf ("\t");
             }
          }
       printf ("\n");
       }
       //Cleanup
       freeRRCData (result);
       freeRRInstance (rrHandle);
       getchar ();
       exit (0);
    }

Would create output as shown below:

.. code-block::

    Starting Test Program: <File path Here>
         time            [S1]            [S2]            [S3]            [S4]
     0.000000        0.000000        0.000000        0.000000        0.000000
     1.111111        3.295975        1.677255        1.121418        1.074708
     2.222222        0.971810        1.658970        1.841065        2.192728
     3.333333        0.137340        0.501854        1.295138        2.444883
     4.444445        0.141470        0.200937        0.549172        1.505662
     5.555556        1.831017        1.317792        1.129982        1.351300
     6.666667        0.306310        0.775477        1.304950        1.952076
     7.777778        0.193459        0.268986        0.628542        1.483161
     8.888889        1.566864        1.219950        1.105718        1.370199
    10.000000        0.269437        0.678127        1.199353        1.868247


License
--------

.. code-block::

    Copyright (C) 2012-2015
    University of Washington, Seattle, WA, USA

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    In plain english this means:

    You CAN freely download and use this software, in whole or in part, for personal,
    company internal, or commercial purposes;

    You CAN use the software in packages or distributions that you create.

    You SHOULD include a copy of the license in any redistribution you may make;

    You are NOT required include the source of software, or of any modifications you may
    have made to it, in any redistribution you may assemble that includes it.

    YOU CANNOT:

    redistribute any piece of this software without proper attribution;




.. doxygenfunction:: rrc::getInstallFolder



