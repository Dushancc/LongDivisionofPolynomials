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
#include "Term.h"
// de-comment the next line if you need stdafx.h
//#include "stdafx.h"
#define ERROR 1
#define NO_ERROR 0
using namespace std;

void dynamicArrayMethod(string dividend, string divisor, bool* isDynamicArraySuccessful);
void linkedListMethod(string dividend, string divisor, bool* isLinkedListSuccessful);

class Polynomial {
private:
	Term* firstTerm = NULL;
	Term* lastTerm = NULL;
	int length = 0;
public:
	Polynomial() { firstTerm = NULL; lastTerm = NULL; length = 0; }
	Polynomial(double coefficient, int power) {
		Term* newTerm = new Term(coefficient, power);
		firstTerm = newTerm;
		lastTerm = newTerm;
		length = 1;
	}
	Polynomial(string polynomial, bool* isValidPolynomial, string polynomialType) {
		firstTerm = NULL; lastTerm = NULL; length = 0;
		// separate each terms with a space using replaceDelimiter()
		polynomial = replaceDelimiter(polynomial);
		// count the number of terms using getTermNumber()
		int termNumber = getTermNumber(polynomial);
		// use istringstream to extract terms from the original input
		istringstream polynomialStream(polynomial);
		string term = "";
		for (int i = 0; i < termNumber; i++) {
			polynomialStream >> term;
			// convert "n*x^k" to "n k"
			if (term.length() == 1) {
				// transform "x" to "1 1"
				if (term == "x") term.replace(0, 1, "1 1");
				// transform "n" to "n 0"
				else term.replace(0, term.length(), term + " 0");
			}
			else if (term.length() == 2) {
				// transform "-x" to "-1 1"
				if (term == "-x") term.replace(0, 2, "-1 1");
				// transfrom "n" to "n 0"
				else term.replace(0, term.length(), term + " 0");
			}
			else if (term.length() >= 3) {
				bool nonZeroPower = false;
				// transform "x^k" to "1 k"
				if (term.substr(0, 2) == "x^") { nonZeroPower = true; term.replace(0, 2, "1 "); }
				// transform "-x^k" to "-1 k"
				else if (term.substr(0, 3) == "-x^") { nonZeroPower = true; term.replace(0, 3, "-1 "); }
				// tranform "n*x" to "n 1"
				else if (term.substr(term.length() - 2, 2) == "*x") { nonZeroPower = true; term.replace(term.length() - 2, 2, " 1"); }
				else {
					// transform "n*x^k" to "n k"
					for (unsigned int i = 0; i < term.length(); i++) {
						if (term.substr(i, 3) == "*x^") {
							nonZeroPower = true;
							term.replace(i, 3, " ");
							break;
						}
					}
				}
				// transform "n" to "n 0"
				if (!nonZeroPower) term.replace(0, term.length(), term + " 0");
			}
			istringstream termStream(term);
			double coefficient = 0, power = 0;
			string coefficientString = "", powerString = "";
			termStream >> coefficientString >> powerString;
			coefficient = stod(coefficientString); power = stod(powerString);
			// find float numbers in coefficients and powers, and zero coefficients
			if (coefficient - (int)coefficient != 0 || hasDot(coefficientString)) {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": FLOAT IN COEFFICIENT IN TERM " << i + 1 << endl;
			}
			if (power - (int)power || hasDot(powerString)) {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": FLOAT IN POWER IN TERM " << i + 1 << endl;
			}
			if (coefficient == 0) {
				*isValidPolynomial = false;
				cout << "ERROR IN " << polynomialType << ": ZERO IN COEFFICIENT IN TERM " << i + 1 << endl;
			}
			append(coefficient, (int)power);
		}
		refine();
	}
	Polynomial(const Polynomial& another) {
		firstTerm = NULL; lastTerm = NULL; length = 0;
		*this = another;
	}
	~Polynomial() { deleteAll(); }
	const Polynomial& operator=(const Polynomial& another) {
		if (this != &another) {
			if (firstTerm != NULL) deleteAll();
			Term* copyTerm = NULL;
			Term* originalTerm = another.firstTerm;
			while (originalTerm != NULL) {
				if (firstTerm == NULL)
				firstTerm = copyTerm = new Term(originalTerm->getCoefficient(), originalTerm->getPower());
				else {
					copyTerm->setNext(new Term(originalTerm->getCoefficient(), originalTerm->getPower()));
					copyTerm = copyTerm->getNext();
				}
			originalTerm = originalTerm->getNext();
			lastTerm = copyTerm; length++;
			}
		}
		return *this;
	}
	void refine() {
		// relocate terms by the order of their powers from largest to lowest
		Polynomial refined;
		int degree = this->getDegree();
		for (int i = 0; i <= degree; i++) {
			int power = degree - i;
			double coefficient = 0;
			for (Term* currentTerm = this->firstTerm; currentTerm != NULL; currentTerm = currentTerm->getNext()) {
				if (currentTerm->getPower() == power) coefficient += currentTerm->getCoefficient();
			}
			if (coefficient != 0) refined.append(coefficient, power);
		}
		// clear the current content and pass in the refined content
		if (refined.firstTerm == NULL) refined.append(0, 0);
		this->deleteAll();
		for (Term* currentTerm = refined.getFirstTerm(); currentTerm != NULL; currentTerm = currentTerm->getNext())
			this->append(currentTerm->getCoefficient(), currentTerm->getPower());
	}
	void append(double coefficient, int power) {
		if (firstTerm == NULL && lastTerm == NULL && length == 0) {
			firstTerm = new Term(coefficient, power);
			lastTerm = firstTerm; length++;
		}
		else if (firstTerm != NULL && lastTerm != NULL && length > 0) {
			lastTerm->setNext(new Term(coefficient, power));
			lastTerm = lastTerm->getNext(); length++;
		}
	}
	void addNext(int position, double coefficient, int power) {
		Term* currentTerm = this->at(position);
		if (currentTerm == lastTerm) append(coefficient, power);
		else if (currentTerm == NULL) cout << "ERROR: UNDEFINED PLACE TO ADD NEXT" << endl;
		else {
			Term* newTerm = new Term(coefficient, power);
			newTerm->setNext(currentTerm->getNext());
			currentTerm->setNext(newTerm);
			length++;
		}
	}
	void deleteAt(int position) {
		Term* currentTerm = this->at(position);
		if (currentTerm == NULL) cout << "ERROR: UNDEFINED PLACE TO DELETE" << endl;
		else if (currentTerm == lastTerm) this->deleteLastTerm();
		else if (currentTerm == firstTerm) this->deleteFirstTerm();
		else {
			this->at(position - 1)->setNext(this->at(position + 1));
			delete currentTerm;
			length--;
		}
	}
	void deleteAll() {
		if (firstTerm != NULL && lastTerm != NULL && length > 0) {
			Term* currentTerm = firstTerm;
			while (currentTerm != NULL) {
				firstTerm = currentTerm->getNext();
				delete currentTerm; currentTerm = firstTerm;
				length--;
			}
			lastTerm = NULL;
		}
	}
	void deleteFirstTerm() {
		if (firstTerm != NULL && lastTerm != NULL && length > 0) {
			if (firstTerm == lastTerm && length == 1) {
				delete firstTerm;
				firstTerm = NULL; lastTerm = NULL; length = 0;
			}
			else if (firstTerm != lastTerm && length > 1) {
				Term* currentTerm = firstTerm;
				firstTerm = firstTerm->getNext();
				delete currentTerm;
				length--;
			}
		}
	}
	void deleteLastTerm() {
		if (firstTerm != NULL && lastTerm != NULL && length > 0) {
			if (firstTerm == lastTerm && length == 1) {
				delete lastTerm;
				firstTerm = NULL; lastTerm = NULL; length = 0;
			}
			else if (firstTerm != lastTerm && length > 1) {
				at(length - 2)->setNext(NULL);
				delete lastTerm;
				lastTerm = at(length - 2);
				length--;
			}
		}
	}
	Term* at(int termIndex) {
		if (termIndex < 0 || termIndex >= length) return NULL;
		else {
			Term* currentTerm = firstTerm;
			for (int i = 0; i < termIndex; i++) currentTerm = currentTerm->getNext();
			return currentTerm;
		}
	}
	string toString() {
		if (firstTerm == NULL) return "0";
		else {
			ostringstream polynomialStream("");
			for (Term* currentTerm = firstTerm; currentTerm != NULL; currentTerm = currentTerm->getNext()) {
				if (currentTerm->getPower() > 1) {
					if (currentTerm->getCoefficient() > 1 || (currentTerm->getCoefficient() > 0 && currentTerm->getCoefficient() < 1))
						polynomialStream << "+" << currentTerm->getCoefficient() << "*x^" << currentTerm->getPower();
					else if (currentTerm->getCoefficient() == 1) polynomialStream << "+x^" << currentTerm->getPower();
					else if (currentTerm->getCoefficient() == -1) polynomialStream << "-x^" << currentTerm->getPower();
					else if (currentTerm->getCoefficient() < -1 || (currentTerm->getCoefficient() < 0 && currentTerm->getCoefficient() > -1))
						polynomialStream << currentTerm->getCoefficient() << "*x^" << currentTerm->getPower();
				}
				else if (currentTerm->getPower() == 1) {
					if (currentTerm->getCoefficient() > 1 || (currentTerm->getCoefficient() > 0 && currentTerm->getCoefficient() < 1))
						polynomialStream << "+" << currentTerm->getCoefficient() << "*x";
					else if (currentTerm->getCoefficient() == 1) polynomialStream << "+x";
					else if (currentTerm->getCoefficient() == -1) polynomialStream << "-x";
					else if (currentTerm->getCoefficient() < -1 || (currentTerm->getCoefficient() < 0 && currentTerm->getCoefficient() > -1))
						polynomialStream << currentTerm->getCoefficient() << "*x";
				}
				else if (currentTerm->getPower() == 0) {
					if (currentTerm->getCoefficient() > 0) polynomialStream << "+" << currentTerm->getCoefficient();
					else if (currentTerm->getCoefficient() < 0) polynomialStream << currentTerm->getCoefficient();
				}
			}
			if (polynomialStream.str() == "") polynomialStream << 0;
			if (polynomialStream.str()[0] == '+') return polynomialStream.str().substr(1, polynomialStream.str().length() - 1);
			else return polynomialStream.str();
		}
	}
	int getLength() { return length; }
	int getDegree() {
		if (firstTerm == NULL) return -1;
		else {
			Term* currentTerm = firstTerm;
			int degree = firstTerm->getPower();
			while (currentTerm != NULL) {
				if (currentTerm->getPower() > degree) degree = currentTerm->getPower();
				currentTerm = currentTerm->getNext();
			}
			return degree;
		}
	}
	Term* getFirstTerm() { return firstTerm; }
	Term* getLastTerm() { return lastTerm; }
};

