%module SettingTestsSwigAPI

// handles type maps for C -> Python and Python -> C with the Setting object
%include "rr_setting.i"

// This is the C++ code that we want to wrap.
// Its purpose is to isolate the Setting functionality
// for testing
%include "SettingTestsSwigAPI.h"

















