/* OTF file format rasterizer */

#include "OTFRasterizer.h"
#include "SystemFonts.h"



const ConstStringInfo_t OTF_File_Header_Strings[NUM_OTF_FILE_TYPES] = {
		{
				(const char *)& OTF_SFNT_VERSION_OTTO[0],
				(uint32_t) CFFdata
		},
		{
				(const char *)& OTF_SFNT_VERSION_TTF[0],
				(uint32_t) TrueTypeOutlines
		}
};

const ConstStringInfo_t OTF_Table_Header_Strings[NUM_OTF_TABLES] = {
		{
				(const char *)&OTF_TABLE_HEADER_cmap[0],
				(uint32_t) cmap
		},
		{
				(const char *)&OTF_TABLE_HEADER_head[0],
				(uint32_t) head
		},
		{
				(const char *)&OTF_TABLE_HEADER_hhea[0],
				(uint32_t) hhea
		},
		{
				(const char *)&OTF_TABLE_HEADER_hmtx[0],
				(uint32_t) hmtx
		},
		{
				(const char *)&OTF_TABLE_HEADER_maxp[0],
				(uint32_t) maxp
		},
		{
				(const char *)&OTF_TABLE_HEADER_name[0],
				(uint32_t) name
		},
		{
				(const char *)&OTF_TABLE_HEADER_OS_2[0],
				(uint32_t) OS_2
		},
		{
				(const char *)&OTF_TABLE_HEADER_post[0],
				(uint32_t) post
		},
		{
				(const char *)&OTF_TABLE_HEADER_cvt_[0],
				(uint32_t) cvt_
		},
		{
				(const char *)&OTF_TABLE_HEADER_fpgm[0],
				(uint32_t) fpgm
		},
		{
				(const char *)&OTF_TABLE_HEADER_glyf[0],
				(uint32_t) glyf
		},
		{
				(const char *)&OTF_TABLE_HEADER_loca[0],
				(uint32_t) loca
		},
		{
				(const char *)&OTF_TABLE_HEADER_prep[0],
				(uint32_t) prep
		},
		{
				(const char *)&OTF_TABLE_HEADER_CFF_[0],
				(uint32_t) CFF_
		},
		{
				(const char *)&OTF_TABLE_HEADER_VORG[0],
				(uint32_t) VORG
		},
		{
				(const char *)&OTF_TABLE_HEADER_EBDT[0],
				(uint32_t) EBDT
		},
		{
				(const char *)&OTF_TABLE_HEADER_EBLC[0],
				(uint32_t) EBLC
		},
		{
				(const char *)&OTF_TABLE_HEADER_EBSC[0],
				(uint32_t) EBSC
		},
		{
				(const char *)&OTF_TABLE_HEADER_BASE[0],
				(uint32_t) BASE
		},
		{
				(const char *)&OTF_TABLE_HEADER_GDEF[0],
				(uint32_t) GDEF
		},
		{
				(const char *)&OTF_TABLE_HEADER_GPOS[0],
				(uint32_t) GPOS
		},
		{
				(const char *)&OTF_TABLE_HEADER_GSUB[0],
				(uint32_t) GSUB
		},
		{
				(const char *)&OTF_TABLE_HEADER_JSTF[0],
				(uint32_t) JSTF
		},
		{
				(const char *)&OTF_TABLE_HEADER_DSIG[0],
				(uint32_t) DSIG
		},
		{
				(const char *)&OTF_TABLE_HEADER_gasp[0],
				(uint32_t) gasp
		},
		{
				(const char *)&OTF_TABLE_HEADER_hdmx[0],
				(uint32_t) hdmx
		},
		{
				(const char *)&OTF_TABLE_HEADER_kern[0],
				(uint32_t) kern
		},
		{
				(const char *)&OTF_TABLE_HEADER_LTSH[0],
				(uint32_t) LTSH
		},
		{
				(const char *)&OTF_TABLE_HEADER_PCLT[0],
				(uint32_t) PCLT
		},
		{
				(const char *)&OTF_TABLE_HEADER_VDMX[0],
				(uint32_t) VDMX
		},
		{
				(const char *)&OTF_TABLE_HEADER_vhea[0],
				(uint32_t) vhea
		},
		{
				(const char *)&OTF_TABLE_HEADER_vmtx[0],
				(uint32_t) vmtx
		},
};


const ConstStringMatchingInfo_t MatchingInfoForType = {
		&OTF_File_Header_Strings[0],
		4,
		NUM_OTF_FILE_TYPES
};

const ConstStringMatchingInfo_t MatchingInfoForOTFTable = {
		&OTF_Table_Header_Strings[0],
		4,
		NUM_OTF_TABLES
};


