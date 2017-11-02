#include "QInt.h"

/** Constructor **/
QInt::QInt()
{
	for (int i = 0; i < 4; ++i)
		a[i] = 0;
}

/** Constructor 1 argument **/
QInt::QInt(unsigned int x)
{
	a[0] = x;
	for (int i = 1; i<4; i++)
		a[i] = 0;
}

/** Input **/
void QInt::transfer(string str, int type)
{
	bool check;
	if (str[0] == '-')
	{
		check = true;
		str = str.substr(1);
	}
	else
		check = false;

	if (type == 2) //convert from binary to QInt
	{
		float num = (float)str.length() / 32; //number of string tmp, max is 4
		string tmp;
		int i = 0;
		size_t pos = (size_t)0;
		do
		{
			tmp = str.substr(pos, 32);
			this->a[i] = Bin2Dec(tmp);
			++i;
			pos += (size_t)32;
		} while (i <= num);
	}
	else if (type == 10) //convert from decimal to QInt
	{
		unsigned int* tmp = Dec2QInt(str);
		for (int i = 0; i < 4; ++i)
			this->a[i] = tmp[i];
	}
	else if (type == 16) //convert from hexa to QInt
	{
		float num = (float)str.length() / 8; //number of string tmp, max is 4
		string tmp;
		int i = 0;
		size_t pos = (size_t)0;
		do
		{
			tmp = str.substr(pos, 8);
			this->a[i] = Bin2Dec(Hex2Bin(tmp));
			++i;
			pos += (size_t)32;
		} while (i <= num);
	}

	if (check == true)
		*this = this->tru();
}

/** Output **/
string QInt::Output(int type)
{
	string re;
	char bit;
	QInt t;
	t = *this;
	re = "";
	switch (type)
	{
	case 2:
	{
			  for (int i = 127; i>-1; i--)
			  {
				  bit = (char)(this->getBit(i) + 48);
				  re = re + bit;
			  }
			  break;

	}
	case 10:
	{
			   QInt mod;
			   mod = 10;
			   QInt cs;
			   if (t.dau() == 1) re = re + "-";
			   if (t == 0) re = "0";
			   while (t>0)
			   {
				   cs = t % 10;
				   t = t / 10;
				   bit = (char)(cs.a[0] + 48);
				   re = bit + re;
			   }
			   break;
	}
	case 16:
	{
			   int dem = 0;
			   bit = 0;
			   string tmp;
			   for (int i = 127; i>-1; i--)
			   {
				   dem++;
				   bit = (bit << 1) | (this->getBit(i));
				   if (dem % 4 == 0)
				   {
					   if (bit > 9)
						   bit = (char)(bit + 55);
					   else bit = (char)(bit + 48);
					   re = re + bit;
					   bit = 0;
				   }
			   }
			   break;
	}
	default: 
		break;
	}

	while (re[0] == '0')
		re = re.substr(1);

	return re;
}

/** Operator = **/
QInt& QInt::operator =(const QInt&X)
{
	a[0] = X.a[0];
	a[1] = X.a[1];
	a[2] = X.a[2];
	a[3] = X.a[3];
	return *this;
}

/** Operator + **/
QInt QInt::operator +(const QInt&X)
{
	QInt result;
	long long Mod;
	unsigned int tmp;
	tmp = 0;
	tmp = ~tmp;
	Mod = (long long)tmp + 1;
	long long sum, nho;
	sum = 0;
	nho = 0;
	for (int i = 0; i<4; i++)
	{
		sum = (long long)a[i] + (long long)X.a[i] + nho;
		nho = sum / Mod;
		result.a[i] = sum % Mod;
	}
	return result;
}

/** Operator - **/
QInt QInt::operator -(const QInt&X)
{
	QInt truX;
	truX = X;
	truX = ~truX;
	truX = truX + 1;
	return (*this + truX);
}

/** Operator * **/
QInt QInt::operator *(const QInt& X)
{
	QInt tmp, b, re;
	b = X;
	tmp = 0;
	re = *this;
	while (!(b == 0))
	{
		if ((b.a[0] & 1) != 0)
			tmp = tmp + re;
		re = re << 1;
		b = b >> 1;
	}
	return tmp;

}

/** Operator / **/
QInt QInt::operator /(const QInt&X)
{
	bool sign;
	QInt D, re;
	re = *this;
	D = X;
	sign = re.dau() ^ D.dau();
	if (re.dau() == 1) re = re.tru();
	if (D.dau() == 1) D = D.tru();
	QInt l, r, mid;
	l = 0;
	r = re * 2;
	while (l + 1<r)
	{
		mid = (l + r) >> 1;
		if ((D*mid) == re)
		{
			return mid;
		}
		else
		if ((D*mid)>re)
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}

	if ((D*mid) <= re) re = mid;
	else re = mid - 1;

	if (sign == 1) re = re.tru();
	return re;
}

/** Operator mod **/
QInt QInt::operator %(const QInt&X)
{
	return (*this - (*this / X)*X);
}

/** Operator > **/
bool QInt::operator>(const QInt&X)
{
	QInt tmp;
	tmp = X;
	if ((tmp.dau() == 1) && (this->dau() == 0)) return 1;
	if ((tmp.dau() == 0) && (this->dau() == 1)) return 0;
	for (int i = 3; i>-1; i--)
	{
		if (a[i] < X.a[i]) return 0;
		else if ((a[i]) > (X.a[i])) return 1;
	}
	return 0;
}

