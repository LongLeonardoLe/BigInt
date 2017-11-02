#include "BigInt.h"

//Divide the big number by 2
int Div2(string& A)
{
	string Tmp = ""; //result
	int cnt = 0, len = A.length();
	//compute
	for (int i = 0; i < len; i++){
		cnt = cnt * 10 + (A[i] - '0');
		Tmp += (cnt / 2 + '0');
		cnt %= 2;
	}
	len = Tmp.size();
	while (len > 1 && Tmp[0] == '0'){
		Tmp.erase(0, 1);
		len--;
	}
	A = Tmp;
	return cnt;
}

//convert from binary to decimal number
unsigned int Bin2Dec(string str)
{
	unsigned int x = 0;

	for (int i = 31; i >= 0; --i)
	{
		if ((unsigned int)32 - i <= str.length())
		{
			if (str[str.length() - (31 - i) - 1] == '0')
				x = x | 0 << (31 - i);
			else x = x | 1 << (31 - i);
		}
		else x = x | 0 << (31 - i);
	}

	return x;
}

//convert from decimal to QInt
unsigned int* Dec2QInt(string str)
{
	unsigned int* a = new unsigned int[4];
	// Init a = 0;
	for (int i = 0; i < 4; ++i)
		a[i] = 0;
	string binString;
	while (str != "0")
	{

		int bit = Div2(str);
		binString += (char)(bit + 48);
	}
	//transfer from binary to decimal
	float num = (float)binString.length() / 32; //number of string tmp, max is 4
	string tmp;
	int i = 0;
	size_t posi = (size_t)0;
	do
	{
		tmp = binString.substr(posi, 32);
		tmp = strrev(tmp);
		a[i] = Bin2Dec(tmp);
		//cout << a[i] <<endl;
		++i;
		posi += (size_t)32;
	} while (i <= num);

	return a;
}

string Hex2Bin(string str)
{
	string result;

	for (unsigned int i = 0; i < 8; ++i)
	{
		char tmp;
		if (i < str.length())
			tmp = str[i];
		else tmp = ' ';

		switch (tmp)
		{
		case '0':
			result += "0000";
			break;
		case'1':
			result += "0001";
			break;
		case'2':
			result += "0010";
			break;
		case'3':
			result += "0011";
			break;
		case'4':
			result += "0100";
			break;
		case'5':
			result += "0101";
			break;
		case'6':
			result += "0110";
			break;
		case'7':
			result += "0111";
			break;
		case'8':
			result += "1000";
			break;
		case'9':
			result += "1001";
			break;
		case'A':
			result += "1010";
			break;
		case'B':
			result += "1011";
			break;
		case'C':
			result += "1100";
			break;
		case 'D':
			result += "1101";
			break;
		case 'E':
			result += "1110";
			break;
		case 'F':
			result += "1111";
			break;
		default:
			result = "0000" + result;
			break;
		}
	}

	return result;
}

BigInt::BigInt()
{
	this->p1 = 0;
	this->p2 = 0;
	this->type = 0;
}

//input from file
void BigInt::input(ifstream& fin)
{
	string str;
	string tmp;
	getline(fin, str);

	size_t startPos = 0; //the start position to get the tmp string
	size_t spacePos; //the position of " " nearest startPos

	spacePos = str.find_first_of(" ");
	size_t len = spacePos - startPos; //length of the sub string gotten
	tmp = str.substr(startPos, len); //get the tmp string

	//first notice
	if (tmp == "10")
		this->p1 = 10;
	else if (tmp == "2")
		this->p1 = 2;
	else this->p1 = 16;

	//second notice if exist
	startPos = spacePos + 1;
	spacePos = str.find_first_of(" ", startPos);
	len = spacePos - startPos;
	tmp = str.substr(startPos, len);

	if (tmp == "10") // p2 = 10
		this->p2 = 10;
	else if (tmp == "2") // p2 = 2
		this->p2 = 2;
	else if (tmp == "16") // p2 = 16
		this->p2 = 16;
	else if (tmp == "ror" || tmp == "rol" || tmp == "~") // ror, rol, ~
	{
		this->p2 = 0;
		if (tmp == "ror")
			this->type = 7;
		else if (tmp == "rol")
			this->type = 8;
		else if (tmp == "~")
			this->type = 9;
	}
	else // a number
	{
		this->p2 = 0;
		this->a.transfer(tmp, this->p1);
	}

	//take the next argument
	startPos = spacePos + 1;
	spacePos = str.find_first_of(" ", startPos);
	len = spacePos - startPos;
	tmp = str.substr(startPos, len);

	if (spacePos == str.npos)
	{
		this->a.transfer(tmp, this->p1);
		return;
	}
	if (tmp == "+") //+
		this->type = 1;
	else if (tmp == "-") //-
		this->type = 2;
	else if (tmp == "*") //*
		this->type = 3;
	else if (tmp == "/") //operator /
		this->type = 4;
	else if (tmp == ">>") // >>
	{
		this->type = 5;
		this->shAmount = 0;
		tmp = str.substr(spacePos + 1);
		for (unsigned int i = 0; i < tmp.length(); ++i)
		{
			int tmp1 = (int)tmp[i] - 48;
			this->shAmount += tmp1 + (i * 10);
		}
		return;
	}
	else if (tmp == "<<") // <<
	{
		this->type = 6;
		this->shAmount = 0;
		tmp = str.substr(spacePos + 1);
		for (unsigned int i = 0; i < tmp.length(); ++i)
		{
			int tmp1 = (int)tmp[i] - 48;
			this->shAmount += tmp1 * i * 10;
		}
		return;
	}
	else if (tmp == "&") // &
		this->type = 10;
	else if (tmp == "|") // |
		this->type = 11;
	else if (tmp == "^")
		this->type = 12;

	this->b.transfer(str.substr(spacePos + 1), this->p1);
}

//return the result as a string
void BigInt::process()
{
	switch (this->type)
	{
	case 1:
	{
			  kq = a + b;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 2:
	{
			  kq = a - b;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 3:
	{
			  kq = a * b;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 4:
	{
			  kq = a / b;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 5:
	{
			  kq = a >> this->shAmount;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 6:
	{
			  kq = a << this->shAmount;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 7:
	{
			  kq = a.ROR();
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 8:
	{
			  kq = a.ROL();
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 9:
	{
			  kq = ~a;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 10:
	{
			   kq = a & b;
			  kqs = kq.Output(this->p1);
			  break;
	}
	case 11:
	{
			   kq = a | b;
			   kqs = kq.Output(this->p1);
			   break;
	}
	case 12:
	{
			   kq = a ^ b;
			   kqs = kq.Output(this->p1);
			   break;
	}
	default: //transfer case, from base 10 to 2 for example
	{
				 kq = a;
				 kqs = kq.Output(this->p2);
				 break;
	}
	}
}

//output the result with the string data type
void BigInt::output(ofstream& fout)
{
	fout.open("output.txt", ios::app);
	if (fout.is_open())
	{
		fout << this->kqs << endl;
	}
	fout.close();
}

/** string reverse **/
string strrev(const string&A)
{
	string tmp;
	for (unsigned int i = 0; i < A.length(); ++i)
	{
		tmp = A[i] + tmp;
	}
	return tmp;
}
