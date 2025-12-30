#include <iostream>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

vector<vector<int>> A, B, C;
int N, k;
pthread_mutex_t mutex;

struct BlockParams {
    int startRow, endRow, startCol, endCol;
};

void* multiplyBlock(void* arg) {
    auto* params = (BlockParams*)arg;
    for (int i = params->startRow; i < params->endRow; ++i) {
        for (int j = params->startCol; j < params->endCol; ++j) {
            int sum = 0;
            for (int t = 0; t < N; ++t) {
                sum += A[i][t] * B[t][j];
            }
            pthread_mutex_lock(&mutex);
            C[i][j] = sum;
            pthread_mutex_unlock(&mutex);
        }
    }
    delete params;
    return nullptr;
}

void multithreadedMultiplyPthread(int blockSize) {
    k = blockSize;
    vector<pthread_t> threads;
    for (int i = 0; i < N; i += k) {
        for (int j = 0; j < N; j += k) {
            auto* params = new BlockParams{i, min(i + k, N), j, min(j + k, N)};
            pthread_t thread;
            pthread_create(&thread, nullptr, multiplyBlock, params);
            threads.push_back(thread);
        }
    }
    for (pthread_t thread : threads) {
        pthread_join(thread, nullptr);
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

int main() {
    srand(time(nullptr));
    N = 100; // Размер матрицы
    A.resize(N, vector<int>(N));
    B.resize(N, vector<int>(N));
    C.resize(N, vector<int>(N, 0));
    
    pthread_mutex_init(&mutex, nullptr);

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
        multithreadedMultiplyPthread(blockSize);
        auto stopThread = high_resolution_clock::now();
        auto durationThread = duration_cast<milliseconds>(stopThread - startThread);
        cout << blockSize << "           | " << durationThread.count() << endl;
    }

    pthread_mutex_destroy(&mutex);
    
    return 0;
}