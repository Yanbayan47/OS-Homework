#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

bool StartProcess(const std::string& appName, int hStdIn, int hStdOut, pid_t& pid) {
    pid = fork();
    
    if (pid == 0) { 
        if (hStdIn != STDIN_FILENO) {
            dup2(hStdIn, STDIN_FILENO);
            close(hStdIn);
        }
        if (hStdOut != STDOUT_FILENO) {
            dup2(hStdOut, STDOUT_FILENO);
            close(hStdOut);
        }
        
        // Запуск
        execl(appName.c_str(), appName.c_str(), (char*)NULL);
        
        // Если execl завершился неудачей
        std::cerr << "Error: Could not launch " << appName << std::endl;
        exit(1);
    } 
    else if (pid < 0) { // Ошибка fork
        std::cerr << "Error: Fork failed" << std::endl;
        return false;
    }
    
    return true;
}

int main() {
    int pipeM[2], pipeA[2], pipeP[2], pipeS[2];
    pid_t pidM, pidA, pidP, pidS;
    
    // Создаем пайпы
    if (pipe(pipeM) == -1 || pipe(pipeA) == -1 || 
        pipe(pipeP) == -1 || pipe(pipeS) == -1) {
        std::cerr << "Error: Pipe creation failed" << std::endl;
        return 1;
    }
    
    // Запуск в цепочке
    
    if (!StartProcess("./processM", pipeM[0], pipeA[1], pidM)) return 1;
    close(pipeM[0]);
    close(pipeA[1]);
    
    if (!StartProcess("./processA", pipeA[0], pipeP[1], pidA)) return 1;
    close(pipeA[0]);
    close(pipeP[1]);
    
    if (!StartProcess("./processP", pipeP[0], pipeS[1], pidP)) return 1;
    close(pipeP[0]);
    close(pipeS[1]);
    
    if (!StartProcess("./processS", pipeS[0], STDOUT_FILENO, pidS)) return 1;
    close(pipeS[0]);
    
    //  данные в первый процесс
    std::string data = "1 2 3 4 5";
    std::cout << "Data sent: " << data << std::endl;
    
    write(pipeM[1], data.c_str(), data.length());
    close(pipeM[1]);
    
    std::cout << "Waiting for result..." << std::endl;
    
    // ждуним
    waitpid(pidS, nullptr, 0);
    waitpid(pidP, nullptr, 0);
    waitpid(pidA, nullptr, 0);
    waitpid(pidM, nullptr, 0);
    
    std::cout << "\nDone." << std::endl;
    return 0;
}