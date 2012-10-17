#ifndef rrCapsSupportH
#define rrCapsSupportH
#include <vector>
#include "rrObject.h"
#include "rrCapabilitiesSection.h"
//---------------------------------------------------------------------------

using std::vector;
namespace rr
{

class CapabilitiesSection;
class RoadRunner;

class RR_DECLSPEC CapsSupport : public rrObject
{
    protected:
        string                          mName;
        string                          mDescription;
        vector<CapabilitiesSection>     mCapabilitiesSections;
        RoadRunner                     *mRoadRunner;

    public:
                                        CapsSupport(RoadRunner* rr = NULL);
        void                            Add(const CapabilitiesSection& section);
        string                          AsXMLString();
        u_int                           Count();
};

}
#endif

////using System;
////using System.Collections.Generic;
////using System.Text;
////using System.Xml;
////using LibRoadRunner.Solvers;
////using System.Runtime.Serialization;
////
////namespace LibRoadRunner.Util
////{
////    /// <summary>
////    /// Summary description for sbwInterface.
////    /// </summary>
////    public class CapsSupport
////    {
////        public static CapsSupport CurrentSettings
////        {
////            get
////            {
////                var result = new CapsSupport("RoadRunner", "Settings For RoadRunner");
////                var integration = new Section { Name = "integration", Method = "CVODE", Description = "CVODE Integrator",
////                    Capabilities = new List<Capability>( new Capability[] {
////                        new Capability("BDFOrder", CvodeInterface.MaxBDFOrder, "Maximum order for BDF Method"),
////                        new Capability("AdamsOrder", CvodeInterface.MaxAdamsOrder, "Maximum order for Adams Method"),
////                        new Capability("rtol", CvodeInterface.relTol, "Relative Tolerance"),
////                        new Capability("atol", CvodeInterface.absTol, "Absolute Tolerance"),
////                        new Capability("maxsteps", CvodeInterface.MaxNumSteps, "Maximum number of internal stepsc"),
////                        new Capability("initstep", CvodeInterface.InitStep, "the initial step size"),
////                        new Capability("minstep", CvodeInterface.MinStep, "specifies a lower bound on the magnitude of the step size."),
////                        new Capability("maxstep", CvodeInterface.MaxStep, "specifies an upper bound on the magnitude of the step size."),
////                        new Capability("conservation", RoadRunner._bComputeAndAssignConservationLaws, "enables (=1) or disables (=0) the conservation analysis of models for timecourse simulations.")
////                     })
////                };
////                result.Add(integration);
////
////                var steady = new Section
////                {
////                    Name = "SteadyState",
////                    Method = "NLEQ2",
////                    Description = "NLEQ2 Steady State Solver",
////                    Capabilities = new List<Capability>( new Capability[] {
////                        new Capability("MaxIterations", NLEQInterface.maxIterations, "Maximum number of newton iterations"),
////                        new Capability("relativeTolerance", NLEQInterface.relativeTolerance, "Relative precision of solution components"),
////                     })
////                };
////                result.Add(steady);
////
////
////                return result;
////            }
////        }
////
////        public List<Section> SectionList {get; set; }
////
////        public void Add(CapsSupport.Section section)
////        {
////            SectionList
////                .Add(section);
////        }
////
////        public bool HasSection(string sectionName)
////        {
////            return IndexOf(sectionName) >= 0;
////        }
////
////        public void GetTolerances( out double atol, out double rtol, out int maxsteps)
////        {
////            rtol = this["integration", "rtol"];
////            atol = this["integration", "atol"];
////            maxsteps = this["integration", "maxsteps"];
////        }
////
////        public void Apply()
////        {
////            if (this.HasSection("integration"))
////            {
////                CvodeInterface.MaxBDFOrder = this["integration", "BDFOrder"];
////                CvodeInterface.MaxAdamsOrder = this["integration", "AdamsOrder"];
////                CvodeInterface.relTol  = this["integration", "rtol"];
////                CvodeInterface.absTol= this["integration", "atol"];
////                CvodeInterface.MaxNumSteps = this["integration", "maxsteps"];
////                CvodeInterface.InitStep = this["integration", "initstep"];
////                CvodeInterface.MaxStep = this["integration", "maxstep"];
////                CvodeInterface.MinStep = this["integration", "minstep"];
////                RoadRunner.ComputeAndAssignConservationLaws(this["integration", "conservation"]);
////            }
////
////            if (this.HasSection("SteadyState"))
////            {
////                NLEQInterface.maxIterations = this["SteadyState", "MaxIterations"];
////                NLEQInterface._relativeTolerance = this["SteadyState", "relativeTolerance"];
////            }
////        }
////
////        public Capability this[int section, int parameter]
////        {
////            get
////            {
////                return this[section][parameter];
////            }
////            set
////            {
////                this[section][parameter] = value;
////            }
////        }
////
////        public Capability this[string section, string parameter]
////        {
////            get
////            {
////                return this[section][parameter];
////            }
////            set
////            {
////            	 this[section][parameter] =  value;
////            }
////        }
////
////        public Section this[int index]
////        {
////            get
////            {
////                return SectionList[index];
////            }
////            set
////            {
////            	SectionList[index] = value;
////            }
////        }
////
////        public Section this[string sectionName]
////        {
////            get
////            {
////                int index = IndexOf(sectionName);
////                return SectionList[index];
////            }
////            set
////            {
////                int index = IndexOf(sectionName);
////                SectionList[index] = value;
////            }
////        }
////
////        public CapsSupport(string xmlString) : this()
////        {
////            loadCapsString(xmlString);
////        }
////
////        public CapsSupport(string name, string description) : this()
////        {
////            Name = name;
////            Description = description;
////        }
////
////        public CapsSupport()
////        {
////            SectionList = new List<Section>();
////        }
////
////        public int IndexOf(string sName)
////        {
////            for (int i = 0; i < SectionList.Count; i++)
////                if (SectionList[i].Name.ToLower() == sName.ToLower())
////                    return i;
////            return -1;
////        }
////
////        public int NumCapabilities(int nSectionIndex)
////        {
////            if (nSectionIndex >= 0)
////                return SectionList[nSectionIndex].Capabilities.Count;
////            return 0;
////        }
////
////        public int NumCapabilities(string sName)
////        {
////            return NumCapabilities(IndexOf(sName));
////        }
////
////        public Capability GetCapability(string sName, int nCapsIndex)
////        {
////            return GetCapability(IndexOf(sName), nCapsIndex);
////        }
////
////        public Capability GetCapability(int nSectionIndex, int nCapsIndex)
////        {
////            if (nSectionIndex >= 0)
////            {
////                return SectionList[nSectionIndex].Capabilities[nCapsIndex];
////            }
////            return null;
////        }
////
////        public string Name { get; set; }
////        public string Description { get; set; }
////
////        public void loadCapsString(string capsStr)
////        {
////            XmlDocument xDoc = new XmlDocument();
////            xDoc.LoadXml(capsStr);
////            XmlNodeList capsNode = xDoc.GetElementsByTagName("caps");
////            for (int i = 0; i < capsNode.Count; i++)
////            {
////                XmlElement node = (XmlElement)capsNode.Item(i);
////                Name = node.GetAttribute("name");
////                Description = node.GetAttribute("description");
////
////                XmlNodeList capsList = node.GetElementsByTagName("section");
////                SectionList = new List<Section>();
////
////                foreach (XmlNode item in capsList)
////                {
////                    SectionList.Add(new Section((XmlElement)item));
////                }
////            }
////        }
////
////        public string ToXml()
////        {
////            var builder = new StringBuilder();
////            var writer = XmlWriter.Create(builder, new XmlWriterSettings { Indent = true,
////                                                                             OmitXmlDeclaration = true,
////                                                                             Encoding = Encoding.UTF8 });
////            writer.WriteStartDocument();
////            WriteTo(writer);
////            writer.WriteEndDocument();
////            writer.Flush();
////            writer.Close();
////            return builder.ToString();
////        }
////
////        public void WriteTo(XmlWriter writer)
////        {
////            writer.WriteStartElement("caps");
////            writer.WriteAttributeString("name", Name);
////            writer.WriteAttributeString("description", Description);
////            foreach (var item in SectionList)
////            {
////                item.WriteTo(writer);
////            }
////            writer.WriteEndElement();
////        }
////
////        #region Nested type: Capability
////
////        [Serializable]
////        public class Capability
////        {
////            public string Hint { get; set; }
////            public string Name { get; set; }
////            public string Type { get; set; }
////            public string Value{ get; set; }
////
////            /// <summary>
////            /// Initializes a new instance of the Capability class.
////            /// </summary>
////            public Capability(string name, string value, string hint, string type)
////            {
////                Hint = hint;
////                Name = name;
////                Type = type;
////                Value = value;
////            }
////
////
////            public Capability(string name, object value, string hint, string type)
////            {
////                Hint = hint;
////                Name = name;
////                Type = type;
////                Value = value.ToString();
////            }
////
////            protected Capability(SerializationInfo info, StreamingContext context)
////            {
////            }
////
////            public Capability(string name, object value) : this(name, value, name)
////            {
////
////            }
////
////            /// <summary>
////            /// Initializes a new instance of the Capability class.
////            /// </summary>
////            public Capability(string name, object value, string hint)
////            {
////                Hint = hint;
////                Name = name;
////                Value = value.ToString();
////                switch (value.GetType().ToString())
////                {
////                    case "System.Boolean": Type = "int";
////                        BoolValue = (bool)value;
////                        break;
////                    case "System.Int32":
////                    case "System.Int16":
////                    case "System.Int64":
////                        Type = "integer"; break;
////                    case "System.Double":
////                    case "System.Single":
////                        Type = "double"; break;
////                    default: break;
////                }
////            }
////
////            public Capability()
////            {
////            }
////
////            public Capability(XmlNode capNode)
////            {
////                XmlAttribute attr;
////                for (int k1 = 0; k1 < capNode.Attributes.Count; k1++)
////                {
////                    attr = capNode.Attributes.Item(k1) as XmlAttribute;
////                    switch (attr.Name)
////                    {
////                        case "name":
////                            Name = attr.Value;
////                            break;
////                        case "value":
////                            Value = attr.Value;
////                            break;
////                        case "hint":
////                            Hint = attr.Value;
////                            break;
////                        case "type":
////                            Type = attr.Value;
////                            break;
////                    }
////                }
////            }
////
////            public double DoubleValue
////            {
////                get
////                {
////                    double value; double.TryParse(Value, out value);
////                    return value;
////                }
////                set
////                {
////                	 Value = value.ToString();
////                }
////            }
////
////            public int IntValue
////            {
////                get
////                {
////                    int value; int.TryParse(Value, out value);
////                    return value;
////                }
////                set
////                {
////                	Value = value.ToString();
////                }
////            }
////
////            public bool BoolValue
////            {
////                get
////                {
////                    int value; int.TryParse(Value, out value);
////                    return value == 1;
////                }
////                set
////                {
////                	Value= value ? "1" : "0";
////                }
////            }
////
////            public static implicit operator int(Capability c)
////            {
////                return c.IntValue;
////            }
////
////            public static implicit operator double(Capability c)
////            {
////                return c.DoubleValue;
////            }
////
////            public static implicit operator bool (Capability c)
////                {
////                	return c.BoolValue;
////                }
////            public void WriteTo(XmlWriter writer)
////            {
////                writer.WriteStartElement("cap");
////                writer.WriteAttributeString("name", Name);
////                writer.WriteAttributeString("value", Value);
////                writer.WriteAttributeString("hint", Hint);
////                writer.WriteAttributeString("type", Type);
////                writer.WriteEndElement();
////            }
////        }
////
////        #endregion
////
////        #region Nested type: Section
////
////        public class Section
////        {
////            public Section()
////            {
////                Capabilities = new List<Capability>();
////            }
////
////            public Section(XmlElement section) : this()
////            {
////                Name = section.GetAttribute("name");
////                Method = section.GetAttribute("method");
////                Description = section.GetAttribute("description");
////
////                XmlNodeList capNodeList = section.ChildNodes;
////
////                for (int k = 0; k < capNodeList.Count; k++)
////                {
////                    Capabilities.Add(new Capability(capNodeList[k]));
////                }
////            }
////
////            public List<Capability> Capabilities { get; set; }
////            public string Description { get; set; }
////            public string Method { get; set; }
////            public string Name { get; set; }
////
////            public Capability this[int index]
////            {
////                get
////                {
////                    return Capabilities[index];
////                }
////                set
////                {
////                    Capabilities[index] = value;
////                }
////            }
////
////            public int IndexOf(string parameter)
////            {
////                for (int i = 0; i < Capabilities.Count; i++)
////                    if (Capabilities[i].Name == parameter)
////                        return i;
////                return -1;
////            }
////
////            public void WriteTo(XmlWriter writer)
////            {
////                writer.WriteStartElement("section");
////                writer.WriteAttributeString("name", Name);
////                writer.WriteAttributeString("method", Method);
////                writer.WriteAttributeString("description", Description);
////
////                foreach (var item in Capabilities)
////                {
////                    item.WriteTo(writer);
////                }
////
////                writer.WriteEndElement();
////            }
////            public bool HasCapability (string parameter)
////            {
////                    return IndexOf(parameter) >= 0;
////            }
////
////            public Capability this[string parameter]
////            {
////                get
////                {
////                    if (HasCapability(parameter))
////                        return this[IndexOf(parameter)];
////                    return new Capability();
////                }
////                set
////                {
////                    if (HasCapability(parameter))
////                	    this[IndexOf(parameter)] = value;
////                }
////            }
////        }
////
////        #endregion
////    }
////}

