#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <tlhelp32.h>

std::wstring s2ws(const std::string& str) {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void killProcessById(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
    if (hProcess != NULL) {
        if (TerminateProcess(hProcess, 0)) {
            std::cout << "[Killer] Killed process ID: " << processId << std::endl;
        } else {
            std::cerr << "[Killer] Failed to terminate ID: " << processId << ". Error: " << GetLastError() << std::endl;
        }
        CloseHandle(hProcess);
    }
}

void killProcessByName(const std::wstring& processName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    bool found = false;
    if (Process32FirstW(hSnap, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                std::wcout << L"[Killer] Found process: " << pe.szExeFile << L" (ID: " << pe.th32ProcessID << L")" << std::endl;
                killProcessById(pe.th32ProcessID);
                found = true;
            }
        } while (Process32NextW(hSnap, &pe));
    }
    if (!found) {
        std::wcout << L"[Killer] No process found with name: " << processName << std::endl;
    }
    CloseHandle(hSnap);
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--id" && i + 1 < argc) {
            killProcessById(std::stoul(argv[++i]));
        } else if (arg == "--name" && i + 1 < argc) {
            killProcessByName(s2ws(argv[++i]));
        }
    }

    const char* env_p = std::getenv("PROC_TO_KILL");
    if (env_p != nullptr) {
        std::cout << "[Killer] Found PROC_TO_KILL variable: " << env_p << std::endl;
        std::stringstream ss(env_p);
        std::string item;
        while (std::getline(ss, item, ',')) {
            // Очистка пробелов
            item.erase(0, item.find_first_not_of(" "));
            item.erase(item.find_last_not_of(" ") + 1);
            if (!item.empty()) killProcessByName(s2ws(item));
        }
    } else {
        std::cout << "[Killer] PROC_TO_KILL variable is empty or not set." << std::endl;
    }
    return 0;
}