/*
 * common.c
 *
 *  Created on: Jan 22, 2014
 *      Author: HarveyIDG
 */

//#include <stdint.h>
//#include <stdbool.h
#include "FreeRTOS.h"
#include "hal_stdtypes.h"
#include "common.h"
#include "Tasks.h"


char g_version_string[VERSION_STRING_LENGTH] = "0.3.0.0_140228H";
//char g_version_string[VERSION_STRING_LENGTH] = "0.2.0.0_140228C";
//char g_sms_telnumber[SMS_TELNUMBER_LENGTH] = "18082776089\0";
char g_sms_telnumber[SMS_TELNUMBER_LENGTH] = "12067354781\0";
char g_mcs_nickname[MAX_KEYVALUE_LENGTH] = "12067354781\0";





uint16_t pow10uint16(int ten_to){
	int i = 0;
	uint16_t rtnval = 1;
	if(ten_to >= 0){
		for(i = 0; i < ten_to; i++)
			rtnval *= 10;
	}
	else
		rtnval = 0;

	return rtnval;
}

float pow10float(int ten_to){
	int i = 0;
	float rtnval = 1;
	if(ten_to >= 0){
		for(i = 0; i < ten_to; i++)
			rtnval *= 10;
	}
	else{
		for(i = 0; i > ten_to; i--)
			rtnval /= 10;
	}

	return rtnval;
}

int log10int8_t(int8_t num){
	int rtnval = 1;
	if(num < 0)
		num *= -1;

	while(num >= 10){
		num /= 10;
		rtnval++;
	}

	return rtnval;
}

int get_str_int8(int8_t num, char* target, int target_max_len){
	int rtnval = 0;

	int neededlen = 0;
	neededlen = log10int8_t(num);
	if(num < 0){
		target[rtnval] = '-';
		rtnval++;
		num *= -1;
	}

	if((neededlen + rtnval) > target_max_len){
		/* Can't fit the number into the allotted string */
		rtnval = 0;
	}
	else{
		int i = 0;
		for(i = 1; i <= neededlen; i++){
			target[neededlen - i + rtnval] = (uint8_t)(num % 10) + 0x30;
			num /= 10;
		}
		rtnval += neededlen;
	}
	return rtnval;
}

int log10int(int num){
	int rtnval = 1;
	if(num < 0)
		num *= -1;

	while(num >= 10){
		num /= 10;
		rtnval++;
	}

	return rtnval;
}

int get_str_int(int num, char* target, int target_max_len){
	int rtnval = 0;

	int neededlen = 0;
	neededlen = log10int(num);
	if(num < 0){
		target[rtnval] = '-';
		rtnval++;
		num *= -1;
	}

	if((neededlen + rtnval) > target_max_len){
		/* Can't fit the number into the allotted string */
		rtnval = 0;
	}
	else{
		int i = 0;
		for(i = 1; i <= neededlen; i++){
			target[neededlen - i + rtnval] = (uint8_t)(num % 10) + 0x30;
			num /= 10;
		}
		rtnval += neededlen;
	}
	return rtnval;
}

float round_to_nearest_whole(float x){
	boolean reflip = false;
	float y = x;

	if(x < 0){
		x *= -1;
		reflip = true;
	}

	/* Round it down */
	while(y > 1)
		y -= 1;

	x -= y;
	if(y >= 0.5)
		x += 1;

	return (x) * (reflip ? -1 : 1);
}

/*
 * only designed for 0 <= x < 10 - outside of this range it will act weird
 */
uint8_t get_1s_character(float x){
	/* Char is 0 */
	uint8_t rtnval = 0x30;
	uint8_t i = 0;
	float j = 0;

	if(x < 0)
		x *= -1;

	/* Round it down */
	while(x > 10)
		x -= 10;

	for(i = 0; i < 10; i++, j++){
		if(x < (j + 1)){
			rtnval = 0x30 + i;
			break;
		}
	}

	return rtnval;
}

/*
 * convert int to ascii int
 * 1245 converts to "1245"
 * max len is 8
 * (note that string output is MSB)
 * */
void mcs_int_to_asciiint (char* str, uint8_t len, uint32_t val)
{
  uint8_t i;
  for(i=1; i<=len; i++)
  {
    str[len-i] = (uint8_t) ((val % 10UL) + '0');
    val/=10;
  }

  str[i-1] = '\0';
}

/* x^y power */
int16_t mcs_pow(int16_t x, int16_t y) {
	int16_t i;

	if (y < 0)
		return 0;
	int16_t res = 1;

	for (i = 0; i < y; ++i) {
		res = res*x;
	}

	return res;
}



/* convert ascii type of gps messages to ints
 * Max len of strin is 4
 * i.e. "72"  converts to 72 (note that string input is MSB)
 * */
int16_t mcs_asciiint_to_int(char* instr, uint8_t len) {
	uint16_t val = 0;
	uint8_t i;

	for (i = 0; i < len; i++) {

		/* make sure ascii values are in range */
		if ((instr[i] > '9') || (instr[i] < '0')) {
			return -1;
		} else {
			val += ((instr[i] - '0') * mcs_pow(10, (len - i - 1)));
		}

	}

	return val;
}


