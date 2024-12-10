#pragma once

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <vector>
#include <iostream>
#include <sstream>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"

#define LOG(title, x) std::cout << title << ": " << x << "\n"

#ifdef _WIN32
#include <windows.h>

static inline void EnableAnsiSupport() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

static void PrintTestResult(const CU_pSuite& suite) {
    const uint32_t failures = CU_get_number_of_failures();
    const uint32_t tests = CU_get_number_of_tests_run();
    const uint32_t passed = tests - failures;

    std::cout << CYAN << "----------------------------------------" << RESET << std::endl;
    std::cout << CYAN << "Recruitment Tests: " << suite->pName << RESET << std::endl;
    std::cout << "Total tests: " << tests << std::endl;
    std::cout << GREEN << "Successful: " << passed << RESET << std::endl;
    if (failures > 0) {
        std::cout << RED << "Failed: " << failures << RESET << std::endl;
    } else {
        std::cout << GREEN << "All tests have been successfully passed!" << RESET << std::endl;
    }
    std::cout << CYAN << "----------------------------------------" << RESET << std::endl;
}

template<typename T>
static std::vector<std::vector<T>> GetUserMatrix() {
    uint32_t n, m;
    std::cout << "Enter the dimensions of the matrix (N and M): ";
    std::cin >> n >> m;
    std::vector<std::vector<T>> inputMatrix(n, std::vector<T>(m, 0.0));
    std::cout << "Enter the elements of the matrix (N rows of M elements): ";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            std::cin >> inputMatrix[i][j];
        }
    }
    return inputMatrix;
}

template<typename T>
void PrintMatrix(const std::vector<std::vector<T>>& matrix, const char* title) {
    uint32_t width = 0;
    for (const std::vector<T>& row : matrix) {
        for (T elem : row) {
            uint32_t len = snprintf(nullptr, 0, "%.2f", elem);
            if (len > width) {
                width = len;
            }
        }
    }

    for (const std::vector<T>& row : matrix) {
        std::cout << "| ";
        for (T elem : row) {
            printf("%-*.*f ", width, 2, elem);
        }
        std::cout << "|" << std::endl;
    }
}