Polynomial getResult(Polynomial dividend, Polynomial divisor, bool remainderOnly);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "ERROR: UNEXPECTED NUMBER OF ARGUMENTS" << endl;
		return ERROR;
	}
	if (argc == 2) {
		if (strcmp(argv[1], "0") != 0 && strcmp(argv[1], "1") != 0) {
			cout << "ERROR: UNEXPECTED INPUT ARGUMENT" << endl;
			return ERROR;
		}
		if (strcmp(argv[1], "0") == 0) cout << "3695331,s3695331@rmit.edu.vn,Pham Nguyen Huan" << endl;
		else if (strcmp(argv[1], "1") == 0) {
			string dividend = "", divisor = "";
			bool isValidDividend = true, isValidDivisor = true;
			bool isDynamicArraySuccessful = true, isLinkedListSuccessful = true;
			cout << "Input the dividend (eg. x^4+3*x^3-8*x^2+x-9):" << endl;
			getline(cin, dividend);
			cout << "Input the divisor (eg. x+2):" << endl;
			getline(cin, divisor);
			cout << "Dividend:  " << dividend << endl;
			// check for errors in dividend (accept floats (ie ".") at this stage)
			errorChecking(dividend, &isValidDividend, "DIVIDEND");
			cout << "Divisor:   " << divisor << endl;
			// check for errors in divisor (accept floats (ie ".") at this stage)
			errorChecking(divisor, &isValidDivisor, "DIVISOR");
			if (!isValidDividend || !isValidDivisor) return ERROR;
			// call for two methods
			dynamicArrayMethod(dividend, divisor, &isDynamicArraySuccessful);
			linkedListMethod(dividend, divisor, &isLinkedListSuccessful);
			// return error code if one of the methods fails
			if (!isLinkedListSuccessful|| !isDynamicArraySuccessful) return ERROR;
		}
	}
	return NO_ERROR;
}

