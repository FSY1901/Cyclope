#ifdef CYCLOPE_BUILD_DLL
	#define	API __declspec(dllexport)
#else
	#define	API __declspec(dllimport)
#endif // CYCLOPE_BUILD_DLL