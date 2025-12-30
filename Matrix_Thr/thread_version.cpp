#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>

using namespace std;
using namespace chrono;

mutex mtx;

void multiplyBlock(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, 
                   int startRow, int endRow, int startCol, int endCol, int N) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = startCol; j < endCol; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            lock_guard<mutex> lock(mtx);
            C[i][j] = sum;
        }
    }
}

void multithreadedMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int N, int k) {
    vector<thread> threads;
    for (int i = 0; i < N; i += k) {
        for (int j = 0; j < N; j += k) {
            int endRow = min(i + k, N);
            int endCol = min(j + k, N);
            threads.emplace_back(multiplyBlock, cref(A), cref(B), ref(C), i, endRow, j, endCol, N);
        }
    }
    for (auto& t : threads) {
        t.join();
    }
}

void regularMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(nullptr));
    int N = 100; // можно менять, минимум 5
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));

    // Заполняем случайными числами
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // Обычное умножение
    auto start = high_resolution_clock::now();
    regularMultiply(A, B, C, N);
    auto stop = high_resolution_clock::now();
    auto durationRegular = duration_cast<milliseconds>(stop - start);
    cout << "Regular multiplication time: " << durationRegular.count() << " ms" << endl;

    // Многопоточное умножение для разных k
    cout << "\nBlock size k | Time (ms)" << endl;
    cout << "-------------------------" << endl;
    for (int k = 1; k <= N; k *= 2) {
        // Обнуляем C
        for (auto& row : C) {
            fill(row.begin(), row.end(), 0);
        }

        auto startThread = high_resolution_clock::now();
        multithreadedMultiply(A, B, C, N, k);
        auto stopThread = high_resolution_clock::now();
        auto durationThread = duration_cast<milliseconds>(stopThread - startThread);
        cout << k << "           | " << durationThread.count() << endl;
    }

    return 0;
}