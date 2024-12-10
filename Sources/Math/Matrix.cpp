#include "Matrix.h"

#include <iostream>

Matrix::Matrix(const std::vector<std::vector<double>>& inputMatrix)
    : m_RowsCount(inputMatrix.size()), m_ColsCount(inputMatrix[0].size()), m_IsSquare(m_RowsCount == m_ColsCount) {
    ConvertToCSR(inputMatrix);
}

Matrix::Matrix(uint32_t rowsCount, uint32_t colsCount, const std::vector<double>& values,
               const std::vector<uint32_t>& colsIdx, const std::vector<uint32_t>& rowPtr)
               : m_RowsCount(rowsCount), m_ColsCount(colsCount), m_IsSquare(rowsCount == colsCount),
               m_Values(values), m_ColsIdx(colsIdx), m_RowPtr(rowPtr) {}

void Matrix::ConvertToCSR(const std::vector<std::vector<double>>& inputMatrix) {
    m_RowPtr.reserve(m_RowsCount + 1);
    m_RowPtr.push_back(0);

    for (uint32_t cRow = 0; cRow < m_RowsCount; cRow++) {
        for (uint32_t cCol = 0; cCol < m_ColsCount; cCol++) {
            if (inputMatrix[cRow][cCol] != 0) {
                m_Values.push_back(inputMatrix[cRow][cCol]);
                m_ColsIdx.push_back(cCol);
            }
        }
        m_RowPtr.push_back(m_Values.size());
    }
}

double Matrix::GetTrace() const {
    double trace = 0.0;
    if (!m_IsSquare) {
        std::cout << "[LOG] [WARNING] It is not a square matrix! ";
        return trace;
    }

    for (uint32_t cRow = 0; cRow < m_RowsCount; cRow++) {
        const uint32_t startIdx = m_RowPtr[cRow];
        const uint32_t endIdx = m_RowPtr[cRow + 1];
        for (uint32_t cIdx = startIdx; cIdx < endIdx; cIdx++) {
            if (m_ColsIdx[cIdx] == cRow) {
                trace += m_Values[cIdx];
                break;
            }
        }
    }

    return trace;
}

double Matrix::GetElement(uint32_t row, uint32_t col) const {
    row--;
    col--;
    const uint32_t startIdx = m_RowPtr[row];
    const uint32_t endIdx = m_RowPtr[row + 1];
    for (uint32_t cIdx = startIdx; cIdx < endIdx; cIdx++) {
        if (m_ColsIdx[cIdx] == col) {
            return m_Values[cIdx];
        }
    }

    return 0.0;
}

Matrix Matrix::operator*(const double scalar) {
    for (uint32_t cIdx = 0; cIdx < m_Values.size(); cIdx++) {
        m_Values[cIdx] *= scalar;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (m_ColsCount != other.GetColsCount() || m_RowsCount != other.GetRowsCount()) {
        std::cout << "[LOG] [ERROR] Matrices cannot be added: inconsistent sizes!\n";
        return Matrix(std::vector<std::vector<double>>{{0.0}});
    }

    std::vector<double> resultValues;
    std::vector<uint32_t> resultColsIdx;
    std::vector<uint32_t> resultRowPtr(m_RowsCount + 1, 0.0);

    for (uint32_t cRow = 0; cRow < m_RowsCount; cRow++) {
        uint32_t startIdx = m_RowPtr[cRow];
        uint32_t endIdx = m_RowPtr[cRow + 1];
        uint32_t otherStartIdx = other.GetRowPtr()[cRow];
        uint32_t otherEndIdx = other.GetRowPtr()[cRow + 1];

        uint32_t currentIdx = startIdx;
        uint32_t otherCurrentIdx = otherStartIdx;

        while (currentIdx < endIdx || otherCurrentIdx < otherEndIdx) {
            if (currentIdx < endIdx && (otherCurrentIdx >= otherEndIdx || m_ColsIdx[currentIdx] < other.GetColsIdx()[otherCurrentIdx])) {
                resultValues.push_back(m_Values[currentIdx]);
                resultColsIdx.push_back(m_ColsIdx[currentIdx]);
                currentIdx++;
            } else if (otherCurrentIdx < otherEndIdx && (currentIdx >= endIdx || m_ColsIdx[currentIdx] > other.GetColsIdx()[otherCurrentIdx])) {
                resultValues.push_back(other.GetValues()[otherCurrentIdx]);
                resultColsIdx.push_back(other.GetColsIdx()[otherCurrentIdx]);
                otherCurrentIdx++;
            } else {
                resultValues.push_back(m_Values[currentIdx] + other.GetValues()[otherCurrentIdx]);
                resultColsIdx.push_back(m_ColsIdx[currentIdx]);
                currentIdx++;
                otherCurrentIdx++;
            }
        }
        resultRowPtr[cRow + 1] = resultValues.size();
    }

    return Matrix(m_RowsCount, m_ColsCount, resultValues, resultColsIdx, resultRowPtr);
}


Matrix Matrix::operator*(const Matrix& other) const {
    std::vector<std::vector<double>> currentMatrix = this->GetStd();
    const std::vector<std::vector<double>> otherMatrix = other.GetStd();
    if (currentMatrix[0].size() != otherMatrix.size()) {
        std::cout << "[LOG] [ERROR] Matrices cannot be multiplied: inconsistent sizes! ";
        return Matrix(std::vector<std::vector<double>>{{0}});
    }
    const uint32_t rows = currentMatrix.size();
    const uint32_t cols = otherMatrix[0].size();
    const uint32_t commonDim = currentMatrix[0].size();
    std::vector<std::vector<double>> resultMatrix(rows, std::vector<double>(cols, 0.0));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            for (size_t k = 0; k < commonDim; ++k) {
                resultMatrix[i][j] += currentMatrix[i][k] * otherMatrix[k][j];
            }
        }
    }
    return Matrix(resultMatrix);
}

std::vector<std::vector<double>> Matrix::GetStd() const {
    std::vector<std::vector<double>> outputMatrix(m_RowsCount, std::vector<double>(m_ColsCount, 0.0));
    for (uint32_t cRow = 0; cRow < m_RowsCount; cRow++) {
        const uint32_t startIdx = m_RowPtr[cRow];
        const uint32_t endIdx = m_RowPtr[cRow + 1];
        for (uint32_t cIdx = startIdx; cIdx < endIdx; cIdx++) {
            outputMatrix[cRow][m_ColsIdx[cIdx]] = m_Values[cIdx];
        }
    }

    return outputMatrix;
}

double Matrix::GetDeterminant() const {
    if (!m_IsSquare) {
        std::cout << "[LOG] [WARNING] It is not a square matrix!\n";
    }
    return DeterminantRecursive(GetStd());
}

double Matrix::DeterminantRecursive(const std::vector<std::vector<double>>& matrix) const {
    uint32_t n = matrix.size();
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    double det = 0.0;
    for (uint32_t cCol = 0; cCol < n; cCol++) {
        std::vector<std::vector<double>> minorMatrix;
        for (uint32_t i = 1; i < n; i++) {
            std::vector<double> row;
            for (uint32_t j = 0; j < n; j++) {
                if (j != cCol) {
                    row.push_back(matrix[i][j]);
                }
            }
            minorMatrix.push_back(row);
        }
        det += (cCol % 2 == 0 ? 1 : -1) * matrix[0][cCol] * DeterminantRecursive(minorMatrix);
    }
    return det;
}