void dynamicArrayMethod(string dividend, string divisor, bool* isDynamicArraySuccessful) {
	cout << "Method 1: Dynamic Array" << endl;
	// separate each terms in dividend and divisor with a space
	dividend = replaceDelimiter(dividend);
	divisor = replaceDelimiter(divisor);
	// count the number of terms in each polynomials
	int dividendTermNumber = getTermNumber(dividend);
	int divisorTermNumber = getTermNumber(divisor);
	bool isValidDividend = true, isValidDivisor = true;
	// read and analyse two polynomials
	double** dividendTermSet = getTermSet(dividend, dividendTermNumber, &isValidDividend, "DIVIDEND");
	double** divisorTermSet = getTermSet(divisor, divisorTermNumber, &isValidDivisor, "DIVISOR");
	// return and set the status flag to false to indicate the unsuccess
	if (!isValidDividend || !isValidDivisor) {
		*isDynamicArraySuccessful = false;
		delete2D<double>(dividendTermSet, dividendTermNumber);
		delete2D<double>(divisorTermSet, divisorTermNumber);
		return;
	}
	// return if the divisor has higher degree than the dividend
	if (!isValidDivision(dividendTermSet, dividendTermNumber, divisorTermSet, divisorTermNumber)) {
		*isDynamicArraySuccessful = false;
		cout << "ERROR: HIGHER DEGREE DIVISOR" << endl;
		delete2D<double>(dividendTermSet, dividendTermNumber);
		delete2D<double>(divisorTermSet, divisorTermNumber);
		return;
	}
	// get the result and remainder of the division
	double** result = getResult(dividendTermSet, dividendTermNumber, divisorTermSet, divisorTermNumber, false);
	double** remainder = getResult(dividendTermSet, dividendTermNumber, divisorTermSet, divisorTermNumber, true);
	int resultHighestDegree = (int)getDegree(dividendTermSet, dividendTermNumber) -
							  (int)getDegree(divisorTermSet, divisorTermNumber);
	int remainderHighestDegree = (int)getDegree(divisorTermSet, divisorTermNumber) - 1;
	cout << "Quotient:  " << outputPolynomial(result, resultHighestDegree + 1) << endl;
	cout << "Remainder: " << outputPolynomial(remainder, remainderHighestDegree + 1) << endl;
	// delete the memory allocated on the heap
	delete2D<double>(dividendTermSet, dividendTermNumber);
	delete2D<double>(divisorTermSet, divisorTermNumber);
	delete2D<double>(result, resultHighestDegree + 1);
	delete2D<double>(remainder, remainderHighestDegree + 1);
}
void linkedListMethod(string dividend, string divisor, bool* isLinkedListSuccessful) {
	cout << "Method 2: Linked List" << endl;
	bool isValidDividend = true, isValidDivisor = true;
	// read and analyse the two polynomials using the constructor
	Polynomial dividendList(dividend, &isValidDividend, "DIVIDEND");
	Polynomial divisorList(divisor, &isValidDivisor, "DIVISOR");
	// return if there are any errors
	if (!isValidDividend || !isValidDivisor) {
		*isLinkedListSuccessful = false;
		return;
	}
	if (dividendList.getDegree() < divisorList.getDegree()) {
		*isLinkedListSuccessful = false;
		cout << "ERROR: HIGHER DEGREE DIVISOR" << endl;
		return;
	}
	cout << "Quotient:  " << getResult(dividendList, divisorList, false).toString() << endl;
	cout << "Remainder: " << getResult(dividendList, divisorList, true).toString() << endl;
}

