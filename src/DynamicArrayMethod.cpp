/*
  RMIT University Vietnam
  Course:     EEET2482 Software Engineering Design
  Semester:   2018A
  Assessment: Project
  Author:     Pham Nguyen Huan
  ID:         3695331
*/

#include <iostream>
#include <string>
#include <sstream>
#include "DynamicArrayMethod.h"
using namespace std;

// function to detect illegal characters in input polynomials ('.' is accepted here)
void errorChecking(string polynomialInput, bool* isValidPolynomial, string polynomialType) {
	if (!isdigit(polynomialInput[0]) && polynomialInput[0] != '+' &&
		polynomialInput[0] != '-' && polynomialInput[0] != 'x') {
		*isValidPolynomial = false;
		cout << "ERROR IN " << polynomialType << ": INVALID FIRST CHARACTER" << endl;
	}
	for (unsigned int i = 0; i < polynomialInput.length() - 1; i++) {
		if (isdigit(polynomialInput[i])) {
			if (!isdigit(polynomialInput[i + 1]) && polynomialInput[i + 1] != '+' &&
				polynomialInput[i + 1] != '-' && polynomialInput[i + 1] != '*' &&
				polynomialInput[i + 1] != '.') {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else if (polynomialInput[i] == '+') {
			if (!isdigit(polynomialInput[i + 1]) && polynomialInput[i + 1] != 'x') {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else if (polynomialInput[i] == '-') {
			if (!isdigit(polynomialInput[i + 1]) && polynomialInput[i + 1] != 'x') {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else if (polynomialInput[i] == '*') {
			if (polynomialInput[i + 1] != 'x') {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else if (polynomialInput[i] == '.') {
			if (!isdigit(polynomialInput[i + 1])) {
				isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else if (polynomialInput[i] == 'x') {
			if (polynomialInput[i + 1] != '+' && polynomialInput[i + 1] != '-' &&
				polynomialInput[i + 1] != '^') {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else if (polynomialInput[i] == '^') {
			if (!isdigit(polynomialInput[i + 1])) {
				isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
			}
		}
		else {
			*isValidPolynomial = false;
			cout << "ERROR IN " << polynomialType << ": INVALID CHARACTER AT POSITION " << i + 1 << endl;
		}
	}
	if (!isdigit(polynomialInput[polynomialInput.length() - 1]) &&
		polynomialInput[polynomialInput.length() - 1] != 'x') {
		*isValidPolynomial = false;
		cout << "ERROR IN " << polynomialType << ": INVALID LAST CHARACTER" << endl;
	}
}
// function to separate each term by a space
string replaceDelimiter(string polynomialInput) {
	for (unsigned int i = 0; i < polynomialInput.length(); i++) {
		if (polynomialInput[i] == '-' && i > 0) {
			polynomialInput.replace(i, 1, " -");
			i++;
		}
		else if (polynomialInput[i] == '+')
			polynomialInput.replace(i, 1, " ");
	}
	return polynomialInput;
}
// function to count the number of terms (counting spaces)
int getTermNumber(string polynomialInput) {
	int count = 1;
	for (unsigned int i = 0; i < polynomialInput.length(); i++)
		if (polynomialInput[i] == ' ')
			count++;
	return count;
}
// function to check if a number (string) has a decimal point
bool hasDot(string number) {
	for (unsigned int i = 0; i < number.length(); i++)
		if (number[i] == '.') return true;
	return false;
}
// function to analyse a polynomial (return a 2D array, coefficients in the first row and powers in the second one
double** getTermSet(string polynomialInput, int termSetSize, bool *isValidPolynomial, string polynomialType) {
	double** termSet = new double*[termSetSize];
	istringstream polynomialStream(polynomialInput);
	string term = "";
	for (int i = 0; i < termSetSize; i++) {
		polynomialStream >> term;
		if (term.length() == 1) {
			if (term == "x") term.replace(0, 1, "1 1");
			else term.replace(0, term.length(), term + " 0");
		}
		else if (term.length() == 2) {
			if (term == "-x") term.replace(0, 2, "-1 1");
			else term.replace(0, term.length(), term + " 0");
		}
		else if (term.length() >= 3) {
			bool nonZeroPower = false;
			if (term.substr(0, 2) == "x^") {
				nonZeroPower = true;
				term.replace(0, 2, "1 ");
			}
			else if (term.substr(0, 3) == "-x^") {
				nonZeroPower = true;
				term.replace(0, 3, "-1 ");
			}
			else if (term.substr(term.length() - 2, 2) == "*x") {
				nonZeroPower = true;
				term.replace(term.length() - 2, 2, " 1");
			}
			else {
				for (unsigned int i = 0; i < term.length(); i++) {
					if (term.substr(i, 3) == "*x^") {
						nonZeroPower = true;
						term.replace(i, 3, " ");
						break;
					}
				}
			}
			if (!nonZeroPower) term.replace(0, term.length(), term + " 0");
		}
		istringstream termStream(term);
		string coefficient = "", power = "";
		termStream >> coefficient >> power;
		termSet[i] = new double[2];
		termSet[i][0] = stod(coefficient); termSet[i][1] = stod(power);
		if (termSet[i][0] - (int)termSet[i][0] != 0 || hasDot(coefficient)) {
			*isValidPolynomial = false;
			cout << "ERROR IN " << polynomialType << ": FLOAT IN COEFFICIENT IN TERM " << i + 1 << endl;
		}
		if (termSet[i][1] - (int)termSet[i][1] != 0 || hasDot(power)) {
			*isValidPolynomial = false;
			cout << "ERROR IN " << polynomialType << ": FLOAT IN POWER IN TERM " << i + 1 << endl;
		}
		if (termSet[i][0] == 0) {
			*isValidPolynomial = false;
			cout << "ERROR IN " << polynomialType << ": ZERO IN COEFFICIENT IN TERM " << i + 1 << endl;
		}
	}
	return termSet;
}
// function to check if dividend has higher degree than divisor
bool isValidDivision(double** dividendTermSet, int dividendTermNumber, double** divisorTermSet, int divisorTermNumber) {
	double dividendDegree = getDegree(dividendTermSet, dividendTermNumber);
	double divisorDegree = getDegree(divisorTermSet, divisorTermNumber);
	if (dividendDegree >= divisorDegree) return true;
	return false;
}
// function to get the degree of a polynomial
double getDegree(double** termSet, int termNumber) {
	double highestDegree = termSet[0][1];
	for (int i = 0; i < termNumber; i++)
		if (highestDegree < termSet[i][1])
			highestDegree = termSet[i][1];
	return highestDegree;
}
// function to calculate the result or remainder of a long division
double** getResult(double** dividendTermSet, int dividendTermNumber, double** divisorTermSet, int divisorTermNumber, bool remainderOnly) {
	int dividendHighestDegree = (int)getDegree(dividendTermSet, dividendTermNumber);
	int divisorHighestDegree = (int)getDegree(divisorTermSet, divisorTermNumber);
	int resultHighestDegree = dividendHighestDegree - divisorHighestDegree;
	int remainderHighestDegree = divisorHighestDegree - 1;
	double** dividend = new double*[dividendHighestDegree + 1];
	double** divisor = new double*[divisorHighestDegree + 1];
	double** result = new double*[resultHighestDegree + 1];
	double** remainder = new double*[remainderHighestDegree + 1];
	for (int i = 0; i <= dividendHighestDegree; i++) {
		dividend[i] = new double[2];
		dividend[i][0] = 0;
		dividend[i][1] = (double)dividendHighestDegree - i;
		for (int j = 0; j < dividendTermNumber; j++)
			if (dividendTermSet[j][1] == dividendHighestDegree - i)
				dividend[i][0] += dividendTermSet[j][0];
	}
	for (int i = 0; i <= divisorHighestDegree; i++) {
		divisor[i] = new double[2];
		divisor[i][0] = 0;
		divisor[i][1] = (double)divisorHighestDegree - i;
		for (int j = 0; j < divisorTermNumber; j++)
			if (divisorTermSet[j][1] == divisorHighestDegree - i)
				divisor[i][0] += divisorTermSet[j][0];
	}
	for (int i = 0; i <= resultHighestDegree; i++) {
		result[i] = new double[2];
		result[i][0] = dividend[i][0] / divisor[0][0];
		result[i][1] = (double)resultHighestDegree - i;
		int tempPolynomialHighestDegree = dividendHighestDegree - i;
		double** tempPolynomial = new double*[tempPolynomialHighestDegree + 1];
		for (int j = 0; j <= tempPolynomialHighestDegree; j++) {
			tempPolynomial[j] = new double[2];
			tempPolynomial[j][1] = tempPolynomialHighestDegree - j;
			if (j <= divisorHighestDegree) tempPolynomial[j][0] = result[i][0]*divisor[j][0];
			else if (j > divisorHighestDegree) tempPolynomial[j][0] = 0;
			dividend[j + i][0] -= tempPolynomial[j][0];
		}
		delete2D<double>(tempPolynomial, tempPolynomialHighestDegree + 1);
	}
	for (int i = 0; i <= remainderHighestDegree; i++) {
		remainder[i] = new double[2];
		remainder[i][0] = dividend[resultHighestDegree + 1 + i][0];
		remainder[i][1] = dividend[resultHighestDegree + 1 + i][1];
	}
	delete2D<double>(dividend, dividendHighestDegree + 1);
	delete2D<double>(divisor, divisorHighestDegree + 1);
	if (remainderOnly) {
		delete2D<double>(result, resultHighestDegree + 1);
		return remainder;
	}
	delete2D<double>(remainder, remainderHighestDegree + 1);
	return result;
}
// function to convert a polynomial (in 2D array) to string
string outputPolynomial(double** polynomialTermSet, int polynomialTermNumber) {
	ostringstream polynomialStream("");
	for (int i = 0; i < polynomialTermNumber; i++) {
		if (polynomialTermSet[i][1] > 1) {
			if (polynomialTermSet[i][0] > 1 || (polynomialTermSet[i][0] > 0 && polynomialTermSet[i][0] < 1))
				polynomialStream << "+" << polynomialTermSet[i][0] << "*x^" << polynomialTermSet[i][1];
			else if (polynomialTermSet[i][0] == 1) polynomialStream << "+x^" << polynomialTermSet[i][1];
			else if (polynomialTermSet[i][0] == -1) polynomialStream << "-x^" << polynomialTermSet[i][1];
			else if (polynomialTermSet[i][0] < -1 || (polynomialTermSet[i][0] < 0 && polynomialTermSet[i][0] > -1))
				polynomialStream << polynomialTermSet[i][0] << "*x^" << polynomialTermSet[i][1];
		}
		else if (polynomialTermSet[i][1] == 1) {
			if (polynomialTermSet[i][0] > 1 || (polynomialTermSet[i][0] > 0 && polynomialTermSet[i][0] < 1))
				polynomialStream << "+" << polynomialTermSet[i][0] << "*x";
			else if (polynomialTermSet[i][0] == 1) polynomialStream << "+x";
			else if (polynomialTermSet[i][0] == -1) polynomialStream << "-x";
			else if (polynomialTermSet[i][0] < -1 || (polynomialTermSet[i][0] < 0 && polynomialTermSet[i][0] > -1))
				polynomialStream << polynomialTermSet[i][0] << "*x";
		}
		else if (polynomialTermSet[i][1] == 0) {
			if (polynomialTermSet[i][0] > 0) polynomialStream << "+" << polynomialTermSet[i][0];
			else if (polynomialTermSet[i][0] < 0) polynomialStream << polynomialTermSet[i][0];
		}
	}
	if (polynomialStream.str() == "") polynomialStream << 0;
	if (polynomialStream.str()[0] == '+') return polynomialStream.str().substr(1, polynomialStream.str().length() - 1);
	return polynomialStream.str();
}
// function to copy and delete dynamically allocated arrays
template<class T> T** copy2D(T** source, int dimension1Size, int dimension2Size) {
	T** destination = new T*[dimension1Size];
	for (int i = 0; i < dimension1Size; i++) {
		destination[i] = new T[dimension2Size];
		for (int j = 0; j < dimension2Size; j++)
			destination[i][j] = source[i][j];
	}
	return destination;
}
template<class T> void delete2D(T** array, int dimension1Size) {
	for (int i = 0; i < dimension1Size; i++)
		delete[] array[i];
}