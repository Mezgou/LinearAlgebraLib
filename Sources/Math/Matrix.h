#pragma once

#include <vector>

class Matrix {
public:
    explicit Matrix(const std::vector<std::vector<double>>& inputMatrix);
    explicit Matrix(uint32_t rowsCount, uint32_t colsCount, const std::vector<double>& values, const std::vector<uint32_t>& colsIdx, const std::vector<uint32_t>& rowPtr);
    ~Matrix() = default;

    double GetTrace() const;
    double GetElement(uint32_t row, uint32_t col) const;
    double GetDeterminant() const;

    Matrix operator*(const double scalar);
    Matrix operator+(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;

    bool IsSquare() const { return m_IsSquare; }

    inline uint32_t GetRowsCount() const { return m_RowsCount; }
    inline uint32_t GetColsCount() const { return m_ColsCount; }

    std::vector<std::vector<double>> GetStd() const;

    inline std::vector<double> GetValues() const { return m_Values; }
    inline std::vector<uint32_t> GetColsIdx() const { return m_ColsIdx; }
    inline std::vector<uint32_t> GetRowPtr() const { return m_RowPtr; }
private:
    void ConvertToCSR(const std::vector<std::vector<double>>& inputMatrix);

    double DeterminantRecursive(const std::vector<std::vector<double>>& matrix) const;
private:
    uint32_t m_RowsCount;
    uint32_t m_ColsCount;

    bool m_IsSquare;

    std::vector<double> m_Values;
    std::vector<uint32_t> m_ColsIdx;
    std::vector<uint32_t> m_RowPtr;
};