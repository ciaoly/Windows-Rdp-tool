#pragma once
#include <windows.h>

class OpenRdp {
public:
	class RegHelper {
	public:
		static BOOL SetStringValue(HKEY Hroot, const char* SzSubKey, const char* SzValueName, const char* SzValue);

		static BOOL SetDwordValue(HKEY Hroot, const char* SzSubKey, const char* SzValueName, DWORD SzValue);
	};

	static OpenRdp* getInstance();
	
	BOOL permitRootLogin(BOOL yes);
	BOOL changePort(DWORD port);
	BOOL openRdp(DWORD port, BOOL permitRootLogin);
	BOOL openRdp();
private:
	static RegHelper regHelper;
	static OpenRdp* instance;
private:
	OpenRdp() {};
	~OpenRdp() {};
	OpenRdp(const OpenRdp&);
	OpenRdp& operator=(const OpenRdp&);
private:
	class Deletor {
	public:
		~Deletor();
	};
	static Deletor deletor;
};