///** Operator < **/
bool QInt::operator <(const QInt&X)
{
	QInt tmp;
	tmp = X;
	if ((tmp.dau() == 1) && (this->dau() == 0)) return 0;
	if ((tmp.dau() == 0) && (this->dau() == 1)) return 1;
	for (int i = 3; i>-1; i--)
	{
		if (a[i] < X.a[i]) return 1;
		else if (a[i] > X.a[i])return 0;
	}
	return 0;
}

/** Operator == **/
bool QInt::operator ==(const QInt&X)
{
	for (int i = 3; i>-1; i--)
	{
		if (a[i] != X.a[i]) return 0;
	}
	return 1;
}

/** Operator >= **/
bool QInt::operator >=(const QInt &X)
{
	return ((*this>X) || (*this == X));
}

/** Operator <= **/
bool QInt::operator <=(const QInt&X)
{
	return ((*this<X) || (*this == X));
}

/** Operator ~ **/
QInt QInt::operator ~()
{
	QInt re;
	re = *this;
	for (int i = 0; i<4; i++)
	{
		re.a[i] = ~re.a[i];
	}
	return re;
}

/** Operator tru() return -x **/
QInt QInt::tru()
{
	return (~(*this)) + 1;
}

/** QInt Operator AND **/
QInt QInt::operator &(const QInt&X)
{
	QInt re;
	re = *this;
	for (int i = 0; i<4; i++)
	{
		re.a[i] = re.a[i] & X.a[i];
	}
	return re;
}

/** QInt Operator | OR **/
QInt QInt::operator |(const QInt&X)
{
	QInt re;
	re = *this;
	for (int i = 0; i<4; i++)
	{
		re.a[i] = re.a[i] | X.a[i];
	}
	return re;
}

/** Operator ^ XOR **/
QInt QInt::operator ^(const QInt&X)
{
	QInt re;
	re = *this;
	for (int i = 0; i<4; i++)
	{
		re.a[i] = re.a[i] ^ X.a[i];
	}
	return re;
}

/** Operator Shift left **/
QInt QInt::operator <<(int x)
{
	QInt re;
	re = *this;
	int tmp;
	int bit;
	if ((x<1)) return *this;
	if (x>127)
	{
		return 0;
	}
	for (int i = 0; i<x; i++)
	{
		for (int j = 3; j>0; j--)
		{
			re.a[j] = re.a[j] << 1;
			bit = (re.a[j - 1] >> 31) & 1;
			re.a[j] = re.a[j] + bit;
		}
		re.a[0] = re.a[0] << 1;
	}

	return re;

}

/** Operator Shift Right arithmetic**/
QInt QInt::operator >>(int x)
{
	QInt re;
	re = *this;
	int mark;
	mark = 1;
	mark = mark << 31;
	mark = ~mark;
	int bit;
	if ((x<1)) return *this;
	if (x>127)
	{
		return 0;
	}
	for (int i = 0; i<x; i++)
	{
		for (int j = 0; j<3; j++)
		{
			re.a[j] = re.a[j] >> 1;
			bit = ((re.a[j + 1]) & 1) << 31;
			re.a[j] = (re.a[j] & mark) | bit;

		}
		re.a[3] = (int)re.a[3] >> 1;
	}

	return re;

}

/** Operator Shift Right Logical **/
QInt QInt::ShiftR(int x)
{
	QInt re;
	re = *this;
	int mark;
	mark = 1;
	mark = mark << 31;
	mark = ~mark;
	int bit;
	if ((x<1)) return *this;
	if (x>127)
	{
		return 0;
	}
	for (int i = 0; i<x; i++)
	{
		for (int j = 0; j<3; j++)
		{
			re.a[j] = re.a[j] >> 1;
			bit = ((re.a[j + 1]) & 1) << 31;
			re.a[j] = (re.a[j] & mark) | bit;

		}
		re.a[3] = re.a[3] >> 1;
	}

	return re;


}

/** Rotate Right **/
QInt QInt::ROR()
{
	QInt re;
	re = *this;
	int bit;
	bit = (a[0] & 1) << 31;
	re = re.ShiftR(1);
	re = re | bit;
	return re;

}

/** Rotate Left **/
QInt QInt::ROL()
{
	QInt re;
	re = *this;
	int bit;
	bit = (a[3] >> 31) & 1;
	re = re << 1;
	re = re | bit;
	return re;
}

/** Set Bit **/
QInt QInt::setBit(int x, int bit)
{
	int vt, vtb, mark;
	QInt re;
	re = *this;
	if ((x >= 0) && (x<128))
	{
		vt = x / 32;
		vtb = (x % 32);
		mark = 1;
		mark = mark << vtb;
		bit = bit << vtb;
		mark = ~mark;
		re.a[vt] = re.a[vt] & mark;
		re.a[vt] = re.a[vt] | bit;
	}
	return re;
}

/** get Bit **/
int QInt::getBit(int x)
{
	int vt, vtb;
	QInt tmp;
	tmp = *this;
	int re = 0;
	if ((x >= 0) && (x<128))
	{
		vt = x / 32;
		vtb = (x % 32);
		re = ((a[vt] >> vtb) & 1);
	}
	return re;
}

/** Output the sign **/
bool QInt::dau()
{
	return (a[3] >> 31) & 1;
}

/** Out  to screen **/
void QInt::Out()
{
	for (int i = 3; i >= 0; i--)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

void Out();
