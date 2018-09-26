/*
bs.cpp - Book Stack Test
Written by Alexey Lubkin
*/
#include "../statTests/include/stat_fncs.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <boost/math/distributions/chi_squared.hpp>

namespace statistical_tests_space {

#if (defined _MSC_VER) || (defined __BORLANDC__)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

#ifdef __GNUC__
#include <unistd.h>
#include <errno.h>
#define __int64 long long

typedef int errno_t;
errno_t fopen_s(FILE **f, const char *name, const char *mode) {
    errno_t ret = 0;
#ifdef _WIN32
    assert(f);
#endif
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}
#endif

using namespace std;

#define sq(x) (x)*(x)

bool qq = false; // quiet mode

const long NShift = 0;

struct THashValue;
typedef THashValue* PHashValue;
typedef PHashValue* PPHashValue;
struct THashValue
{
	unsigned long ID;
	PHashValue prevLink, nextLink;
	PHashValue Next;
};

class THash
{
	// "light" version of a hash table, designed specially for book stack test
protected:
	unsigned long NUpperPart;
	unsigned long NHashSize;
public:
	PPHashValue HashLinks;
	PHashValue  Meat;

	PHashValue Final, Cursor, FreePtr;

	unsigned long Hash(unsigned long x);

	PHashValue Find(unsigned long x);
	PHashValue Add(const THashValue &v);
	PHashValue AddAtCursor(const THashValue &v);

	THash(unsigned long aNUpperPart, unsigned long NHashSize);
	virtual void Clear();
	virtual ~THash();
};

class TBookStack : public THash
{
private:
	PHashValue first, last;
	__int64 NBooks;
	__int64 MaxSampleSize;
	unsigned long bpw; // bits per word
protected:
	bool stopped; // no more data
public:
	__int64 SampleSize;
	TBookStack(__int64 nn, unsigned long ww, unsigned long uu, unsigned long aNHashSize);
	// nn - max number of bits to process, ww - bits per word, uu - size of upper part
	PHashValue DeleteExist(PHashValue v); // delete book from the bookstack
	void PutBook(unsigned long x);        // put the book on the top
	bool IsBookPresent(unsigned long x);  // check, whether book is present in the upper part
	void MoveUp();                        // move current Book on the top
	void RemoveLast();                    // remove book at the bottom

