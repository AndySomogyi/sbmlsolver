/**
 * @file    flattenModel.java
 * @brief   Flattens the comp code from the given SBML file.
 * @author  Sarah Keating
 *
 * <!--------------------------------------------------------------------------
 * This sample program is distributed under a different license than the rest
 * of libSBML.  This program uses the open-source MIT license, as follows:
 *
 * Copyright (c) 2013-2018 by the California Institute of Technology
 * (California, USA), the European Bioinformatics Institute (EMBL-EBI, UK)
 * and the University of Heidelberg (Germany), with support from the National
 * Institutes of Health (USA) under grant R01GM070923.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Neither the name of the California Institute of Technology (Caltech), nor
 * of the European Bioinformatics Institute (EMBL-EBI), nor of the University
 * of Heidelberg, nor the names of any contributors, may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * ------------------------------------------------------------------------ -->
 */

import org.sbml.libsbml.*;

public class flattenModel
{
  public static void main (String[] args)
  {
    if (! SBMLExtensionRegistry.isPackageEnabled("comp"))
    {
      println("This copy of libSBML does not contain the 'comp' extension");
      println("Unable to proceed with flattening the model.");
      System.exit(2);
    }

    if (args.length < 2 || args.length > 3)
    {
      println("Usage: java flattenModel [-p] input-filename output-filename");
      println(" -p : list unused ports");
      System.exit(3);
    }

    SBMLReader reader  = new SBMLReader();
    SBMLWriter writer  = new SBMLWriter();
    boolean leavePorts = false;

    SBMLDocument doc;
    if (args.length == 2)
    {
      doc = reader.readSBML(args[0]);
    }
    else
    {
      doc = reader.readSBML(args[1]);
      leavePorts = true;
    }

    if (doc.getErrorLog().getNumFailsWithSeverity(libsbml.LIBSBML_SEV_ERROR) > 0)
    {
      doc.printErrors();
    }
    else
    {
      /* create a new conversion properties structure */
      ConversionProperties props = new ConversionProperties();

      /* add an option that we want to flatten */
      props.addOption("flatten comp", true);

      /* add an option to leave ports if the user has requested this */
      props.addOption("leavePorts", leavePorts);

      /* perform the conversion */
      if (doc.convert(props) != libsbml.LIBSBML_OPERATION_SUCCESS)
      {
        println("Conversion failed.");
        doc.printErrors();
        System.exit(4);
      }

      if (args.length == 2)
      {
        writer.writeSBML(doc, args[1]);
      }
      else
      {
        writer.writeSBML(doc, args[2]);
      }
    }
  }


  static void println (String msg)
  {
    System.out.println(msg);
  }


  /**
   * Loads the SWIG-generated libSBML Java module when this class is
   * loaded, or reports a sensible diagnostic message about why it failed.
   */
  static
  {
    try
    {
      System.loadLibrary("sbmlj");
      // For extra safety, check that the jar file is in the classpath.
      Class.forName("org.sbml.libsbml.libsbml");
    }
    catch (UnsatisfiedLinkError e)
    {
      System.err.println("Error encountered while attempting to load libSBML:");
      System.err.println("Please check the value of your "
                         + (System.getProperty("os.name").startsWith("Mac OS")
                            ? "DYLD_LIBRARY_PATH" : "LD_LIBRARY_PATH") +
                         " environment variable and/or your" +
                         " 'java.library.path' system property (depending on" +
                         " which one you are using) to make sure it list the" +
                         " directories needed to find the " +
                         System.mapLibraryName("sbmlj") + " library file and" +
                         " libraries it depends upon (e.g., the XML parser).");
      System.exit(1);
    }
    catch (ClassNotFoundException e)
    {
      System.err.println("Error: unable to load the file 'libsbmlj.jar'." +
                         " It is likely that your -classpath command line " +
                         " setting or your CLASSPATH environment variable " +
                         " do not include the file 'libsbmlj.jar'.");
      e.printStackTrace();

      System.exit(1);
    }
    catch (SecurityException e)
    {
      System.err.println("Error encountered while attempting to load libSBML:");
      e.printStackTrace();
      System.err.println("Could not load the libSBML library files due to a"+
                         " security exception.\n");
      System.exit(1);
    }
  }
}
