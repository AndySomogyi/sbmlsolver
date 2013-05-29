#include <iostream>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrLogger.h"
using namespace rr;

int main(int argc, char** argv)
{
	const char* rootPath = "..";


	try
    {
        LogOutput::mLogToConsole = true;
        gLog.SetCutOffLogLevel(lInfo);
		string tmpFolder = joinPath(rootPath, "temp");

        const string modelFile = joinPath(rootPath, "models", "test_1.xml");

        //Load modelFiles..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";

        RoadRunner rr1(tmpFolder);
        if(!rr1.loadSBMLFromFile(modelFile, true))
        {
			Log(lError)<<"There was a problem loading model in file: "<<modelFile;
            throw(Exception("Bad things in loadSBMLFromFile function"));
        }

    	SModelData data;
	    clog<<"Size: "<<sizeof(SModelData)<<endl;
	    clog<<"Size ptr: "<<sizeof(data.eventDelays)<<endl;
        //rr1.getModel()->cInitModelData(&data);
        Log(lInfo)<<" ---------- SIMULATE ---------------------";
        Log(lInfo)<<"Data:"<<rr1.simulate();

    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

//    Pause(true);
    return 0;
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
//#pragma comment(lib, "cprts.lib")
void ResolveUnresolvedExternals()
{
locale loc;
std::ostream st;
st.exceptions(2);
st.precision(1);
st.setf( std::ios::showbase,  std::ios::showbase);
st.eof();
st.bad();
st.operator void *();
st.operator !();

st<<use_facet <codecvt<wchar_t, char, mbstate_t> >(loc).max_length();

char_traits<wchar_t>::char_type str1[25] = L"The Hell Boy";
char_traits<wchar_t>::char_type str2[25] = L"Something To ponder";
char_traits<wchar_t>::move(str2, str1, 10);
char_traits<wchar_t>::copy(str1, str2, 2);
char_traits<wchar_t>::assign(*str1, *str2);
char_traits<wchar_t>::assign(str1, 2, *str2);
char_traits<char>::compare("test", "test", 2);
char_traits<char>::find("test", 2, '2');

//ios_base::precision(2);
double test1 = std::numeric_limits<unsigned int>::quiet_NaN();
double test2 = std::numeric_limits<int>::quiet_NaN();
double test3 = std::numeric_limits<unsigned int>::quiet_NaN();
double test4 = std::numeric_limits<int>::quiet_NaN();
double test5 = std::numeric_limits<double>::quiet_NaN();
double test6 = std::numeric_limits<float>::quiet_NaN();
double test7 = std::numeric_limits<float>::infinity();
double test8 = std::numeric_limits<double>::infinity();
double test9 = std::numeric_limits<int>::max();
}

#endif


