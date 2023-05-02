#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

void print_matrix(int** A, int N) {
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void matrix_multiply(int** A, int** B, int** C, int N, int start_row, int end_row) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    const int N = 32;  // Size of matrix

    // Initialize matrices A, B, C
    int** A = new int*[N];
    int** B = new int*[N];
    int** C = new int*[N];
    for (int i = 0; i < N; i++) {
        A[i] = new int[N];
        B[i] = new int[N];
        C[i] = new int[N];
    }

    // Initialize matrices A, B with random values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 2;
            B[i][j] = rand() % 2;
        }
    }

    // Print initial matrices
    print_matrix(A, N);
    print_matrix(B, N);

    // Perform Cannon's algorithm for matrix multiplication
    int num_threads = 16;
    int q = std::sqrt(num_threads);
    std::vector<std::thread> threads;
    for (int p = 0; p < num_threads; p++) {
        int row = p / q;
        int col = p % q;
        threads.emplace_back(matrix_multiply, A, B, C, N, row * (N / q), (row + 1) * (N / q));
    }
    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    // Print result matrix
    print_matrix(C, N);

    // Free memory
    for (int i = 0; i < N; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}

