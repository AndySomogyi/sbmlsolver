#ifndef rrSBMLSymbolH
#define rrSBMLSymbolH
#include <ostream>
#include <vector>
#include <deque>
#include "rrExporter.h"
#include "rrUtils.h"
#include "rrSBMLSymbolDependencies.h"
using std::vector;
using std::deque;
using std::ostream;
namespace rr
{

class RR_DECLSPEC SBMLSymbol
{
    protected:
    public:
        std::string                          mId;
        enum SBMLType                   mType;
        SBMLSymbolDependencies          mDependencies;

        double                          mValue;
        bool                            HasValue();

        double&                         mConcentration; //Assing ref to mValue..
        double&                         mAmount; //Assing ref to mValue..

        bool                            IsSetAmount;
        bool                            IsSetConcentration;

        bool                            HasInitialAssignment() const;
        std::string                          mInitialAssignment;

        bool                            mHasRule;
        bool                            HasRule();
        std::string                          mRule;

    public:
                                        SBMLSymbol();
                                       ~SBMLSymbol();
                                        SBMLSymbol(const SBMLSymbol& cp);
                                        SBMLSymbol& operator =(const SBMLSymbol& rhs);
        void                            AddDependency(SBMLSymbol* symbol);
        int                             NumberOfDependencies();
        SBMLSymbol                      GetDependency(const int& i);

};

RR_DECLSPEC std::ostream& operator<<(std::ostream& stream, const SBMLSymbol& symbol);
}
#endif



////namespace SBMLSupport
////{
////    public class SBMLSymbol
////    {
////        private std::string _Id;
////        public std::string Id
////        {
////            get { return _Id; }
////            set { _Id = value; }
////        }
////
////        private SBMLType _Type;
////        public SBMLType Type
////        {
////            get { return _Type; }
////            set { _Type = value; }
////        }
////
////        private List<SBMLSymbol> _Dependencies = new List<SBMLSymbol>();
////        public List<SBMLSymbol> Dependencies
////        {
////            get { return _Dependencies; }
////            set { _Dependencies = value; }
////        }
////
////        public bool HasValue
////        {
////            get { return !Double.IsNaN(_Value); }
////        }
////
////        private double _Value = Double.NaN;
////        public double Value
////        {
////            get { return _Value; }
////            set { _Value = value; }
////        }
////
////
////        public double Concentration
////        {
////            set
////            {
////                _Value = value;
////                _IsSetConcentration = true;
////            }
////        }
////
////        public double Amount
////        {
////            set
////            {
////                _Value = value;
////                _IsSetAmount = true;
////            }
////        }
////
////        private bool _IsSetAmount = false;
////        public bool IsSetAmount
////        {
////            get { return _IsSetAmount; }
////            set { _IsSetAmount = value; }
////        }
////
////        private bool _IsSetConcentration = false;
////        public bool IsSetConcentration
////        {
////            get { return _IsSetConcentration; }
////            set { _IsSetConcentration = value; }
////        }
////
////        public bool HasInitialAssignment
////        {
////            get { return !std::string.IsNullOrEmpty(_InitialAssignment); }
////        }
////
////        private std::string _InitialAssignment;
////        public std::string InitialAssignment
////        {
////            get { return _InitialAssignment; }
////            set { _InitialAssignment = value; }
////        }
////
////
////        public bool HasRule
////        {
////            get { return !std::string.IsNullOrEmpty(_Rule); }
////        }
////
////        private std::string _Rule;
////        public std::string Rule
////        {
////            get { return _Rule; }
////            set { _Rule = value; }
////        }
////
////    }
////}

