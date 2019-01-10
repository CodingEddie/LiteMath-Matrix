#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace LiteMath
{
	template<typename T>
	class Matrix
	{
	public:
		Matrix(const size_t rows, const size_t cols);
		Matrix(const size_t rows, const size_t cols, const std::vector<T>&data);
		Matrix(const size_t rows, const size_t cols, std::vector<T>&&data);
		~Matrix() = default;

		Matrix(const Matrix &rhs) = default; // copy constructor
		Matrix(Matrix && rhs) = default; // move constructor

		Matrix& operator=(const Matrix& rhs) = default; // copy assignment
		Matrix& operator=(Matrix&& rhs) = default; // move assignment

		Matrix& zeroes();
		Matrix& ones();

		Matrix operator+(const Matrix &rhs);
		Matrix& operator+=(const Matrix& rhs);
		Matrix operator-(const Matrix &rhs);
		Matrix& operator-=(const Matrix& rhs);
		Matrix operator*(const Matrix &rhs);
		Matrix& operator*=(const Matrix& rhs);

		T& operator()(const size_t rIndex, const size_t cIndex);

		Matrix operator*(const T &rhs);
		Matrix& operator*=(const T& rhs);

		bool operator!=(const Matrix& rhs) const;
		bool operator==(const Matrix& rhs) const;

		//std::ostream & operator<<(std::ostream & os, const Matrix<T> & rhs);

		template<typename T>
		friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& rhs);

		size_t rowCount() const;
		size_t colCount() const;

		bool isAddSubLegal(const Matrix& rhs) const;
		bool isMultLegal(const Matrix& rhs) const;

		void setMatrix(const std::vector<T>& val);
		void setMatrix(const std::vector<T>&& val);

		void setRow(const size_t row, const std::vector<T>& val);
		void setRow(const size_t row, const std::vector<T>&& val);
		void setCol(const size_t col, const std::vector<T>& val);
		void setCol(const size_t col, const std::vector<T>&& val);

		Matrix& transpose();

	private:
		size_t m_rows;
		size_t m_cols;

		std::vector<T> m_mat;
	};


	//#include "Matrix.tcc"

	template<typename T>
	Matrix<T> ::Matrix(const size_t rows, const size_t cols)
		: m_rows(rows), m_cols(cols), m_mat(rows * cols)
	{
	}

	template<typename T>
	Matrix<T> ::Matrix(const size_t rows, const size_t cols, const std::vector<T>& data)
		: m_rows(rows), m_cols(cols), m_mat(data)
	{
		if (m_rows * cols != m_mat.size())
			std::exception("Matrix and vector size do not match");
	}

	template<typename T>
	Matrix<T> ::Matrix(const size_t rows, const size_t cols, std::vector<T>&&data)
		: m_rows(rows), m_cols(cols), m_mat(data)
	{
		if (m_rows * cols != m_mat.size())
			std::exception("Matrix and vector size do not match");
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::zeroes()
	{
		std::fill(m_mat.begin(), m_mat.end(), 0);

		return *this;
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::ones()
	{
		std::fill(m_mat.begin(), m_mat.end(), 1);

		return *this;
	}

	template<typename T>
	Matrix<T>  Matrix<T> ::operator+(const Matrix& rhs)
	{
		return Matrix(*this) += rhs;
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::operator+=(const Matrix& rhs)
	{
		if (!isAddSubLegal(rhs))
			throw std::range_error("Matrix sizes are not compatible\n");

		std::transform(m_mat.begin(), m_mat.end(), rhs.m_mat.begin(),
			m_mat.begin(), std::plus<>());

		return *this;
	}

	template<typename T>
	Matrix<T>  Matrix<T> ::operator-(const Matrix& rhs)
	{
		return Matrix(*this) -= rhs;
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::operator-=(const Matrix& rhs)
	{
		if (!isAddSubLegal(rhs))
			throw std::range_error("Matrix sizes are not compatible\n");

		std::transform(m_mat.begin(), m_mat.end(), rhs.m_mat.begin(),
			m_mat.begin(), std::minus<>());

		return *this;
	}

	template<typename T>
	Matrix<T>  Matrix<T> ::operator*(const Matrix& rhs)
	{
		return Matrix(*this) *= rhs;
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::operator*=(const Matrix& rhs)
	{
		if (!isMultLegal(rhs))
			throw std::range_error("Matrix index are not compatible\n");

		Matrix<T>  temp(m_rows, rhs.m_cols);

		for (size_t i = 0; i < m_rows; i++)
		{
			for (size_t j = 0; j < m_cols; ++j)
			{
				for (size_t k = 0; k < m_cols; ++k)
				{
					temp.m_mat[i * rhs.m_cols + j] += m_mat[i * m_cols + k] * rhs.m_mat[j + k * m_cols];
				}
			}
		}

		*this = std::move(temp);

		return *this;
	}

	template<typename T>
	Matrix<T>  Matrix<T> ::operator*(const T& rhs)
	{
		return Matrix(*this) *= rhs;
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::operator*=(const T& rhs)
	{
		std::transform(m_mat.begin(), m_mat.end(), m_mat.begin(),
			std::bind(std::multiplies<T>(), std::placeholders::_1, rhs));

		return *this;
	}

	template<typename T>
	T& Matrix<T> ::operator()(const size_t rIndex, const size_t cIndex)
	{
		return m_mat.at(rIndex * m_cols + cIndex);
	}

	template<typename T>
	bool Matrix<T> ::operator!=(const Matrix& rhs) const
	{
		bool isNotEqual = false;

		for (size_t i = 0; i < m_rows; i++)
		{
			for (size_t j = 0; j < m_cols; ++j)
			{
				isNotEqual = std::numeric_limits<T>::epsilon() <=
					std::abs(m_mat[i * m_cols + j] - rhs.m_mat[i * m_cols + j]);
				if (isNotEqual)
					break;
			}
		}
		return isNotEqual;
	}

	template<typename T>
	bool Matrix<T> ::operator==(const Matrix& rhs) const
	{
		return !(*this != rhs);
	}


	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Matrix<T>& rhs)
	{
		for (size_t i = 0; i < rhs.m_rows; ++i)
		{
			for (size_t j = 0; j < rhs.m_cols; ++j)
				os << std::setw(6) << std::fixed << std::setprecision(2) << rhs.m_mat[i * rhs.m_cols + j] << ' ';

			os << '\n';
		}
		return os;
	}

	template<typename T>
	Matrix<T>  operator*(const T &lhs, Matrix<T> & rhs)
	{
		return rhs * lhs;
	}

	template<typename T>
	size_t Matrix<T> ::rowCount() const
	{
		return m_rows;
	}

	template<typename T>
	size_t Matrix<T> ::colCount() const
	{
		return m_cols;
	}

	template<typename T>
	bool Matrix<T> ::isAddSubLegal(const Matrix& rhs) const
	{
		return ((m_rows == rhs.m_rows) && (m_cols == rhs.m_cols) &&
			m_cols > 0 && rhs.m_rows > 0 && rhs.m_cols > 0);
	}

	template<typename T>
	bool Matrix<T> ::isMultLegal(const Matrix& rhs) const
	{
		return (m_cols == rhs.m_rows && m_rows > 0 &&
			m_cols > 0 && rhs.m_rows > 0 && rhs.m_cols > 0);
	}

	template<typename T>
	void Matrix<T> ::setMatrix(const std::vector<T>& val)
	{
		std::copy(val.begin(), val.end(), m_mat.begin());
		return;
	}

	template<typename T>
	void Matrix<T> ::setMatrix(const std::vector<T>&& val)
	{
		std::copy(val.begin(), val.end(), m_mat.begin());
		return;
	}

	template<typename T>
	void Matrix<T> ::setRow(const size_t row, const std::vector<T> &val)
	{
		if (row >= m_rows)
			throw std::range_error("Matrix index is out of range\n");
		if (val.size() > m_cols)
		{
			throw std::range_error("Data size is too large\n");
		}

		std::copy(val.begin(), val.end(), m_mat.begin() + row * m_cols);
	}

	template<typename T>
	void Matrix<T> ::setRow(const size_t row, const std::vector<T>&& val)
	{
		if (row >= m_rows)
			throw std::range_error("Matrix index is out of range\n");
		if (val.size() > m_cols)
		{
			throw std::range_error("Data size is too large\n");
		}

		std::copy(val.begin(), val.end(), m_mat.begin() + row * m_cols);
	}

	template<typename T>
	void Matrix<T> ::setCol(const size_t col, const std::vector<T>& val)
	{
		if (col >= m_cols)
			throw std::range_error("Matrix index is out of range\n");
		if (val.size() > m_rows)
		{
			throw std::range_error("Data size is too large\n");
		}

		for (size_t i = 0; i != val.size(); i++)
		{
			m_mat[col + i * m_rows] = val[i];
		}
	}

	template<typename T>
	void Matrix<T> ::setCol(const size_t col, const std::vector<T>&& val)
	{
		if (col >= m_cols)
			throw std::range_error("Matrix index is out of range\n");
		if (val.size() > m_rows)
		{
			throw std::range_error("Data size is too large\n");
		}

		for (size_t i = 0; i != val.size(); i++)
		{
			m_mat[col + i * m_rows] = val[i];
		}
	}

	template<typename T>
	Matrix<T> & Matrix<T> ::transpose()
	{
		// TODO: insert return statement here
		if (m_cols != 1 && m_rows != 1)
		{
			decltype(m_cols) colStart = 0;
			for (size_t i = 0; i < m_rows; ++i)
			{
				for (size_t j = colStart; j < m_cols; ++j)
				{
					std::iter_swap(m_mat.begin() + (i * m_cols + j),
						m_mat.begin() + (j * m_rows + i));
				}
				++colStart;
			}
		}

		std::swap(m_rows, m_cols);

		return *this;
	}
}