/*
 * converts ascii hex string to its hex value
 * Max len of strin is 8
 * i.e. "7D"  converts to 0x7D = 125 (note that string input is MSB)
 */
uint32_t mcs_asciihex_to_int(char* strin, uint8_t len) {
	int i = 0;
	uint32_t rtnval = 0;

	if(len <= 8){
		for (i = 0; i < len; i++) {
			rtnval += (char) (
				(strin[i] >= 48 && strin[i] <= 57) ?
					strin[i] - 48 :
					((strin[i] >= 65 && strin[i] <= 70) ?
						strin[i] - 65 + 10 : strin[i] - 97 + 10))
						<< (4 * ((len-1) - i));
		}
	}
	return rtnval;
}




/*
 * convert int value to ascii hex value
 * max string len is 7
 * i.e. 1257 = 0x4e9 converts to "4E9"  (note that string output is MSB)
 * */
void mcs_int_to_asciihex( uint32_t number, char number_str[])
{
	uint8_t hex[16] = "0123456789ABCDEF";
	int32_t i=0;
	int32_t j=0;
	int32_t shift=0;
	uint8_t c=0;
	uint32_t index = 0;
	uint8_t start =0;

	/* clear array */
	for (i=0; i<7; i++){
		number_str[i] = 0;
	}
	//memset(number_str,0,8);

	number_str[j]='0';
	for (i=0; i<7; i++)
	{
		 index = (uint32_t)(((uint32_t)((uint32_t)(number) & ((uint32_t)(0xF000000>>shift)))) >> ((uint32_t)(24-shift)));
		 c =  hex[index];
		 if (c!='0') start++;
		 if (c!='0' || start!=0)
		 {
		 	number_str[j]=c;
			j++;
		 }
		 shift+=4;
	}
}



char * mcs_strcat(char* dest, char* src)
{
	char *p = dest;

	if (dest == NULL || src == NULL){
		return dest; /* we need not have to do anything */
	}

	while ((*dest) != '\0'){
		dest++;
	}

	while ((*src) != '\0'){
	*dest++ = *src++;
	}

	*dest = '\0';

	return p;
}





/* convert ascii type of gps messages into floats
 * TODO - len isn't fixed (length used in speed isn't correct, need to add
 * some error tolerance in this: only successfully parses x.xx, not xxx.xx)
 * */
float mcs_asciifloat_to_float(char* s, uint8_t len) {
//	float val = -1;
//	uint8_t i, k;
//	uint8_t wdlen = 0;
//	uint8_t flen = 0;
//
//	for (i = 0; i < len; i++){
//		/* keep parsing until floating point is found */
//		if (instr[i] == '.'){
//			wdlen = i;
//			break;
//		}
//	}
//
//	for (i = 0; i < wdlen; i++){
//		/* make sure ascii values are in range */
//		if ((instr[i] > '9') || (instr[i] < '0')) {
//			break;
//			//			return -1;
//		} else {
//			val += ((instr[i] - '0') * mcs_pow(10, (wdlen - i - 1)));
//		}
//	}
//
//	flen = len - wdlen - 1;
//
//	/* take care of floating point */
//	for (k = 0; k < flen; k++) {
//		/* make sure ascii values are in range */
//		if ((instr[k + wdlen + 1] > '9') || (instr[k + wdlen + 1] < '0')) {
//			break;
//			//			return -1;
//		} else {
//			val += ((float) (instr[k + wdlen + 1] - '0')
//					/ (float) mcs_pow(10, (k + 1)));
//		}
//	}
//
//	return val;

	  float rez = 0, fact = 1;
	  uint8_t point_seen = 0;

	  if (*s == '-'){
	    s++;
	    fact = -1;
	  };
	  for (point_seen= 0; *s; s++){
	    if (*s == '.'){
	      point_seen = 1;
	      continue;
	    };
	    int d = *s - '0';
	    if (d >= 0 && d <= 9){
	      if (point_seen) fact /= 10.0f;
	      rez = rez * 10.0f + (float)d;
	    };
	  };
	  return rez * fact;

}

float mcs_atof(char* s)
{
  float rez = 0, fact = 1;
  uint8_t point_seen = 0;

  if (*s == '-'){
    s++;
    fact = -1;
  };
  for (point_seen= 0; *s; s++){
    if (*s == '.'){
      point_seen = 1;
      continue;
    };
    int d = *s - '0';
    if (d >= 0 && d <= 9){
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (float)d;
    };
  };
  return rez * fact;
}


bool Is_Upper_Case(char a){
	return ((a >= 'A') && (a <= 'Z'));
}

bool Is_Number(char a){
	return ((a >= '0') && (a <= '9'));
}

bool Is_Same_Char(char a, char b, bool iscasesensitive){
	if(iscasesensitive)
		return a == b;

	/* Reach here by not being case sensitive; set a and b to lower case */
	a = Is_Upper_Case(a) ? (a + ('a' - 'A')) : a;
	b = Is_Upper_Case(b) ? (b + ('a' - 'A')) : b;

	return a == b;
}



