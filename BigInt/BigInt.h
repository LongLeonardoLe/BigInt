#include "QInt.h"
class BigInt
{
private:
	QInt a, b, kq;
	string x, y, kqs;
	int p1, p2;//2, 10, 16
	int type;
	int shAmount;
	//1. +
	//2. -
	//3. *
	//3. /
	//5. >>
	//6. <<
	//7. ror
	//8. rol
	//9. ~
	//10. &
	//11. |
	//12. ^

public:
	BigInt();

	void input(ifstream& fin);
	void process();
	void output(ofstream& fout);
};


