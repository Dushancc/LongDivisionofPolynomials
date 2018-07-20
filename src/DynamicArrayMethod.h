/*
  RMIT University Vietnam
  Course:     EEET2482 Software Engineering Design
  Semester:   2018A
  Assessment: Project
  Author:     Pham Nguyen Huan
  ID:         3695331
*/

#include <string>
using namespace std;

void errorChecking(string polynomialInput, bool* isValidPolynomial, string polynomialType);
string replaceDelimiter(string polynomialInput);
int getTermNumber(string polynomialInput);
bool hasDot(string number);
double** getTermSet(string polynomialInput, int termSetSize, bool* isValidPolynomial, string polynomialType);
bool isValidDivision(double** dividendTermSet, int dividendTermNumber, double** divisorTermSet, int divisorTermNumber);
double getDegree(double** termSet, int termNumber);
double** getResult(double** dividendTermSet, int dividendTermNumber, double** divisorTermSet, int divisorTermNumber, bool remainderOnly);
string outputPolynomial(double** polynomialTermSet, int polynomialTermNumber);
template<class T> T** copy2D(T** source, int dimension1Size, int dimension2Size);
template<class T> void delete2D(T** array, int dimension1Size);