#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class QInt
{
public:
	QInt();
	QInt(unsigned int x);
	void transfer(string str, int type);

	string Output(int);
	void Out();

	QInt& operator =(const QInt&X);
	QInt operator +(const QInt&X);
	QInt operator -(const QInt&X);
	QInt operator *(const QInt&X);
	QInt operator /(const QInt&X);
	QInt operator %(const QInt&X);
	QInt operator &(const QInt&X);
	QInt operator |(const QInt&X);
	QInt operator ^(const QInt&X);
	QInt operator <<(int x);
	QInt operator >>(int x);
	//QInt operator >>>(int x);
	QInt ROR();
	QInt ROL();
	QInt ShiftR(int);
	QInt operator ~();
	bool operator >(const QInt&X);
	bool operator <(const QInt&X);
	bool operator ==(const QInt&X);
	bool operator >=(const QInt&X);
	bool operator <=(const QInt&X);
	QInt tru();
	QInt setBit(int, int);
	int getBit(int);
	bool dau();

private:
	unsigned int a[4];
};

//from a binary to a decimal
unsigned int Bin2Dec(string str);

//from a big decimal integer to QInt
unsigned int* Dec2QInt(string str);

string Hex2Bin(string str);

int Div2(string& A);

string strrev(const string&A);