Polynomial getResult(Polynomial dividend, Polynomial divisor, bool remainderOnly) {
	int dividendDegree = dividend.getDegree();
	int divisorDegree = divisor.getDegree();
	int resultDegree = dividendDegree - divisorDegree;
	int remainderDegree = divisorDegree - 1;
	Polynomial result, remainder;
	// please refer to the flow chart
	for (int i = 0; i <= dividendDegree; i++) {
		int power = dividendDegree - i;
		if (dividend.at(i) == NULL) dividend.append(0, power);
		else if (dividend.at(i)->getPower() < power) dividend.addNext(i - 1, 0, power);
	}
	for (int i = 0; i <= divisorDegree; i++) {
		int power = divisorDegree - i;
		if (divisor.at(i) == NULL) divisor.append(0, power);
		else if (divisor.at(i)->getPower() < power) divisor.addNext(i - 1, 0, power);
	}
	for (int i = 0; i <= resultDegree; i++) {
		result.append(dividend.at(i)->getCoefficient()/divisor.getFirstTerm()->getCoefficient(), resultDegree - i);
		int tempPolynomialDegree = dividendDegree - i;
		Polynomial tempPolynomial;
		for (int j = 0; j <= tempPolynomialDegree; j++) {
			if (j <= divisorDegree)
				tempPolynomial.append(result.at(i)->getCoefficient()*divisor.at(j)->getCoefficient(), tempPolynomialDegree - j);
			else if (j > divisorDegree)
				tempPolynomial.append(0, tempPolynomialDegree - j);
			dividend.at(j + i)->setCoefficient(dividend.at(j + i)->getCoefficient() - tempPolynomial.at(j)->getCoefficient());
		}
		tempPolynomial.deleteAll();
	}
	for (int i = 0; i <= remainderDegree; i++) {
		remainder.append(dividend.at(resultDegree + 1 + i)->getCoefficient(), dividend.at(resultDegree + 1 + i)->getPower());
	}
	for (int i = 0; i < result.getLength(); i++) {
		if (result.at(i)->getCoefficient() == 0) {
			result.deleteAt(i);
			i--;
		}
	}
	for (int i = 0; i < remainder.getLength(); i++) {
		if (remainder.at(i)->getCoefficient() == 0) {
			remainder.deleteAt(i);
			i--;
		}
	}
	if (result.getFirstTerm() == NULL) result.append(0, 0);
	if (remainder.getFirstTerm() == NULL) remainder.append(0, 0);
	if (remainderOnly) return remainder;
	return result;
}