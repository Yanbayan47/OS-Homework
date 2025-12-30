// windows_version.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <windows.h>
#include <algorithm>

using namespace std;
using namespace chrono;

vector<vector<int>> A, B, C;
int N;
int k;
CRITICAL_SECTION cs;

DWORD WINAPI multiplyBlock(LPVOID param) {
    auto* params = (pair<int, int>*)param;
    int startRow = params->first;
    int startCol = params->second;
    int endRow = min(startRow + k, N);
    int endCol = min(startCol + k, N);

    for (int i = startRow; i < endRow; ++i) {
        for (int j = startCol; j < endCol; ++j) {
            int sum = 0;
            for (int t = 0; t < N; ++t) {
                sum += A[i][t] * B[t][j];
            }
            EnterCriticalSection(&cs);
            C[i][j] = sum;
            LeaveCriticalSection(&cs);
        }
    }
    delete params;
    return 0;
}

void multithreadedMultiplyWinAPI(int blockSize) {
    k = blockSize;
    vector<HANDLE> threads;
    for (int i = 0; i < N; i += k) {
        for (int j = 0; j < N; j += k) {
            auto* params = new pair<int, int>(i, j);
            HANDLE hThread = CreateThread(NULL, 0, multiplyBlock, params, 0, NULL);
            threads.push_back(hThread);
        }
    }
    WaitForMultipleObjects((DWORD)threads.size(), threads.data(), TRUE, INFINITE);
    for (HANDLE h : threads) {
        CloseHandle(h);
    }
}

void regularMultiply() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int t = 0; t < N; ++t) {
                C[i][j] += A[i][t] * B[t][j];
            }
        }
    }
}

// Явное объявление main для консольного приложения
#ifdef _WIN32
#ifndef _CONSOLE
#define _CONSOLE
#endif
#endif

int main() {
    srand(time(nullptr));
    N = 100; // Размер матрицы
    A.resize(N, vector<int>(N));
    B.resize(N, vector<int>(N));
    C.resize(N, vector<int>(N, 0));
    InitializeCriticalSection(&cs);

    // Заполнение матриц случайными числами
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // Обычное умножение (однопоточное)
    auto start = high_resolution_clock::now();
    regularMultiply();
    auto stop = high_resolution_clock::now();
    auto durationRegular = duration_cast<milliseconds>(stop - start);
    cout << "Regular multiplication time: " << durationRegular.count() << " ms" << endl;

    // Многопоточное умножение для разных размеров блоков
    cout << "\nBlock size k | Time (ms)" << endl;
    cout << "-------------------------" << endl;
    for (int blockSize = 1; blockSize <= N; blockSize *= 2) {
        // Обнуляем результирующую матрицу
        for (auto& row : C) {
            fill(row.begin(), row.end(), 0);
        }

        auto startThread = high_resolution_clock::now();
        multithreadedMultiplyWinAPI(blockSize);
        auto stopThread = high_resolution_clock::now();
        auto durationThread = duration_cast<milliseconds>(stopThread - startThread);
        cout << blockSize << "           | " << durationThread.count() << endl;
    }

    DeleteCriticalSection(&cs);
    
    // Пауза, чтобы увидеть результаты
    cout << "\nPress Enter to exit...";
    cin.get();
    
    return 0;
}