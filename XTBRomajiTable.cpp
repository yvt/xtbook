/*
 *  XTBRomajiTable.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/17/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include "XTBRomajiTable.h"

static XTBRomajiTable *g_defaultRomajiTable;

struct XTBRomajiTableEntry{
	wchar_t inputString[64];
	wchar_t convertString[64];
	bool weak;
};

class XTBRomajiStaticTable : public XTBRomajiTable{
protected:
	const XTBRomajiTableEntry *m_entries;
	int m_entryCount;
public:
	XTBRomajiStaticTable(const XTBRomajiTableEntry *entries, int entryCount){
		m_entries=entries;
		m_entryCount=entryCount;
	}
	virtual std::wstring convert(const std::wstring& str, bool strong){
		// this function should be optimized well.
		size_t strLength=str.size();
		if(strLength==0)
			return str;
	
		for(int i=0;i<m_entryCount;i++){
			int j;
			const XTBRomajiTableEntry& entry=m_entries[i];
			
			if(entry.weak && (!strong))
				continue;
			
			size_t entryLen=wcslen(entry.inputString);
			if(entryLen>strLength)
				continue;
			
			for(j=0;j<entryLen;j++){
				if(str[strLength-entryLen+j]!=entry.inputString[j])
					break;
			}
			
			if(j!=entryLen)
				continue;
			
			if(strong)
				return str.substr(0, strLength-entryLen)+std::wstring(entry.convertString);
			else
				return convert(str.substr(0, strLength-entryLen), true)+std::wstring(entry.convertString);
		}
		
		return str;
	}
};

XTBRomajiTable::XTBRomajiTable(){
}
XTBRomajiTable::~XTBRomajiTable(){
}


static XTBRomajiTableEntry tableEntries[]={
	{L"vya", {0x3094,0x3083,0}, false},
	{L"vyi", {0x3094,0x3043,0}, false},
	{L"vyu", {0x3094,0x3085,0}, false},
	{L"vye", {0x3094,0x3047,0}, false},
	{L"vyo", {0x3094,0x3087,0}, false},
	{L"wwa", {0x3063,0x308f,0}, false},
	{L"wwi", {0x3063,0x3046,0x3043,0}, false},
	{L"wwu", {0x3063,0x3046,0}, false},
	{L"wwe", {0x3063,0x3046,0x3047,0}, false},
	{L"wwo", {0x3063,0x3092,0}, false},
	{L"wwyi", {0x3063,0x3090,0}, false},
	{L"wwye", {0x3063,0x3091,0}, false},
	{L"wwha", {0x3063,0x3046,0x3041,0}, false},
	{L"wwhi", {0x3063,0x3046,0x3043,0}, false},
	{L"wwhu", {0x3063,0x3046,0}, false},
	{L"wwhe", {0x3063,0x3046,0x3047,0}, false},
	{L"wwho", {0x3063,0x3046,0x3049,0}, false},
	{L"kya", {0x304d,0x3083,0}, false},
	{L"kyi", {0x304d,0x3043,0}, false},
	{L"kyu", {0x304d,0x3085,0}, false},
	{L"kye", {0x304d,0x3047,0}, false},
	{L"kyo", {0x304d,0x3087,0}, false},
	{L"gya", {0x304e,0x3083,0}, false},
	{L"gyi", {0x304e,0x3043,0}, false},
	{L"gyu", {0x304e,0x3085,0}, false},
	{L"gye", {0x304e,0x3047,0}, false},
	{L"gyo", {0x304e,0x3087,0}, false},
	{L"sya", {0x3057,0x3083,0}, false},
	{L"syi", {0x3057,0x3043,0}, false},
	{L"syu", {0x3057,0x3085,0}, false},
	{L"sye", {0x3057,0x3047,0}, false},
	{L"syo", {0x3057,0x3087,0}, false},
	{L"sha", {0x3057,0x3083,0}, false},
	{L"shi", {0x3057,0}, false},
	{L"shu", {0x3057,0x3085,0}, false},
	{L"she", {0x3057,0x3047,0}, false},
	{L"sho", {0x3057,0x3087,0}, false},
	{L"zya", {0x3058,0x3083,0}, false},
	{L"zyi", {0x3058,0x3043,0}, false},
	{L"zyu", {0x3058,0x3085,0}, false},
	{L"zye", {0x3058,0x3047,0}, false},
	{L"zyo", {0x3058,0x3087,0}, false},
	{L"tya", {0x3061,0x3083,0}, false},
	{L"tyi", {0x3061,0x3043,0}, false},
	{L"tyu", {0x3061,0x3085,0}, false},
	{L"tye", {0x3061,0x3047,0}, false},
	{L"tyo", {0x3061,0x3087,0}, false},
	{L"cha", {0x3061,0x3083,0}, false},
	{L"chi", {0x3061,0}, false},
	{L"chu", {0x3061,0x3085,0}, false},
	{L"che", {0x3061,0x3047,0}, false},
	{L"cho", {0x3061,0x3087,0}, false},
	{L"cya", {0x3061,0x3083,0}, false},
	{L"cyi", {0x3061,0x3043,0}, false},
	{L"cyu", {0x3061,0x3085,0}, false},
	{L"cye", {0x3061,0x3047,0}, false},
	{L"cyo", {0x3061,0x3087,0}, false},
	{L"dya", {0x3062,0x3083,0}, false},
	{L"dyi", {0x3062,0x3043,0}, false},
	{L"dyu", {0x3062,0x3085,0}, false},
	{L"dye", {0x3062,0x3047,0}, false},
	{L"dyo", {0x3062,0x3087,0}, false},
	{L"tsa", {0x3064,0x3041,0}, false},
	{L"tsi", {0x3064,0x3043,0}, false},
	{L"tse", {0x3064,0x3047,0}, false},
	{L"tso", {0x3064,0x3049,0}, false},
	{L"tha", {0x3066,0x3083,0}, false},
	{L"thi", {0x3066,0x3043,0}, false},
	{L"t'i", {0x3066,0x3043,0}, false},
	{L"thu", {0x3066,0x3085,0}, false},
	{L"the", {0x3066,0x3047,0}, false},
	{L"tho", {0x3066,0x3087,0}, false},
	{L"t'yu", {0x3066,0x3085,0}, false},
	{L"dha", {0x3067,0x3083,0}, false},
	{L"dhi", {0x3067,0x3043,0}, false},
	{L"d'i", {0x3067,0x3043,0}, false},
	{L"dhu", {0x3067,0x3085,0}, false},
	{L"dhe", {0x3067,0x3047,0}, false},
	{L"dho", {0x3067,0x3087,0}, false},
	{L"d'yu", {0x3067,0x3085,0}, false},
	{L"twa", {0x3068,0x3041,0}, false},
	{L"twi", {0x3068,0x3043,0}, false},
	{L"twu", {0x3068,0x3045,0}, false},
	{L"twe", {0x3068,0x3047,0}, false},
	{L"two", {0x3068,0x3049,0}, false},
	{L"t'u", {0x3068,0x3045,0}, false},
	{L"dwa", {0x3069,0x3041,0}, false},
	{L"dwi", {0x3069,0x3043,0}, false},
	{L"dwu", {0x3069,0x3045,0}, false},
	{L"dwe", {0x3069,0x3047,0}, false},
	{L"dwo", {0x3069,0x3049,0}, false},
	{L"d'u", {0x3069,0x3045,0}, false},
	{L"nya", {0x306b,0x3083,0}, false},
	{L"nyi", {0x306b,0x3043,0}, false},
	{L"nyu", {0x306b,0x3085,0}, false},
	{L"nye", {0x306b,0x3047,0}, false},
	{L"nyo", {0x306b,0x3087,0}, false},
	{L"hya", {0x3072,0x3083,0}, false},
	{L"hyi", {0x3072,0x3043,0}, false},
	{L"hyu", {0x3072,0x3085,0}, false},
	{L"hye", {0x3072,0x3047,0}, false},
	{L"hyo", {0x3072,0x3087,0}, false},
	{L"bya", {0x3073,0x3083,0}, false},
	{L"byi", {0x3073,0x3043,0}, false},
	{L"byu", {0x3073,0x3085,0}, false},
	{L"bye", {0x3073,0x3047,0}, false},
	{L"byo", {0x3073,0x3087,0}, false},
	{L"pya", {0x3074,0x3083,0}, false},
	{L"pyi", {0x3074,0x3043,0}, false},
	{L"pyu", {0x3074,0x3085,0}, false},
	{L"pye", {0x3074,0x3047,0}, false},
	{L"pyo", {0x3074,0x3087,0}, false},
	{L"fya", {0x3075,0x3083,0}, false},
	{L"fyu", {0x3075,0x3085,0}, false},
	{L"fyo", {0x3075,0x3087,0}, false},
	{L"hwa", {0x3075,0x3041,0}, false},
	{L"hwi", {0x3075,0x3043,0}, false},
	{L"hwe", {0x3075,0x3047,0}, false},
	{L"hwo", {0x3075,0x3049,0}, false},
	{L"hwyu", {0x3075,0x3085,0}, false},
	{L"mya", {0x307f,0x3083,0}, false},
	{L"myi", {0x307f,0x3043,0}, false},
	{L"myu", {0x307f,0x3085,0}, false},
	{L"mye", {0x307f,0x3047,0}, false},
	{L"myo", {0x307f,0x3087,0}, false},
	{L"jya", {0x3058,0x3083,0}, false},
	{L"jyi", {0x3058,0x3043,0}, false},
	{L"jyu", {0x3058,0x3085,0}, false},
	{L"jye", {0x3058,0x3047,0}, false},
	{L"jyo", {0x3058,0x3087,0}, false},
	{L"rya", {0x308a,0x3083,0}, false},
	{L"ryi", {0x308a,0x3043,0}, false},
	{L"ryu", {0x308a,0x3085,0}, false},
	{L"rye", {0x308a,0x3047,0}, false},
	{L"ryo", {0x308a,0x3087,0}, false},
	{L"xka", {0x30f5,0}, false},
	{L"xke", {0x30f6,0}, false},
	{L"lka", {0x30f5,0}, false},
	{L"lke", {0x30f6,0}, false},
	{L"lyi", {0x3043,0}, false},
	{L"xyi", {0x3043,0}, false},
	{L"lye", {0x3047,0}, false},
	{L"xye", {0x3047,0}, false},
	{L"kwa", {0x304f,0x3041,0}, false},
	{L"kwi", {0x304f,0x3043,0}, false},
	{L"kwe", {0x304f,0x3047,0}, false},
	{L"kwo", {0x304f,0x3049,0}, false},
	{L"gwa", {0x3050,0x3041,0}, false},
	{L"xtu", {0x3063,0}, false},
	{L"xtsu", {0x3063,0}, false},
	{L"ltu", {0x3063,0}, false},
	{L"ltsu", {0x3063,0}, false},
	{L"wha", {0x3046,0x3041,0}, false},
	{L"whi", {0x3046,0x3043,0}, false},
	{L"whu", {0x3046,0}, false},
	{L"whe", {0x3046,0x3047,0}, false},
	{L"who", {0x3046,0x3049,0}, false},
	{L"xwa", {0x308e,0}, false},
	{L"lwa", {0x308e,0}, false},
	{L"xya", {0x3083,0}, false},
	{L"lya", {0x3083,0}, false},
	{L"wye", {0x3091,0}, false},
	{L"xyo", {0x3087,0}, false},
	{L"lyo", {0x3087,0}, false},
	{L"wyi", {0x3090,0}, false},
	{L"xyu", {0x3085,0}, false},
	{L"lyu", {0x3085,0}, false},
	{L"tsu", {0x3064,0}, false},
	{L"z/", {0x30fb,0}, false},
	{L"z.", {0x2026,0}, false},
	{L"z,", {0x2025,0}, false},
	{L"zh", {0x2190,0}, false},
	{L"zj", {0x2193,0}, false},
	{L"zk", {0x2191,0}, false},
	{L"zl", {0x2192,0}, false},
	{L"z-", {0x301c,0}, false},
	{L"z[", {0x300e,0}, false},
	{L"z]", {0x300f,0}, false},
	{L"[", {0x300c,0}, false},
	{L"]", {0x300d,0}, false},
	{L"va", {0x3094,0x3041,0}, false},
	{L"vi", {0x3094,0x3043,0}, false},
	{L"vu", {0x3094,0}, false},
	{L"ve", {0x3094,0x3047,0}, false},
	{L"vo", {0x3094,0x3049,0}, false},
	{L"qq", {0x3063,0x71,0}, false},
	{L"vv", {0x3063,0x76,0}, false},
	{L"ll", {0x3063,0x6c,0}, false},
	{L"xx", {0x3063,0x78,0}, false},
	{L"kk", {0x3063,0x6b,0}, false},
	{L"gg", {0x3063,0x67,0}, false},
	{L"ss", {0x3063,0x73,0}, false},
	{L"zz", {0x3063,0x7a,0}, false},
	{L"jj", {0x3063,0x6a,0}, false},
	{L"tt", {0x3063,0x74,0}, false},
	{L"dd", {0x3063,0x64,0}, false},
	{L"hh", {0x3063,0x68,0}, false},
	{L"ff", {0x3063,0x66,0}, false},
	{L"bb", {0x3063,0x62,0}, false},
	{L"pp", {0x3063,0x70,0}, false},
	{L"mm", {0x3063,0x6d,0}, false},
	{L"yy", {0x3063,0x79,0}, false},
	{L"rr", {0x3063,0x72,0}, false},
	{L"fa", {0x3075,0x3041,0}, false},
	{L"fi", {0x3075,0x3043,0}, false},
	{L"fu", {0x3075,0}, false},
	{L"fe", {0x3075,0x3047,0}, false},
	{L"fo", {0x3075,0x3049,0}, false},
	{L"nn", {0x3093,0}, false},
	{L"xn", {0x3093,0}, false},
	{L"xa", {0x3041,0}, false},
	{L"xi", {0x3043,0}, false},
	{L"xu", {0x3045,0}, false},
	{L"xe", {0x3047,0}, false},
	{L"xo", {0x3049,0}, false},
	{L"la", {0x3041,0}, false},
	{L"li", {0x3043,0}, false},
	{L"lu", {0x3045,0}, false},
	{L"le", {0x3047,0}, false},
	{L"lo", {0x3049,0}, false},
	{L"ye", {0x3044,0x3047,0}, false},
	{L"ka", {0x304b,0}, false},
	{L"ki", {0x304d,0}, false},
	{L"ku", {0x304f,0}, false},
	{L"ke", {0x3051,0}, false},
	{L"ko", {0x3053,0}, false},
	{L"ga", {0x304c,0}, false},
	{L"gi", {0x304e,0}, false},
	{L"gu", {0x3050,0}, false},
	{L"ge", {0x3052,0}, false},
	{L"go", {0x3054,0}, false},
	{L"sa", {0x3055,0}, false},
	{L"si", {0x3057,0}, false},
	{L"su", {0x3059,0}, false},
	{L"se", {0x305b,0}, false},
	{L"so", {0x305d,0}, false},
	{L"ca", {0x304b,0}, false},
	{L"ci", {0x3057,0}, false},
	{L"cu", {0x304f,0}, false},
	{L"ce", {0x305b,0}, false},
	{L"co", {0x3053,0}, false},
	{L"qa", {0x304f,0x3041,0}, false},
	{L"qi", {0x304f,0x3043,0}, false},
	{L"qu", {0x304f,0}, false},
	{L"qe", {0x304f,0x3047,0}, false},
	{L"qo", {0x304f,0x3049,0}, false},
	{L"za", {0x3056,0}, false},
	{L"zi", {0x3058,0}, false},
	{L"zu", {0x305a,0}, false},
	{L"ze", {0x305c,0}, false},
	{L"zo", {0x305e,0}, false},
	{L"ja", {0x3058,0x3083,0}, false},
	{L"ji", {0x3058,0}, false},
	{L"ju", {0x3058,0x3085,0}, false},
	{L"je", {0x3058,0x3047,0}, false},
	{L"jo", {0x3058,0x3087,0}, false},
	{L"ta", {0x305f,0}, false},
	{L"ti", {0x3061,0}, false},
	{L"tu", {0x3064,0}, false},
	{L"te", {0x3066,0}, false},
	{L"to", {0x3068,0}, false},
	{L"da", {0x3060,0}, false},
	{L"di", {0x3062,0}, false},
	{L"du", {0x3065,0}, false},
	{L"de", {0x3067,0}, false},
	{L"do", {0x3069,0}, false},
	{L"na", {0x306a,0}, false},
	{L"ni", {0x306b,0}, false},
	{L"nu", {0x306c,0}, false},
	{L"ne", {0x306d,0}, false},
	{L"no", {0x306e,0}, false},
	{L"ha", {0x306f,0}, false},
	{L"hi", {0x3072,0}, false},
	{L"hu", {0x3075,0}, false},
	{L"fu", {0x3075,0}, false},
	{L"he", {0x3078,0}, false},
	{L"ho", {0x307b,0}, false},
	{L"ba", {0x3070,0}, false},
	{L"bi", {0x3073,0}, false},
	{L"bu", {0x3076,0}, false},
	{L"be", {0x3079,0}, false},
	{L"bo", {0x307c,0}, false},
	{L"pa", {0x3071,0}, false},
	{L"pi", {0x3074,0}, false},
	{L"pu", {0x3077,0}, false},
	{L"pe", {0x307a,0}, false},
	{L"po", {0x307d,0}, false},
	{L"ma", {0x307e,0}, false},
	{L"mi", {0x307f,0}, false},
	{L"mu", {0x3080,0}, false},
	{L"me", {0x3081,0}, false},
	{L"mo", {0x3082,0}, false},
	{L"ya", {0x3084,0}, false},
	{L"yu", {0x3086,0}, false},
	{L"yo", {0x3088,0}, false},
	{L"ra", {0x3089,0}, false},
	{L"ri", {0x308a,0}, false},
	{L"ru", {0x308b,0}, false},
	{L"re", {0x308c,0}, false},
	{L"ro", {0x308d,0}, false},
	{L"wa", {0x308f,0}, false},
	{L"wi", {0x3046,0x3043,0}, false},
	{L"we", {0x3046,0x3047,0}, false},
	{L"wo", {0x3092,0}, false},
	{L"cc", {0x3063,0x63,0}, false},
	{L"a", {0x3042,0}, false},
	{L"i", {0x3044,0}, false},
	{L"u", {0x3046,0}, false},
	{L"wu", {0x3046,0}, false},
	{L"e", {0x3048,0}, false},
	{L"o", {0x304a,0}, false},
	{L"n'", {0x3093,0}, false},
	{L"n", {0x3093,0}, true},
	{L"-", {0x30fc,0}, false},
	{L"~", {0x301c,0}, false},
	{L".", {0x3002,0}, false},
	{L",", {0x3001,0}, false}
};

XTBRomajiTable *XTBRomajiTable::defaultRomajiTable(){
	if(g_defaultRomajiTable==NULL){
		g_defaultRomajiTable=new XTBRomajiStaticTable(tableEntries, 
													  sizeof(tableEntries)/sizeof(XTBRomajiTableEntry));
	}
	return g_defaultRomajiTable;
}




