/*
  RMIT University Vietnam
  Course:     EEET2482 Software Engineering Design
  Semester:   2018A
  Assessment: Project
  Author:     Pham Nguyen Huan
  ID:         3695331
*/

class Term {
public:
	Term();
	Term(double coefficient, int power);
	Term(double coefficient, int power, Term* next);
	~Term();
	void setCoefficient(double coefficient);
	void setPower(int power);
	void setNext(Term* next);
	double getCoefficient();
	int getPower();
	Term* getNext();
private:
	double coefficient;
	int power;
	Term* next;
};