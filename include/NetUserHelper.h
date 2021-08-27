#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <windows.h> 
#include <lm.h>

class NetUserHelper
{
public:
	BOOL addAdministrator(LPWSTR username, LPWSTR password);
	BOOL addAdministrator();
	BOOL addUser(LPWSTR username, LPWSTR password);
	BOOL makeUserAdmin(IN PCWSTR pszUserName);
	BOOL addUserToGroup(LPWSTR strUserName, LPWSTR strGroupName);
	static LPWSTR ConvertCharToLPWSTR(const char* szString, int len);

private:
	static NetUserHelper* instance;

private:
	NetUserHelper() {};
	~NetUserHelper() {};
	NetUserHelper(const NetUserHelper&);
	NetUserHelper& operator=(const NetUserHelper&);
private:
	class Deletor {
	public:
		~Deletor();
	};
	static Deletor deletor;
public:
	static NetUserHelper* getInstance();
};
