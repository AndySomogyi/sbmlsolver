using System;
using System.IO;

using System.Collections.Generic;
using System.Linq;
using System.Text;

using LibRoadRunner;


namespace RRTest
{
	class Program
	{
		static void Main(string[] args)
		{
			RoadRunner rr = new RoadRunner();
			string modelsRootPath = "R:\\SBMLTestCases";
			string caseList = modelsRootPath + "\\xmlfileList.txt";

			string[] models = System.IO.File.ReadAllLines(caseList);

			int i = 0;
			foreach(string model in models)			
			{
				models[i++] = modelsRootPath +"\\all\\" + model;				
			}

			foreach (string model in models)
			{
				if (model.Contains("00001"))
				{
					System.Console.WriteLine(model);
					string sbml = System.IO.File.ReadAllText(model);

					rr.loadSBML(sbml);
					string modelCode = rr.getCSharpCode();
					//RoadRunner.SimulateSBMLFile(sbml, true);
					rr.simulate();

					//Write the code to file
					string currentModel = Path.GetFileNameWithoutExtension(model) + ".cs";
					string outPath = "C:\\RRW\\Testing\\rr_code_output\\cs_from_rr_cs";
					string outFName = outPath + "\\" + currentModel;
					System.IO.File.WriteAllText(outFName, modelCode);
				}
			}
		}
	}
}
