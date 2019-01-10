#include "LiteMathMatrix.h"
#include <iostream>
#include <vector>

int main()
{
	LiteMath::Matrix<double> m1(2, 2);
	std::cout << "Row Count: " << m1.rowCount() << std::endl;
	std::cout << "Column Count: " << m1.colCount() << std::endl;

	LiteMath::Matrix<double> m2(2, 2);
	std::cout << m1.ones();
	std::cout << m2.ones();
	m2(0, 1) = 10.5;
	std::cout << m2(0, 1) << '\n';

	m2 *= 7;

	std::cout << m2 * 7;
	std::cout << 8.0 * m2;

	LiteMath::Matrix<double> m4(2, 2, std::vector<double>{1, 2, 3, 4});
	LiteMath::Matrix<double> m5(2, 1);
	m5.setMatrix(std::vector<double> {7.5, 10.8});

	LiteMath::Matrix<double> m6(3, 3, std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9});
	std::cout << "m6\n";
	std::cout << m6;
	std::cout << m6.transpose();

	LiteMath::Matrix<double> m7(3, 1, std::vector<double>{1, 2, 3});
	std::cout << "m7\n";
	std::cout << m7;
	std::cout << m7.transpose();

	//m1.setElement(0, 0, 19.0);
	//std::cout << m1.getElement(0, 0);

	std::cout << "Is Addition Legal: " << m1.isAddSubLegal(m2) << std::endl;

	m1 += m1;
	std::cout << "m1 + m2\n" << m1 + m2;
	std::cout << "m2 - m2\n" << m2 - m2;
	std::cout << "m1 * m2\n" << m1 * m2;

	std::cout << "m1 != m2: " << (m1 != m2) << std::endl;
	std::cout << "m1 == m2: " << (m1 == m2) << std::endl;

	m1.ones();

	std::cout << "m1 != m2: " << (m1 != m2) << std::endl;
	std::cout << "m1 == m2: " << (m1 == m2) << std::endl;

	LiteMath::Matrix<double> m3(10, 10);
	m3.ones();

	std::cout << "Is Addition Legal: " << m1.isAddSubLegal(m3) << std::endl;

	m3.setRow(0, std::vector<double> {22, 33, 44, 55});
	m3.setCol(9, std::vector<double> {66, 77, 88, 99});
	std::cout << m3;
	if(m1.isAddSubLegal(m3))
		m3 += m1;

	return 0;
}