void init_otf_rasterizer(){
	OTFontInformation_t HelveticaLtdStdInfo = { 0 };
	load_font(&HelveticaLtStd_const[0], &HelveticaLtdStdInfo);
}


/* Check the character set to see if the character can be rendered in the font */
bool can_render_char(char char2render){
	return false;
}

uint32_t MatchString(const ConstStringMatchingInfo_t * MatchingInfoPtr, ADDRESS Ptr2Char, uint32_t * Ptr2Enum){
	int i = 0, j = 0;
	bool it = false;
	for(i = 0; i < MatchingInfoPtr->NumStrings; i++){
		it = true;
		for(j = 0; j < MatchingInfoPtr->StringMatchingLength; j++){
			if((*(Ptr2Char + j)) != (MatchingInfoPtr->PointerToStringInfo[i].PointerToString[j])){
				it = false;
				break;
			}
		}

		/* Entire thing matches */
		if(it){
			*(Ptr2Enum) = MatchingInfoPtr->PointerToStringInfo[i].VoidedValue;
			return MatchingInfoPtr->StringMatchingLength;
		}
	}
	return 0;
}

#define PARSEUSHORT(ptr2char, ushort2store)	ushort2store = (USHORT)((*(ptr2char) << 8) | *(ptr2char + 1)); ptr2char += 2;
#define PARSESHORT(ptr2char, short2store)	short2store = (SHORT)((*(ptr2char) << 8) | *(ptr2char + 1)); ptr2char += 2;
#define PARSEULONG(ptr2char, ulong2store)	ulong2store = (ULONG)((*(ptr2char) << 24) | (*(ptr2char + 1) << 16) | (*(ptr2char + 2) << 8) | *(ptr2char + 3)); ptr2char += 4;

void load_font(ADDRESS ptr2fontaddress, OTFontInformation_t * ptr2fontinfo){
	uint8_t * cur_ptr = ptr2fontaddress;
	uint32_t offset = 0;
	int i = 0, tempint;
	OTFTable_t temptblval;

	offset += MatchString(&MatchingInfoForType, cur_ptr, (uint32_t *)&ptr2fontinfo->FontHeader.ContainedFontOutlineType);

	cur_ptr += offset;

	PARSEUSHORT(cur_ptr, ptr2fontinfo->FontHeader.numTables);
	PARSEUSHORT(cur_ptr, ptr2fontinfo->FontHeader.searchRange);
	PARSEUSHORT(cur_ptr, ptr2fontinfo->FontHeader.entrySelector);
	PARSEUSHORT(cur_ptr, ptr2fontinfo->FontHeader.rangeShift);

	/* Start loading up the locations to the tables */
	for(i = 0; i < ptr2fontinfo->FontHeader.numTables; i++){
		/* Init scratch variables before they're used here (so no old data can be recycled) */
		temptblval = OTFTableUndefined;
		tempint = -1;

		offset = MatchString(&MatchingInfoForOTFTable, cur_ptr, (uint32_t *)&temptblval);
		if(offset > 0){
			cur_ptr += offset;
			tempint = get_OTFTable_Offset_Index(temptblval);
			if(tempint >= 0){
				PARSEULONG(cur_ptr, ptr2fontinfo->FontTableInformation[tempint].OTFSubTableChecksum);
				PARSEULONG(cur_ptr, ptr2fontinfo->FontTableInformation[tempint].OTFSubTableOffset);
				PARSEULONG(cur_ptr, ptr2fontinfo->FontTableInformation[tempint].OTFSubTableLength);
			}
			else
				break;	/* Error parsing font */
		}
		else
			break; /* Error parsing font */
	}


}


int get_OTFTable_Offset_Index(OTFTable_t otftable){
	switch(otftable){
	case cmap:
		return 0;
	case head:
		return 1;
	case hhea:
		return 2;
	case hmtx:
		return 3;
	case maxp:
		return 4;
	case name:
		return 5;
	case OS_2:
		return 6;
	case post:
		return 7;
	case cvt_:
		return 8;
	case fpgm:
		return 9;
	case glyf:
		return 10;
	case loca:
		return 11;
	case prep:
		return 12;
	case CFF_:
		return 13;
	case VORG:
		return 14;
	case EBDT:
		return 15;
	case EBLC:
		return 16;
	case EBSC:
		return 17;
	case BASE:
		return 18;
	case GDEF:
		return 19;
	case GPOS:
		return 20;
	case GSUB:
		return 21;
	case JSTF:
		return 22;
	case DSIG:
		return 23;
	case gasp:
		return 24;
	case hdmx:
		return 25;
	case kern:
		return 26;
	case LTSH:
		return 27;
	case PCLT:
		return 28;
	case VDMX:
		return 29;
	case vhea:
		return 30;
	case vmtx:
		return 31;
	default:
		return -1;
	}
}











