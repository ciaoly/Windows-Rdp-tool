#pragma once
#include <windows.h>
#include "OpenRdp.h"
#include <netfw.h>
#include <stdio.h>

#pragma comment( lib, "ole32.lib" )

class FireWallHelper {
public:
	BOOL fireWallSwitcher(BOOL on);


private:
	static FireWallHelper* instance;
	OpenRdp::RegHelper regHelper;
	//HRESULT hr;
	//HRESULT comInit;
	//INetFwProfile* fwProfile;
	//HRESULT hrComInit;
	INetFwPolicy2* pNetFwPolicy2;

	// Instantiate INetFwPolicy2
	static HRESULT WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2);

private:
	FireWallHelper();
	~FireWallHelper();
	FireWallHelper(const FireWallHelper&);
	FireWallHelper& operator=(const FireWallHelper&);
private:
	class Deletor {
	public:
		~Deletor();
	};
	static Deletor deletor;
public:
	static FireWallHelper* getInstance();
};

