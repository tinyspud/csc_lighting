/*
 * common.h
 *
 *  Created on: Jan 22, 2014
 *      Author: HarveyIDG
 */

#ifndef COMMON_H_
#define COMMON_H_


#include <stdbool.h>

#define TRUE_FLAG	1
#define FALSE_FLAG	0

#define NULL_VAL	0


/* x to the power of y */
int16_t mcs_pow(int16_t x, int16_t y);



void mcs_int_to_asciihex( uint32_t number, char number_str[]);


#define BIT0                   (0x00000001)
#define BIT1                   (0x00000002)
#define BIT2                   (0x00000004)
#define BIT3                   (0x00000008)
#define BIT4                   (0x00000010)
#define BIT5                   (0x00000020)
#define BIT6                   (0x00000040)
#define BIT7                   (0x00000080)
#define BIT8                   (0x00000100)
#define BIT9                   (0x00000200)
#define BIT10                  (0x00000400)
#define BIT11                  (0x00000800)
#define BIT12                  (0x00001000)
#define BIT13                  (0x00002000)
#define BIT14                  (0x00004000)
#define BIT15                  (0x00008000)
#define BIT16                  (0x00010000)
#define BIT17                  (0x00020000)
#define BIT18                  (0x00040000)
#define BIT19                  (0x00080000)
#define BIT20                  (0x00100000)
#define BIT21                  (0x00200000)
#define BIT22                  (0x00400000)
#define BIT23                  (0x00800000)
#define BIT24                  (0x01000000)
#define BIT25                  (0x02000000)
#define BIT26                  (0x04000000)
#define BIT27                  (0x08000000)
#define BIT28                  (0x10000000)
#define BIT29                  (0x20000000)
#define BIT30                  (0x40000000)
#define BIT31                  (0x80000000)


#define VERSION_STRING_LENGTH	(15)
extern char g_version_string[VERSION_STRING_LENGTH];

#define SMS_TELNUMBER_LENGTH	(12)
#define SMS_MIN_TELNUMBER_LENGTH	(5)
extern char g_sms_telnumber[SMS_TELNUMBER_LENGTH];

#define MAX_KEYVALUE_LENGTH 30
extern char g_mcs_nickname[MAX_KEYVALUE_LENGTH];


#define SMS_TXT_LENGTH 			(160)

#define PRINTED_RECEIPT_LENGTH	(300)


uint16_t pow10uint16(int);

float pow10float(int);

uint8_t get_1s_character(float);

float round_to_nearest_whole(float);

int log10int8_t(int8_t);

int get_str_int8(int8_t, char*, int);

int log10int(int);

int get_str_int(int, char*, int);

void mcs_int_to_asciiint (char* str, uint8_t len, uint32_t val);

int16_t mcs_asciiint_to_int(char* instr, uint8_t len);

char * mcs_strcat(char* dest, char* src);

float mcs_asciifloat_to_float(char* instr, uint8_t len);

float mcs_atof(char* s);

uint32_t mcs_asciihex_to_int(char* strin, uint8_t len);

bool Is_Upper_Case(char a);
bool Is_Same_Char(char a, char b, bool iscasesensitive);
bool Is_Number(char a);


#endif /* COMMON_H_ */