	virtual unsigned long GetBits() = 0;    // read bits from input (cipher's output)
	double Run();                           // start test
	virtual void Clear();                 // return bookstack to initial state
};

class stdinBookStack : public TBookStack
{
private:
	unsigned long bpw;   // bits per word
	unsigned long blank; // number of bits to skip between words
	bool eof;            // end of file reached
	bool blankfirst;     // whether to skip bits before or after each word
	FILE* input;         // input stream
	unsigned long data;  // <=32 bits of input
	unsigned long bits;  // 0..32 bits in data
	void FillUp()        // reads bits from stdin and writes them into the data
	{
		unsigned long val;
		if (!eof)
			while (bits < 25)
			{
				if (eof = ((val = fgetc(input)) == (unsigned long)EOF)) break;
				data |= (val << bits);
				bits += 8;
			}
	}
public:
	stdinBookStack(FILE* aInput, bool aBlankfirst, __int64 nn, unsigned long ww, unsigned long bb, unsigned long uu, unsigned long aNHashSize)
		: bits(0), data(0), eof(false), input(aInput), blankfirst(aBlankfirst), bpw(ww), blank(bb), TBookStack(nn, ww, uu, aNHashSize)
		// aInput - input stream, aBlankfirst - whether start to read blank or data first,
		// nn - max number of bits to read, ww - bits per word, bb - bits between words, 
		// uu - size of upper part
	{
		if (blankfirst && (blank != 0))
		{
			FillUp();
			if (bits >= blank)
			{
				data = (data >> 1) >> (blank - 1);
				bits -= blank;
			}
			else stopped = true;
		}
	};
	virtual unsigned long GetBits() // get bpw bits from the input stream (skips blank bits as well)
	{
		if (stopped) return 0;
		unsigned long res = 0, filled = 0;
		// data
		if (bits >= bpw)
		{
			res = data & ((2 << (bpw - 1)) - 1); // now res contains the result
			bits -= bpw;
			data = (data >> 1) >> (bpw - 1); // for IA-32
		}
		else
		{
			if (bits > 0)
			{
				res = data & ((2 << (bits - 1)) - 1);
				filled = bits;
				data = bits = 0;
			}
			FillUp();
			res = (res | (data << filled)) &((2 << (bpw - 1)) - 1);

			if (filled + bits >= bpw)
			{
				bits -= bpw - filled;
				data = (data >> 1) >> (bpw - filled - 1);
				filled = bpw;
			}
			else
				stopped = true;
		}
		// blank
		if (blank)
		{
			filled = 0;
			if (bits < blank)
			{
				filled = bits; // skipped bits as blanks
				data = bits = 0;
				FillUp();
			}
			data = (data >> 1) >> (blank - filled - 1);
			bits -= blank - filled;
		}
		return res;
	}
};

// --------------------------------------
// TBookStack
// --------------------------------------

TBookStack::TBookStack(__int64 nn, unsigned long ww, unsigned long uu, unsigned long aNHashSize)
	: stopped(false), NBooks((__int64)(1) << ww), bpw(ww), MaxSampleSize(nn), THash(uu, aNHashSize)
{
	//if(!qq) printf("Initializing Hash...");
	Clear();
	//if(!qq) printf("Ok!\n");
}

void TBookStack::Clear()
{
	THash::Clear();
	size_t i;
	PHashValue c0, c1;
	THashValue v;

	v.ID = 0;
	v.prevLink = NULL;
	v.nextLink = NULL;
	v.Next = NULL;
	// Initialize by filling upper part with 0, 1, 2, ...., NUpperPart-1
	c0 = first = Add(v); FreePtr++;
	for (i = 1; i < NUpperPart; i++)
	{
		v.ID = uint32_t(i);
		v.prevLink = c0;
		c1 = Add(v); FreePtr++;
		c0->nextLink = c1;
		c0 = c1;
	}
	last = c0;
}


PHashValue TBookStack::DeleteExist(PHashValue v)
{
	PPHashValue cur;

	cur = &HashLinks[Hash(v->ID)];
	while (*cur != v)
		cur = &((*cur)->Next);
	*cur = (*cur)->Next;

	FreePtr = v;
	return v->prevLink;
}

void TBookStack::PutBook(unsigned long x)
{
	THashValue v;
	v.ID = x;
	v.nextLink = first;
	v.Next = NULL;
	first = first->prevLink = AddAtCursor(v);
}

bool TBookStack::IsBookPresent(unsigned long x)
{
	return Find(x) != NULL;
}

void TBookStack::MoveUp()
{
	if (Cursor == first) return;
	if (Cursor == last)
	{
		last = last->prevLink;
	}
	else
	{
		(Cursor->prevLink)->nextLink = Cursor->nextLink;
		(Cursor->nextLink)->prevLink = Cursor->prevLink;
	}
	Cursor->nextLink = first;
	first->prevLink = Cursor;
	first = Cursor;
}

void TBookStack::RemoveLast()
{
	last = DeleteExist(last);
}

// Return chi squared with one degree of freedom
double TBookStack::Run()
{
	__int64 i, up = 0, lo = 0;
	unsigned long val;
	__int64 milc = 0; unsigned long bitc = 0, mstep = 1 << 20;
	// main cycle
	for (i = 0; i < MaxSampleSize; i++)
	{
		// get next value
		val = GetBits();
		// print Mibits processed (1 Mibit = 2^20 bit)
		if (!qq)
		{
			bitc += bpw;
			if (bitc >= mstep)
			{
				bitc -= mstep; milc++;
				cout << "\r" << milc << " Mibits processed"; fflush(stdout);
			}
		}
		if (stopped) break;
		// update bookstack
		if (IsBookPresent(val))
		{
			MoveUp();
			up++;
		}
		else
		{
			PutBook(val);
			RemoveLast();
			lo++;
		}
	}
	SampleSize = i;
	//if (!qq) printf("\n");

	if (SampleSize > 0)
	{
		double ex_up = (double)SampleSize*NUpperPart / NBooks; // expected books in the upper part
		double ex_lo = (double)SampleSize*(NBooks - NUpperPart) / NBooks; // expected books in the lower part

		double chi = sq(up - ex_up) / ex_up + sq(lo - ex_lo) / ex_lo; // chi-square value

		/*if(!qq) printf("================\nx^2 = ");
		printf("%.4f\n",chi);
		if(!qq) printf("================\n");*/

		return chi;
	}
	return -1.;
}

// --------------------------------------
// THash
// --------------------------------------

unsigned long THash::Hash(unsigned long p)
{
	return ((unsigned long)p << NShift) % NHashSize;
}

PHashValue THash::Find(unsigned long x)
{
	PPHashValue
		pcur = &HashLinks[Hash(x)];
	PHashValue
		cur = *pcur, next;

	if (!(cur))
	{
		Final = (*pcur) = FreePtr;
		return Cursor = NULL;
	}
	if (cur->ID == x)
		return Cursor = cur;
	while (next = cur->Next)
	{
		if (next->ID == x)
			return Cursor = next;
		cur = next;
	}
	cur->Next = Final = FreePtr;
	return Cursor = NULL;
}

PHashValue THash::Add(const THashValue &v)
{
	Find(v.ID);
	*Final = v;
	return Final;
}

PHashValue THash::AddAtCursor(const THashValue &v)
{
	*Final = v;
	return Final;
}

THash::THash(unsigned long aNUpperPart, unsigned long aNHashSize) :
	NUpperPart(aNUpperPart), NHashSize(aNHashSize)
{
	long
		ds1 = sizeof(PHashValue) * NHashSize,
		ds2 = sizeof(THashValue) * (NUpperPart + 1);
	/* if(!qq) printf(
		"Allocating memory: \n  HashLinks (%dMb),  Meat (%dMb),  Total (%dMb)...",
		(ds1+1048575)/1048576,(ds2+1048575)/1048576,(ds1+ds2+1048575)/1048576
		);*/

	HashLinks = (PPHashValue)malloc(ds1 + ds2);
	if (!qq)
	{
		//if (HashLinks) printf("Ok!");
		//printf("\n");
	}
	Meat = (PHashValue)(((char*)HashLinks) + ds1);
	FreePtr = Meat;
	if (!HashLinks)
	{
		printf("An error occured while allocating memory.\n"); exit(1);
	}
}

void THash::Clear()
{
	FreePtr = Meat;
	long
		ds1 = sizeof(PHashValue) * NHashSize,
		ds2 = sizeof(THashValue) * (NUpperPart + 1);
	memset(HashLinks, 0, ds1 + ds2);
}

THash::~THash()
{
	//if(!qq) printf("Freeing memory...");
	free(HashLinks);
	//if(!qq) printf("Ok!\n");
}

void PrintHelp()
{
	printf("Book Stack test.\n");
	printf("Program usage:\n");
	printf("  bs [-f filename] [-n num] [-w num] [-b num] [-u num] [-q]\n");
	printf("    -f - specifies the input filename. If omitted then stdin is used.\n");
	printf("    -n - specifies the number of bits to read from file (or stdin).\n");
	printf("      If omitted then read all the data until EOF is reached.\n");
	printf("    -w - specifies the word size in bits\n");
	printf("      must be 1..32 (32 by default)\n");
	printf("    -b - specifies the blank size between words (in bits)\n");
	printf("      must be 0..32 (0 by default), note:\n");
	printf("      If the parameter comes after -w, than first w bits are used\n");
	printf("      to form the word and next b bits are skipped.\n");
	printf("      If the parameter comes before -w, than first b bits are skipped\n");
	printf("      and next w bits are used to form the word.\n");
	printf("    -u - specifies the size of the upper part of book stack\n");
	printf("      must be less than 2^w (2^(w/2) by default)\n");
	printf("    -q - quiet mode. Print x^2 value only.\n");
	printf("    -? - print this screen.\n");
	printf("Samples:\n");
	printf("  bs -f zk.bin -n 8388688 -w 32 -u 65536\n");
	printf("  bs -w 16 -u 16 <rc4.bin\n");
	printf("  bs -f data.bin -w 15 -b 1 -u 1024 -q\n");
}

void Quit()
{
	PrintHelp();
	exit(1);
}

double doBookStackTest(int argc, const char* argv[])
{
#ifdef _MSC_VER
	_setmode(_fileno(stdin), _O_BINARY);
#endif
#ifdef __BORLANDC__
	setmode(_fileno(stdin), O_BINARY);
#endif
	string ff;
	__int64 nn = ((__int64)(1) << 60) - 1;
	long ww = 32, bb = 0, uu;
	bool blankfirst = false;
	long bflag = 0, bflag2 = 0;

	FILE* input;

	int i;

	// parse commandline parameters
	for (i = 1; i < argc; i++)
	{
		string arg = argv[i];
		bool onemore = argc > i + 1;
		if (onemore)
		{
			if (arg == "-f")
			{
				i++;
				ff = argv[i];
			}
			else
				if (arg == "-n")
				{
					i++;
					istringstream is(argv[i]);
					is >> nn;
					if (is.fail()) Quit();
				}
				else
					if (arg == "-w")
					{
						i++;
						istringstream is(argv[i]);
						is >> ww;
						if (is.fail()) Quit();
						bflag = (bflag | 2) & 3; // or 010 , and 011
					}
					else
						if (arg == "-b")
						{
							i++;
							istringstream is(argv[i]);
							is >> bb;
							if (is.fail()) Quit();
							bflag |= 5;          // or 101
						}
						else
							if (arg == "-u")
							{
								i++;
								istringstream is(argv[i]);
								is >> uu;
								if (is.fail()) Quit();
								bflag2 = 1;
							}
							else
								if (arg == "-q") qq = true;
								else Quit();
		}
		else
			if (arg == "-q") qq = true;
			else Quit();
	}


	if (!qq)
	{
#ifdef __GNUC__
		usleep(250000); // wait 0.25 sec before starting output
#else
#if (defined _MSC_VER) || (defined __BORLANDC__)
		Sleep(250); // wait 0.25 sec before starting output
#endif
#endif
	}


	// check parameters
	if (ff.size() > 0)
	{
		fopen_s(&input, ff.c_str(), "rb");
		if (input == NULL)
		{
			printf("ERROR: Coundn't open %s\n", ff.c_str());
			Quit();
		}
	}
	else
		input = stdin;

	if (bflag2 == 0) uu = 1 << (ww / 2);

	if (nn <= 1) { printf("ERROR: Sample size is to small.\n"); Quit(); }
    if ((ww < 1) || (ww > 32)) { printf("ERROR: w (=%ld) is not acceptable.\n", ww); Quit(); }
    if ((bb < 0) || (bb > 32)) { printf("ERROR: b (=%ld) is not acceptable.\n", bb); Quit(); }
    if ((uu < 1) || (uu >= (__int64)(1) << ww)) { printf("ERROR: u (=%ld) is not acceptable.\n", uu); Quit(); }
	nn /= ww;

	blankfirst = bflag == 3;

	TBookStack* pbs;

	pbs = new stdinBookStack(input, blankfirst, nn, ww, bb, uu, (unsigned long)(uu*1.4142));

	//if(!qq) cout << "  Bookstack size: " << ((__int64)(1)<<ww) << "\n";
	//if(!qq) printf("  Upper part: %u\n", uu);

	double chi = pbs->Run();

	//if(!qq) cout << "  Sample size: " << (pbs->SampleSize * ww) <<" bits (" << pbs->SampleSize << " words) \n";
	//if (!qq)
		//if (pbs->SampleSize * uu / ((__int64)(1) << ww) < 5)
		//	printf("WARNING: The (SampleSize*u/(2^w) >= 5) is not true!\n");

	flush(cout);

	delete pbs;
    if (ff.size() > 0)
        fclose(input);

	//cout << "BookStack stat:\t\t\t" << ((p_value >= 0.01) ? "SUCCESS" : "FAILURE") 
	//<< "\t\tp_value = " << p_value << endl << endl;
//	double p_value = 0;
    double p_value = 1 -
        boost::math::cdf(boost::math::chi_squared_distribution<double>(1), chi);
	return p_value;
}



}
