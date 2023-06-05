#ifdef CYCLOPE_BUILD_DLL
	#define	CYCLOPE_API __declspec(dllexport)
#else
	#define	CYCLOPE_API __declspec(dllimport)
#endif // CYCLOPE_BUILD_DLL