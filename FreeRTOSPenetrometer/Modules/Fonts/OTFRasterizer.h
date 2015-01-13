/* OTF file format rasterizer */

#ifndef __OTF_RASTERIZER_H
#define __OTF_RASTERIZER_H

#include <stdint.h>
#include <stdbool.h>
#include <hal_stdtypes.h>

#ifndef uint24_t
/* Create 24-bit intiger */
typedef uint32_t	uint24_t;
#endif


/* OTF Types */
typedef uint8_t			BYTE;
typedef int8_t			CHAR;
typedef uint16_t 		USHORT;
typedef int16_t			SHORT;
typedef uint24_t		UINT24;
typedef uint32_t		ULONG;
typedef int32_t			LONG;
typedef uint32_t		Fixed;
#define FUNIT			1	/* Smallest measurable distance in em space */
typedef SHORT			FWORD;
typedef USHORT			UFWORD;
typedef uint64_t		LONGDATETIME;
typedef uint32_t		Tag;
typedef uint16_t		GlyphID;
typedef uint16_t		Offset;

/* Misc. types used - utility */
typedef const uint8_t*	ADDRESS;
typedef const char *	STR_ADDRESS;
typedef struct ConstStringInfo{
	STR_ADDRESS		PointerToString;
	uint32_t		VoidedValue;
}ConstStringInfo_t;

typedef struct ConstStringMatchingInfo{
	const ConstStringInfo_t * PointerToStringInfo;
	uint32_t			StringMatchingLength;
	uint32_t			NumStrings;
}ConstStringMatchingInfo_t;

/* OTF tables */
/* TODO fix constant numbers */
typedef enum OTFTable{
	cmap = (1 << 0),	/* Required */
	head = (1 << 1),	/* Required */
	hhea = (1 << 2),	/* Required */
	hmtx = (1 << 3),	/* Required */
	maxp = (1 << 4),	/* Required */
	name = (1 << 5),	/* Required */
	OS_2 = (1 << 6),	/* Required */
	post = (1 << 7),	/* Required */
	cvt_ = (1 << 8),
	fpgm = (1 << 9),
	glyf = (1 << 10),
	loca = (1 << 11),
	prep = (1 << 12),
	CFF_ = (1 << 13),
	VORG = (1 << 14),
	EBDT = (1 << 15),
	EBLC = (1 << 16),
	EBSC = (1 << 17),
	BASE = (1 << 18),
	GDEF = (1 << 19),
	GPOS = (1 << 20),
	GSUB = (1 << 21),
	JSTF = (1 << 22),
	DSIG = (1 << 23),
	gasp = (1 << 24),
	hdmx = (1 << 25),
	kern = (1 << 26),
	LTSH = (1 << 27),
	PCLT = (1 << 28),
	VDMX = (1 << 29),
	vhea = (1 << 30),
	vmtx = (1 << 31),
	OTFTableUndefined = 0
}OTFTable_t;

#define OTF_TABLE_HEADER_cmap	"cmap"
#define OTF_TABLE_HEADER_head	"head"
#define OTF_TABLE_HEADER_hhea	"hhea"
#define OTF_TABLE_HEADER_hmtx	"hmtx"
#define OTF_TABLE_HEADER_maxp	"maxp"
#define OTF_TABLE_HEADER_name	"name"
#define OTF_TABLE_HEADER_OS_2	"OS/2"
#define OTF_TABLE_HEADER_post	"post"
#define OTF_TABLE_HEADER_cvt_	"cvt "
#define OTF_TABLE_HEADER_fpgm	"fpgm"
#define OTF_TABLE_HEADER_glyf	"glyf"
#define OTF_TABLE_HEADER_loca	"loca"
#define OTF_TABLE_HEADER_prep	"prep"
#define OTF_TABLE_HEADER_CFF_	"CFF "
#define OTF_TABLE_HEADER_VORG	"VORG"
#define OTF_TABLE_HEADER_EBDT	"EBDT"
#define OTF_TABLE_HEADER_EBLC	"EBLC"
#define OTF_TABLE_HEADER_EBSC	"EBSC"
#define OTF_TABLE_HEADER_BASE	"BASE"
#define OTF_TABLE_HEADER_GDEF	"GDEF"
#define OTF_TABLE_HEADER_GPOS	"GPOS"
#define OTF_TABLE_HEADER_GSUB	"GSUB"
#define OTF_TABLE_HEADER_JSTF	"JSTF"
#define OTF_TABLE_HEADER_DSIG	"DSIG"
#define OTF_TABLE_HEADER_gasp	"gasp"
#define OTF_TABLE_HEADER_hdmx	"hdmx"
#define OTF_TABLE_HEADER_kern	"kern"
#define OTF_TABLE_HEADER_LTSH	"LTSH"
#define OTF_TABLE_HEADER_PCLT	"PCLT"
#define OTF_TABLE_HEADER_VDMX	"VDMX"
#define OTF_TABLE_HEADER_vhea	"vhea"
#define OTF_TABLE_HEADER_vmtx	"vmtx"

#define NUM_OTF_TABLES		32
#define NUM_OTF_FILE_TYPES	2


#define OTF_SFNT_VERSION_OTTO	"OTTO"
#define OTF_SFNT_VERSION_TTF	"\0\1\0\0"

typedef enum OTFOutlineType{
	CFFdata = 0,
	TrueTypeOutlines = 1,
	UndefinedFontType
}OTFOutlineType_t;

typedef struct OTFTableHeader{
	ULONG				OTFSubTableChecksum;
	ULONG				OTFSubTableOffset;
	ULONG				OTFSubTableLength;
}OTFTableHeader_t;

typedef struct OTFHeader{
	OTFOutlineType_t	ContainedFontOutlineType;
	USHORT				numTables;
	USHORT				searchRange;
	USHORT				entrySelector;
	USHORT				rangeShift;
}OTFHeader_t;

typedef struct OTFontInformation{
	OTFHeader_t			FontHeader;
	OTFTable_t			AvailableTables;
	OTFTableHeader_t	FontTableInformation[NUM_OTF_TABLES];
}OTFontInformation_t;

typedef struct OTFcmapSubtable{
	USHORT				cmapPlatformID;
	USHORT				cmapEncodingID;
	ULONG				cmapSubtableOffset;
	USHORT				cmapSubtableFormat;
	USHORT				cmapSubtableLength;
}OTFcmapSubtable_t;

typedef struct OTFcmapMainTable{
	USHORT				cmapVersion;
	USHORT				cmapNumSubtables;
	OTFcmapSubtable_t *	cmapSubtables;
}OTFcmapMainTable_t;





void init_otf_rasterizer(void);

bool can_render_char(char);

uint32_t MatchString(const ConstStringMatchingInfo_t *, ADDRESS, uint32_t *);

void load_font(ADDRESS, OTFontInformation_t *);

int get_OTFTable_Offset_Index(OTFTable_t);

#endif


