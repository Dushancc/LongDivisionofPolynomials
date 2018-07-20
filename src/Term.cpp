/*
  RMIT University Vietnam
  Course:     EEET2482 Software Engineering Design
  Semester:   2018A
  Assessment: Project
  Author:     Pham Nguyen Huan
  ID:         3695331
*/

#include "Term.h"
#define NULL 0
using namespace std;

Term::Term() { coefficient = 0; power = 0; next = NULL; }
Term::Term(double coefficient, int power) {
	this->coefficient = coefficient;
	this->power = power;
	next = NULL;
}
Term::Term(double coefficient, int power, Term* next) {
	this->coefficient = coefficient;
	this->power = power;
	this->next = next;
}
Term::~Term() { next = NULL; }
void Term::setCoefficient(double coefficient) { this->coefficient = coefficient; }
void Term::setPower(int power) { this->power = power; }
void Term::setNext(Term* next) { this->next = next; }
double Term::getCoefficient() { return coefficient; }
int Term::getPower() { return power; }
Term* Term::getNext() { return next; }