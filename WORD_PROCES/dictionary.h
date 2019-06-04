// dictionary.h
//-------DEVRLOPEX-----------
//Created: Gerasimchuk A.
//Date: 3.04/2016

#ifndef  DICTIONARY_H_
#define DICTIONARY_H_

const uint8_t samples[]={
#include "sample.dic"
};
const SLeter dictionary[]={
#include "dic.dic"
};

const uint8_t noise[]={
#include "noise.dic"
};

const uint8_t pi[]={
#include "pi.dic"
};

#pragma pack(push,1)

const uint8_t *digit[]={
		"ZERO",
		"ONE",
		"TWO",
		"THREE",
		"FOUR",
		"FIVE",
		"SIX",
		"SEVEN",
		"EIGHT",
		"NINE",
		"TEN",
		"ELEVEN",
		"TWELVE",
		"THIRTEEN",
		"FOURTEEN",
		"FIFTEEN",
		"SIXTEEN",
		"SEVENTEEN",
		"EIGHTEEN",
		"NINETEEN",
		"TWENTY",
		"THIRTY",
		"FORTY",
		"FIFTY",
		"SIXTY",
		"SEVENTY",
		"EIGHTY",
		"NINETY",
		"HUNDRED",
		"THOUSAND",
		"MILLION"
};

#pragma pack(pop)

#endif
