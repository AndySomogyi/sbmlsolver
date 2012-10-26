
#if defined(EXPORT_POCO)
#define POCO_API_TEST __declspec(dllexport)
#else
#define POCO_API_TEST __declspec(dllimport)
#endif

extern "C" void POCO_API_TEST __stdcall poco_hello();

