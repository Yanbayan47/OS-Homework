#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>

void listProcesses() {
    std::cout << "[User] Current running processes (first 10):" << std::endl;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    int count = 0;
    if (Process32FirstW(hSnap, &pe)) {
        do {
            std::wcout << L" - " << pe.szExeFile << std::endl;
            if (++count > 10) break;
        } while (Process32NextW(hSnap, &pe));
    }
    CloseHandle(hSnap);
}

void runKiller(const std::string& args) {
    std::string command = "killer.exe " + args;
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessA(NULL, (LPSTR)command.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

int main() {
    _putenv("PROC_TO_KILL=Notepad.exe,calc.exe");
    std::cout << "[User] Environment variable PROC_TO_KILL defined." << std::endl;

    std::cout << "[User] Starting Notepad..." << std::endl;
    system("start notepad.exe");
    
    Sleep(3000);

    listProcesses();

    std::cout << "[User] Launching Killer..." << std::endl;
    runKiller("--name Notepad.exe");

    _putenv("PROC_TO_KILL=");
    std::cout << "[User] Cleanup: Environment variable removed." << std::endl;

    return 0;
}