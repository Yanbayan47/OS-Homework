#include <windows.h>
#include <iostream>
#include <string>

bool StartProcess(std::string appName, HANDLE hStdIn, HANDLE hStdOut, HANDLE& hProcess) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hStdIn;
    si.hStdOutput = hStdOut;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    char cmd[256];
    strncpy(cmd, appName.c_str(), sizeof(cmd));

    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Error: Could not launch " << appName << ". Code: " << GetLastError() << std::endl;
        return false;
    }

    hProcess = pi.hProcess;
    CloseHandle(pi.hThread);
    return true;
}

int main() {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hM_In_R, hM_In_W; // Main  M
    HANDLE hA_In_R, hA_In_W; // M  A
    HANDLE hP_In_R, hP_In_W; //  A  P
    HANDLE hS_In_R, hS_In_W; //  P  S

    HANDLE hProcM, hProcA, hProcP, hProcS;


    CreatePipe(&hM_In_R, &hM_In_W, &sa, 0);
    SetHandleInformation(hM_In_W, HANDLE_FLAG_INHERIT, 0);

    CreatePipe(&hA_In_R, &hA_In_W, &sa, 0);
    
    if (!StartProcess("processM.exe", hM_In_R, hA_In_W, hProcM)) return 1;

    CloseHandle(hM_In_R); 
    CloseHandle(hA_In_W);

    CreatePipe(&hP_In_R, &hP_In_W, &sa, 0);

    if (!StartProcess("processA.exe", hA_In_R, hP_In_W, hProcA)) return 1;

    // дескрипторы
    CloseHandle(hA_In_R);
    CloseHandle(hP_In_W);


    CreatePipe(&hS_In_R, &hS_In_W, &sa, 0);
    if (!StartProcess("processP.exe", hP_In_R, hS_In_W, hProcP)) return 1;

    CloseHandle(hP_In_R);
    CloseHandle(hS_In_W);

    if (!StartProcess("processS.exe", hS_In_R, GetStdHandle(STD_OUTPUT_HANDLE), hProcS)) return 1;

    CloseHandle(hS_In_R);

    std::string data = "1 2 3 4 5";
    std::cout << "Data sent: " << data << std::endl;
    
    DWORD written;
    WriteFile(hM_In_W, data.c_str(), data.length(), &written, NULL);
    
    CloseHandle(hM_In_W);

    std::cout << "Waiting for result..." << std::endl;

    WaitForSingleObject(hProcS, INFINITE);

    CloseHandle(hProcM);
    CloseHandle(hProcA);
    CloseHandle(hProcP);
    CloseHandle(hProcS);

    std::cout << "\nDone." << std::endl;
    return 0;
}