#include "BigInt.h"
int main()
{
	BigInt b;
	ifstream fin;
	ofstream fout;

	fin.open("input.txt");
	if (fin.is_open());
	{
		while (fin.eof() == false)
		{
			b.input(fin);
			b.process();
			b.output(fout);
		}
	}
	fin.close();

	return 0;
}
