/*
 *  twSDLDCXPMLoader.cpp
 *  XTBook
 *
 *  Created by Nexhawks on 2/12/11.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */


/*
 SDL_image:  An example image loading library for use with SDL
 Copyright (C) 1997-2009 Sam Lantinga
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
 Sam Lantinga
 slouken@libsdl.org
 */

/*
 * XPM (X PixMap) image loader:
 *
 * Supports the XPMv3 format, EXCEPT:
 * - hotspot coordinates are ignored
 * - only colour ('c') colour symbols are used
 * - rgb.txt is not used (for portability), so only RGB colours
 *   are recognized (#rrggbb etc) - only a few basic colour names are
 *   handled
 *
 * The result is an 8bpp indexed surface if possible, otherwise 32bpp.
 * The colourkey is correctly set if transparency is used.
 * 
 * Besides the standard API, also provides
 *
 *     SDL_Surface *IMG_ReadXPMFromArray(char **xpm)
 *
 * that reads the image data from an XPM file included in the C source.
 *
 * TODO: include rgb.txt here. The full table (from solaris 2.6) only
 * requires about 13K in binary form.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <tcw/twSDLDC.h>


/* Hash table to look up colors from pixel strings */
#define STARTING_HASH_SIZE 256

struct hash_entry {
	char *key;
	Uint32 color;
	struct hash_entry *next;
};

struct color_hash {
	struct hash_entry **table;
	struct hash_entry *entries; /* array of all entries */
	struct hash_entry *next_free;
	int size;
	int maxnum;
};

static int hash_key(const char *key, int cpp, int size)
{
	int hash;
	
	hash = 0;
	while ( cpp-- > 0 ) {
		hash = hash * 33 + *key++;
	}
	return hash & (size - 1);
}

static struct color_hash *create_colorhash(int maxnum)
{
	int bytes, s;
	struct color_hash *hash;
	
	/* we know how many entries we need, so we can allocate
	 everything here */
	hash = (color_hash *)malloc(sizeof *hash);
	if(!hash)
		return NULL;
	
	/* use power-of-2 sized hash table for decoding speed */
	for(s = STARTING_HASH_SIZE; s < maxnum; s <<= 1)
		;
	hash->size = s;
	hash->maxnum = maxnum;
	bytes = hash->size * sizeof(struct hash_entry **);
	hash->entries = NULL;	/* in case malloc fails */
	hash->table = (struct hash_entry **)malloc(bytes);
	if(!hash->table)
		return NULL;
	memset(hash->table, 0, bytes);
	hash->entries = (struct hash_entry *)malloc(maxnum * sizeof(struct hash_entry));
	if(!hash->entries)
		return NULL;
	hash->next_free = hash->entries;
	return hash;
}

static int add_colorhash(struct color_hash *hash,
                         char *key, int cpp, Uint32 color)
{
	int index = hash_key(key, cpp, hash->size);
	struct hash_entry *e = hash->next_free++;
	e->color = color;
	e->key = key;
	e->next = hash->table[index];
	hash->table[index] = e;
	return 1;
}

/* fast lookup that works if cpp == 1 */
#define QUICK_COLORHASH(hash, key) ((hash)->table[*(Uint8 *)(key)]->color)

static Uint32 get_colorhash(struct color_hash *hash, const char *key, int cpp)
{
	struct hash_entry *entry = hash->table[hash_key(key, cpp, hash->size)];
	while(entry) {
		if(memcmp(key, entry->key, cpp) == 0)
			return entry->color;
		entry = entry->next;
	}
	return 0;		/* garbage in - garbage out */
}

static void free_colorhash(struct color_hash *hash)
{
	if(hash && hash->table) {
		free(hash->table);
		free(hash->entries);
		free(hash);
	}
}

/* portable case-insensitive string comparison */
static int string_equal(const char *a, const char *b, int n)
{
	while(*a && *b && n) {
		if(toupper((unsigned char)*a) != toupper((unsigned char)*b))
			return 0;
		a++;
		b++;
		n--;
	}
	return *a == *b;
}

#define ARRAYSIZE(a) (int)(sizeof(a) / sizeof((a)[0]))

/*
 * convert colour spec to RGB (in 0xrrggbb format).
 * return 1 if successful.
 */
static int color_to_rgb(char *spec, int speclen, Uint32 *rgb)
{
	/* poor man's rgb.txt */
	static struct { const char *name; Uint32 rgb; } known[] = {
		{"none",  0xffffffff},
		{"black", 0x00000000},
		{"white", 0x00ffffff},
		{"red",   0x00ff0000},
		{"green", 0x0000ff00},
		{"blue",  0x000000ff},
		{"snow", 0x00fffafa},
		{"GhostWhite", 0x00f8f8ff},
		{"WhiteSmoke", 0x00f5f5f5},
		{"gainsboro", 0x00dcdcdc},
		{"FloralWhite", 0x00fffaf0},
		{"OldLace", 0x00fdf5e6},
		{"linen", 0x00faf0e6},
		{"AntiqueWhite", 0x00faebd7},
		{"PapayaWhip", 0x00ffefd5},
		{"BlanchedAlmond", 0x00ffebcd},
		{"bisque", 0x00ffe4c4},
		{"PeachPuff", 0x00ffdab9},
		{"NavajoWhite", 0x00ffdead},
		{"moccasin", 0x00ffe4b5},
		{"cornsilk", 0x00fff8dc},
		{"ivory", 0x00fffff0},
		{"LemonChiffon", 0x00fffacd},
		{"seashell", 0x00fff5ee},
		{"honeydew", 0x00f0fff0},
		{"MintCream", 0x00f5fffa},
		{"azure", 0x00f0ffff},
		{"AliceBlue", 0x00f0f8ff},
		{"lavender", 0x00e6e6fa},
		{"LavenderBlush", 0x00fff0f5},
		{"MistyRose", 0x00ffe4e1},
		{"white", 0x00ffffff},
		{"black", 0x00000000},
		{"DarkSlateGray", 0x002f4f4f},
		{"DarkSlateGrey", 0x002f4f4f},
		{"DimGray", 0x00696969},
		{"DimGrey", 0x00696969},
		{"SlateGray", 0x00708090},
		{"SlateGrey", 0x00708090},
		{"LightSlateGray", 0x00778899},
		{"LightSlateGrey", 0x00778899},
		{"gray", 0x00bebebe},
		{"grey", 0x00bebebe},
		{"LightGrey", 0x00d3d3d3},
		{"LightGray", 0x00d3d3d3},
		{"MidnightBlue", 0x00191970},
		{"navy", 0x00000080},
		{"NavyBlue", 0x00000080},
		{"CornflowerBlue", 0x006495ed},
		{"DarkSlateBlue", 0x00483d8b},
		{"SlateBlue", 0x006a5acd},
		{"MediumSlateBlue", 0x007b68ee},
		{"LightSlateBlue", 0x008470ff},
		{"MediumBlue", 0x000000cd},
		{"RoyalBlue", 0x004169e1},
		{"blue", 0x000000ff},
		{"DodgerBlue", 0x001e90ff},
		{"DeepSkyBlue", 0x0000bfff},
		{"SkyBlue", 0x0087ceeb},
		{"LightSkyBlue", 0x0087cefa},
		{"SteelBlue", 0x004682b4},
		{"LightSteelBlue", 0x00b0c4de},
		{"LightBlue", 0x00add8e6},
		{"PowderBlue", 0x00b0e0e6},
		{"PaleTurquoise", 0x00afeeee},
		{"DarkTurquoise", 0x0000ced1},
		{"MediumTurquoise", 0x0048d1cc},
		{"turquoise", 0x0040e0d0},
		{"cyan", 0x0000ffff},
		{"LightCyan", 0x00e0ffff},
		{"CadetBlue", 0x005f9ea0},
		{"MediumAquamarine", 0x0066cdaa},
		{"aquamarine", 0x007fffd4},
		{"DarkGreen", 0x00006400},
		{"DarkOliveGreen", 0x00556b2f},
		{"DarkSeaGreen", 0x008fbc8f},
		{"SeaGreen", 0x002e8b57},
		{"MediumSeaGreen", 0x003cb371},
		{"LightSeaGreen", 0x0020b2aa},
		{"PaleGreen", 0x0098fb98},
		{"SpringGreen", 0x0000ff7f},
		{"LawnGreen", 0x007cfc00},
		{"green", 0x0000ff00},
		{"chartreuse", 0x007fff00},
		{"MediumSpringGreen", 0x0000fa9a},
		{"GreenYellow", 0x00adff2f},
		{"LimeGreen", 0x0032cd32},
		{"YellowGreen", 0x009acd32},
		{"ForestGreen", 0x00228b22},
		{"OliveDrab", 0x006b8e23},
		{"DarkKhaki", 0x00bdb76b},
		{"khaki", 0x00f0e68c},
		{"PaleGoldenrod", 0x00eee8aa},
		{"LightGoldenrodYellow", 0x00fafad2},
		{"LightYellow", 0x00ffffe0},
		{"yellow", 0x00ffff00},
		{"gold", 0x00ffd700},
		{"LightGoldenrod", 0x00eedd82},
		{"goldenrod", 0x00daa520},
		{"DarkGoldenrod", 0x00b8860b},
		{"RosyBrown", 0x00bc8f8f},
		{"IndianRed", 0x00cd5c5c},
		{"SaddleBrown", 0x008b4513},
		{"sienna", 0x00a0522d},
		{"peru", 0x00cd853f},
		{"burlywood", 0x00deb887},
		{"beige", 0x00f5f5dc},
		{"wheat", 0x00f5deb3},
		{"SandyBrown", 0x00f4a460},
		{"tan", 0x00d2b48c},
		{"chocolate", 0x00d2691e},
		{"firebrick", 0x00b22222},
		{"brown", 0x00a52a2a},
		{"DarkSalmon", 0x00e9967a},
		{"salmon", 0x00fa8072},
		{"LightSalmon", 0x00ffa07a},
		{"orange", 0x00ffa500},
		{"DarkOrange", 0x00ff8c00},
		{"coral", 0x00ff7f50},
		{"LightCoral", 0x00f08080},
		{"tomato", 0x00ff6347},
		{"OrangeRed", 0x00ff4500},
		{"red", 0x00ff0000},
		{"HotPink", 0x00ff69b4},
		{"DeepPink", 0x00ff1493},
		{"pink", 0x00ffc0cb},
		{"LightPink", 0x00ffb6c1},
		{"PaleVioletRed", 0x00db7093},
		{"maroon", 0x00b03060},
		{"MediumVioletRed", 0x00c71585},
		{"VioletRed", 0x00d02090},
		{"magenta", 0x00ff00ff},
		{"violet", 0x00ee82ee},
		{"plum", 0x00dda0dd},
		{"orchid", 0x00da70d6},
		{"MediumOrchid", 0x00ba55d3},
		{"DarkOrchid", 0x009932cc},
		{"DarkViolet", 0x009400d3},
		{"BlueViolet", 0x008a2be2},
		{"purple", 0x00a020f0},
		{"MediumPurple", 0x009370db},
		{"thistle", 0x00d8bfd8},
		{"snow1", 0x00fffafa},
		{"snow2", 0x00eee9e9},
		{"snow3", 0x00cdc9c9},
		{"snow4", 0x008b8989},
		{"seashell1", 0x00fff5ee},
		{"seashell2", 0x00eee5de},
		{"seashell3", 0x00cdc5bf},
		{"seashell4", 0x008b8682},
		{"AntiqueWhite1", 0x00ffefdb},
		{"AntiqueWhite2", 0x00eedfcc},
		{"AntiqueWhite3", 0x00cdc0b0},
		{"AntiqueWhite4", 0x008b8378},
		{"bisque1", 0x00ffe4c4},
		{"bisque2", 0x00eed5b7},
		{"bisque3", 0x00cdb79e},
		{"bisque4", 0x008b7d6b},
		{"PeachPuff1", 0x00ffdab9},
		{"PeachPuff2", 0x00eecbad},
		{"PeachPuff3", 0x00cdaf95},
		{"PeachPuff4", 0x008b7765},
		{"NavajoWhite1", 0x00ffdead},
		{"NavajoWhite2", 0x00eecfa1},
		{"NavajoWhite3", 0x00cdb38b},
		{"NavajoWhite4", 0x008b795e},
		{"LemonChiffon1", 0x00fffacd},
		{"LemonChiffon2", 0x00eee9bf},
		{"LemonChiffon3", 0x00cdc9a5},
		{"LemonChiffon4", 0x008b8970},
		{"cornsilk1", 0x00fff8dc},
		{"cornsilk2", 0x00eee8cd},
		{"cornsilk3", 0x00cdc8b1},
		{"cornsilk4", 0x008b8878},
		{"ivory1", 0x00fffff0},
		{"ivory2", 0x00eeeee0},
		{"ivory3", 0x00cdcdc1},
		{"ivory4", 0x008b8b83},
		{"honeydew1", 0x00f0fff0},
		{"honeydew2", 0x00e0eee0},
		{"honeydew3", 0x00c1cdc1},
		{"honeydew4", 0x00838b83},
		{"LavenderBlush1", 0x00fff0f5},
		{"LavenderBlush2", 0x00eee0e5},
		{"LavenderBlush3", 0x00cdc1c5},
		{"LavenderBlush4", 0x008b8386},
		{"MistyRose1", 0x00ffe4e1},
		{"MistyRose2", 0x00eed5d2},
		{"MistyRose3", 0x00cdb7b5},
		{"MistyRose4", 0x008b7d7b},
		{"azure1", 0x00f0ffff},
		{"azure2", 0x00e0eeee},
		{"azure3", 0x00c1cdcd},
		{"azure4", 0x00838b8b},
		{"SlateBlue1", 0x00836fff},
		{"SlateBlue2", 0x007a67ee},
		{"SlateBlue3", 0x006959cd},
		{"SlateBlue4", 0x00473c8b},
		{"RoyalBlue1", 0x004876ff},
		{"RoyalBlue2", 0x00436eee},
		{"RoyalBlue3", 0x003a5fcd},
		{"RoyalBlue4", 0x0027408b},
		{"blue1", 0x000000ff},
		{"blue2", 0x000000ee},
		{"blue3", 0x000000cd},
		{"blue4", 0x0000008b},
		{"DodgerBlue1", 0x001e90ff},
		{"DodgerBlue2", 0x001c86ee},
		{"DodgerBlue3", 0x001874cd},
		{"DodgerBlue4", 0x00104e8b},
		{"SteelBlue1", 0x0063b8ff},
		{"SteelBlue2", 0x005cacee},
		{"SteelBlue3", 0x004f94cd},
		{"SteelBlue4", 0x0036648b},
		{"DeepSkyBlue1", 0x0000bfff},
		{"DeepSkyBlue2", 0x0000b2ee},
		{"DeepSkyBlue3", 0x00009acd},
		{"DeepSkyBlue4", 0x0000688b},
		{"SkyBlue1", 0x0087ceff},
		{"SkyBlue2", 0x007ec0ee},
		{"SkyBlue3", 0x006ca6cd},
		{"SkyBlue4", 0x004a708b},
		{"LightSkyBlue1", 0x00b0e2ff},
		{"LightSkyBlue2", 0x00a4d3ee},
		{"LightSkyBlue3", 0x008db6cd},
		{"LightSkyBlue4", 0x00607b8b},
		{"SlateGray1", 0x00c6e2ff},
		{"SlateGray2", 0x00b9d3ee},
		{"SlateGray3", 0x009fb6cd},
		{"SlateGray4", 0x006c7b8b},
		{"LightSteelBlue1", 0x00cae1ff},
		{"LightSteelBlue2", 0x00bcd2ee},
		{"LightSteelBlue3", 0x00a2b5cd},
		{"LightSteelBlue4", 0x006e7b8b},
		{"LightBlue1", 0x00bfefff},
		{"LightBlue2", 0x00b2dfee},
		{"LightBlue3", 0x009ac0cd},
		{"LightBlue4", 0x0068838b},
		{"LightCyan1", 0x00e0ffff},
		{"LightCyan2", 0x00d1eeee},
		{"LightCyan3", 0x00b4cdcd},
		{"LightCyan4", 0x007a8b8b},
		{"PaleTurquoise1", 0x00bbffff},
		{"PaleTurquoise2", 0x00aeeeee},
		{"PaleTurquoise3", 0x0096cdcd},
		{"PaleTurquoise4", 0x00668b8b},
		{"CadetBlue1", 0x0098f5ff},
		{"CadetBlue2", 0x008ee5ee},
		{"CadetBlue3", 0x007ac5cd},
		{"CadetBlue4", 0x0053868b},
		{"turquoise1", 0x0000f5ff},
		{"turquoise2", 0x0000e5ee},
		{"turquoise3", 0x0000c5cd},
		{"turquoise4", 0x0000868b},
		{"cyan1", 0x0000ffff},
		{"cyan2", 0x0000eeee},
		{"cyan3", 0x0000cdcd},
		{"cyan4", 0x00008b8b},
		{"DarkSlateGray1", 0x0097ffff},
		{"DarkSlateGray2", 0x008deeee},
		{"DarkSlateGray3", 0x0079cdcd},
		{"DarkSlateGray4", 0x00528b8b},
		{"aquamarine1", 0x007fffd4},
		{"aquamarine2", 0x0076eec6},
		{"aquamarine3", 0x0066cdaa},
		{"aquamarine4", 0x00458b74},
		{"DarkSeaGreen1", 0x00c1ffc1},
		{"DarkSeaGreen2", 0x00b4eeb4},
		{"DarkSeaGreen3", 0x009bcd9b},
		{"DarkSeaGreen4", 0x00698b69},
		{"SeaGreen1", 0x0054ff9f},
		{"SeaGreen2", 0x004eee94},
		{"SeaGreen3", 0x0043cd80},
		{"SeaGreen4", 0x002e8b57},
		{"PaleGreen1", 0x009aff9a},
		{"PaleGreen2", 0x0090ee90},
		{"PaleGreen3", 0x007ccd7c},
		{"PaleGreen4", 0x00548b54},
		{"SpringGreen1", 0x0000ff7f},
		{"SpringGreen2", 0x0000ee76},
		{"SpringGreen3", 0x0000cd66},
		{"SpringGreen4", 0x00008b45},
		{"green1", 0x0000ff00},
		{"green2", 0x0000ee00},
		{"green3", 0x0000cd00},
		{"green4", 0x00008b00},
		{"chartreuse1", 0x007fff00},
		{"chartreuse2", 0x0076ee00},
		{"chartreuse3", 0x0066cd00},
		{"chartreuse4", 0x00458b00},
		{"OliveDrab1", 0x00c0ff3e},
		{"OliveDrab2", 0x00b3ee3a},
		{"OliveDrab3", 0x009acd32},
		{"OliveDrab4", 0x00698b22},
		{"DarkOliveGreen1", 0x00caff70},
		{"DarkOliveGreen2", 0x00bcee68},
		{"DarkOliveGreen3", 0x00a2cd5a},
		{"DarkOliveGreen4", 0x006e8b3d},
		{"khaki1", 0x00fff68f},
		{"khaki2", 0x00eee685},
		{"khaki3", 0x00cdc673},
		{"khaki4", 0x008b864e},
		{"LightGoldenrod1", 0x00ffec8b},
		{"LightGoldenrod2", 0x00eedc82},
		{"LightGoldenrod3", 0x00cdbe70},
		{"LightGoldenrod4", 0x008b814c},
		{"LightYellow1", 0x00ffffe0},
		{"LightYellow2", 0x00eeeed1},
		{"LightYellow3", 0x00cdcdb4},
		{"LightYellow4", 0x008b8b7a},
		{"yellow1", 0x00ffff00},
		{"yellow2", 0x00eeee00},
		{"yellow3", 0x00cdcd00},
		{"yellow4", 0x008b8b00},
		{"gold1", 0x00ffd700},
		{"gold2", 0x00eec900},
		{"gold3", 0x00cdad00},
		{"gold4", 0x008b7500},
		{"goldenrod1", 0x00ffc125},
		{"goldenrod2", 0x00eeb422},
		{"goldenrod3", 0x00cd9b1d},
		{"goldenrod4", 0x008b6914},
		{"DarkGoldenrod1", 0x00ffb90f},
		{"DarkGoldenrod2", 0x00eead0e},
		{"DarkGoldenrod3", 0x00cd950c},
		{"DarkGoldenrod4", 0x008b6508},
		{"RosyBrown1", 0x00ffc1c1},
		{"RosyBrown2", 0x00eeb4b4},
		{"RosyBrown3", 0x00cd9b9b},
		{"RosyBrown4", 0x008b6969},
		{"IndianRed1", 0x00ff6a6a},
		{"IndianRed2", 0x00ee6363},
		{"IndianRed3", 0x00cd5555},
		{"IndianRed4", 0x008b3a3a},
		{"sienna1", 0x00ff8247},
		{"sienna2", 0x00ee7942},
		{"sienna3", 0x00cd6839},
		{"sienna4", 0x008b4726},
		{"burlywood1", 0x00ffd39b},
		{"burlywood2", 0x00eec591},
		{"burlywood3", 0x00cdaa7d},
		{"burlywood4", 0x008b7355},
		{"wheat1", 0x00ffe7ba},
		{"wheat2", 0x00eed8ae},
		{"wheat3", 0x00cdba96},
		{"wheat4", 0x008b7e66},
		{"tan1", 0x00ffa54f},
		{"tan2", 0x00ee9a49},
		{"tan3", 0x00cd853f},
		{"tan4", 0x008b5a2b},
		{"chocolate1", 0x00ff7f24},
		{"chocolate2", 0x00ee7621},
		{"chocolate3", 0x00cd661d},
		{"chocolate4", 0x008b4513},
		{"firebrick1", 0x00ff3030},
		{"firebrick2", 0x00ee2c2c},
		{"firebrick3", 0x00cd2626},
		{"firebrick4", 0x008b1a1a},
		{"brown1", 0x00ff4040},
		{"brown2", 0x00ee3b3b},
		{"brown3", 0x00cd3333},
		{"brown4", 0x008b2323},
		{"salmon1", 0x00ff8c69},
		{"salmon2", 0x00ee8262},
		{"salmon3", 0x00cd7054},
		{"salmon4", 0x008b4c39},
		{"LightSalmon1", 0x00ffa07a},
		{"LightSalmon2", 0x00ee9572},
		{"LightSalmon3", 0x00cd8162},
		{"LightSalmon4", 0x008b5742},
		{"orange1", 0x00ffa500},
		{"orange2", 0x00ee9a00},
		{"orange3", 0x00cd8500},
		{"orange4", 0x008b5a00},
		{"DarkOrange1", 0x00ff7f00},
		{"DarkOrange2", 0x00ee7600},
		{"DarkOrange3", 0x00cd6600},
		{"DarkOrange4", 0x008b4500},
		{"coral1", 0x00ff7256},
		{"coral2", 0x00ee6a50},
		{"coral3", 0x00cd5b45},
		{"coral4", 0x008b3e2f},
		{"tomato1", 0x00ff6347},
		{"tomato2", 0x00ee5c42},
		{"tomato3", 0x00cd4f39},
		{"tomato4", 0x008b3626},
		{"OrangeRed1", 0x00ff4500},
		{"OrangeRed2", 0x00ee4000},
		{"OrangeRed3", 0x00cd3700},
		{"OrangeRed4", 0x008b2500},
		{"red1", 0x00ff0000},
		{"red2", 0x00ee0000},
		{"red3", 0x00cd0000},
		{"red4", 0x008b0000},
		{"DeepPink1", 0x00ff1493},
		{"DeepPink2", 0x00ee1289},
		{"DeepPink3", 0x00cd1076},
		{"DeepPink4", 0x008b0a50},
		{"HotPink1", 0x00ff6eb4},
		{"HotPink2", 0x00ee6aa7},
		{"HotPink3", 0x00cd6090},
		{"HotPink4", 0x008b3a62},
		{"pink1", 0x00ffb5c5},
		{"pink2", 0x00eea9b8},
		{"pink3", 0x00cd919e},
		{"pink4", 0x008b636c},
		{"LightPink1", 0x00ffaeb9},
		{"LightPink2", 0x00eea2ad},
		{"LightPink3", 0x00cd8c95},
		{"LightPink4", 0x008b5f65},
		{"PaleVioletRed1", 0x00ff82ab},
		{"PaleVioletRed2", 0x00ee799f},
		{"PaleVioletRed3", 0x00cd6889},
		{"PaleVioletRed4", 0x008b475d},
		{"maroon1", 0x00ff34b3},
		{"maroon2", 0x00ee30a7},
		{"maroon3", 0x00cd2990},
		{"maroon4", 0x008b1c62},
		{"VioletRed1", 0x00ff3e96},
		{"VioletRed2", 0x00ee3a8c},
		{"VioletRed3", 0x00cd3278},
		{"VioletRed4", 0x008b2252},
		{"magenta1", 0x00ff00ff},
		{"magenta2", 0x00ee00ee},
		{"magenta3", 0x00cd00cd},
		{"magenta4", 0x008b008b},
		{"orchid1", 0x00ff83fa},
		{"orchid2", 0x00ee7ae9},
		{"orchid3", 0x00cd69c9},
		{"orchid4", 0x008b4789},
		{"plum1", 0x00ffbbff},
		{"plum2", 0x00eeaeee},
		{"plum3", 0x00cd96cd},
		{"plum4", 0x008b668b},
		{"MediumOrchid1", 0x00e066ff},
		{"MediumOrchid2", 0x00d15fee},
		{"MediumOrchid3", 0x00b452cd},
		{"MediumOrchid4", 0x007a378b},
		{"DarkOrchid1", 0x00bf3eff},
		{"DarkOrchid2", 0x00b23aee},
		{"DarkOrchid3", 0x009a32cd},
		{"DarkOrchid4", 0x0068228b},
		{"purple1", 0x009b30ff},
		{"purple2", 0x00912cee},
		{"purple3", 0x007d26cd},
		{"purple4", 0x00551a8b},
		{"MediumPurple1", 0x00ab82ff},
		{"MediumPurple2", 0x009f79ee},
		{"MediumPurple3", 0x008968cd},
		{"MediumPurple4", 0x005d478b},
		{"thistle1", 0x00ffe1ff},
		{"thistle2", 0x00eed2ee},
		{"thistle3", 0x00cdb5cd},
		{"thistle4", 0x008b7b8b},
		{"gray0", 0x00000000},
		{"grey0", 0x00000000},
		{"gray1", 0x00030303},
		{"grey1", 0x00030303},
		{"gray2", 0x00050505},
		{"grey2", 0x00050505},
		{"gray3", 0x00080808},
		{"grey3", 0x00080808},
		{"gray4", 0x000a0a0a},
		{"grey4", 0x000a0a0a},
		{"gray5", 0x000d0d0d},
		{"grey5", 0x000d0d0d},
		{"gray6", 0x000f0f0f},
		{"grey6", 0x000f0f0f},
		{"gray7", 0x00121212},
		{"grey7", 0x00121212},
		{"gray8", 0x00141414},
		{"grey8", 0x00141414},
		{"gray9", 0x00171717},
		{"grey9", 0x00171717},
		{"gray10", 0x001a1a1a},
		{"grey10", 0x001a1a1a},
		{"gray11", 0x001c1c1c},
		{"grey11", 0x001c1c1c},
		{"gray12", 0x001f1f1f},
		{"grey12", 0x001f1f1f},
		{"gray13", 0x00212121},
		{"grey13", 0x00212121},
		{"gray14", 0x00242424},
		{"grey14", 0x00242424},
		{"gray15", 0x00262626},
		{"grey15", 0x00262626},
		{"gray16", 0x00292929},
		{"grey16", 0x00292929},
		{"gray17", 0x002b2b2b},
		{"grey17", 0x002b2b2b},
		{"gray18", 0x002e2e2e},
		{"grey18", 0x002e2e2e},
		{"gray19", 0x00303030},
		{"grey19", 0x00303030},
		{"gray20", 0x00333333},
		{"grey20", 0x00333333},
		{"gray21", 0x00363636},
		{"grey21", 0x00363636},
		{"gray22", 0x00383838},
		{"grey22", 0x00383838},
		{"gray23", 0x003b3b3b},
		{"grey23", 0x003b3b3b},
		{"gray24", 0x003d3d3d},
		{"grey24", 0x003d3d3d},
		{"gray25", 0x00404040},
		{"grey25", 0x00404040},
		{"gray26", 0x00424242},
		{"grey26", 0x00424242},
		{"gray27", 0x00454545},
		{"grey27", 0x00454545},
		{"gray28", 0x00474747},
		{"grey28", 0x00474747},
		{"gray29", 0x004a4a4a},
		{"grey29", 0x004a4a4a},
		{"gray30", 0x004d4d4d},
		{"grey30", 0x004d4d4d},
		{"gray31", 0x004f4f4f},
		{"grey31", 0x004f4f4f},
		{"gray32", 0x00525252},
		{"grey32", 0x00525252},
		{"gray33", 0x00545454},
		{"grey33", 0x00545454},
		{"gray34", 0x00575757},
		{"grey34", 0x00575757},
		{"gray35", 0x00595959},
		{"grey35", 0x00595959},
		{"gray36", 0x005c5c5c},
		{"grey36", 0x005c5c5c},
		{"gray37", 0x005e5e5e},
		{"grey37", 0x005e5e5e},
		{"gray38", 0x00616161},
		{"grey38", 0x00616161},
		{"gray39", 0x00636363},
		{"grey39", 0x00636363},
		{"gray40", 0x00666666},
		{"grey40", 0x00666666},
		{"gray41", 0x00696969},
		{"grey41", 0x00696969},
		{"gray42", 0x006b6b6b},
		{"grey42", 0x006b6b6b},
		{"gray43", 0x006e6e6e},
		{"grey43", 0x006e6e6e},
		{"gray44", 0x00707070},
		{"grey44", 0x00707070},
		{"gray45", 0x00737373},
		{"grey45", 0x00737373},
		{"gray46", 0x00757575},
		{"grey46", 0x00757575},
		{"gray47", 0x00787878},
		{"grey47", 0x00787878},
		{"gray48", 0x007a7a7a},
		{"grey48", 0x007a7a7a},
		{"gray49", 0x007d7d7d},
		{"grey49", 0x007d7d7d},
		{"gray50", 0x007f7f7f},
		{"grey50", 0x007f7f7f},
		{"gray51", 0x00828282},
		{"grey51", 0x00828282},
		{"gray52", 0x00858585},
		{"grey52", 0x00858585},
		{"gray53", 0x00878787},
		{"grey53", 0x00878787},
		{"gray54", 0x008a8a8a},
		{"grey54", 0x008a8a8a},
		{"gray55", 0x008c8c8c},
		{"grey55", 0x008c8c8c},
		{"gray56", 0x008f8f8f},
		{"grey56", 0x008f8f8f},
		{"gray57", 0x00919191},
		{"grey57", 0x00919191},
		{"gray58", 0x00949494},
		{"grey58", 0x00949494},
		{"gray59", 0x00969696},
		{"grey59", 0x00969696},
		{"gray60", 0x00999999},
		{"grey60", 0x00999999},
		{"gray61", 0x009c9c9c},
		{"grey61", 0x009c9c9c},
		{"gray62", 0x009e9e9e},
		{"grey62", 0x009e9e9e},
		{"gray63", 0x00a1a1a1},
		{"grey63", 0x00a1a1a1},
		{"gray64", 0x00a3a3a3},
		{"grey64", 0x00a3a3a3},
		{"gray65", 0x00a6a6a6},
		{"grey65", 0x00a6a6a6},
		{"gray66", 0x00a8a8a8},
		{"grey66", 0x00a8a8a8},
		{"gray67", 0x00ababab},
		{"grey67", 0x00ababab},
		{"gray68", 0x00adadad},
		{"grey68", 0x00adadad},
		{"gray69", 0x00b0b0b0},
		{"grey69", 0x00b0b0b0},
		{"gray70", 0x00b3b3b3},
		{"grey70", 0x00b3b3b3},
		{"gray71", 0x00b5b5b5},
		{"grey71", 0x00b5b5b5},
		{"gray72", 0x00b8b8b8},
		{"grey72", 0x00b8b8b8},
		{"gray73", 0x00bababa},
		{"grey73", 0x00bababa},
		{"gray74", 0x00bdbdbd},
		{"grey74", 0x00bdbdbd},
		{"gray75", 0x00bfbfbf},
		{"grey75", 0x00bfbfbf},
		{"gray76", 0x00c2c2c2},
		{"grey76", 0x00c2c2c2},
		{"gray77", 0x00c4c4c4},
		{"grey77", 0x00c4c4c4},
		{"gray78", 0x00c7c7c7},
		{"grey78", 0x00c7c7c7},
		{"gray79", 0x00c9c9c9},
		{"grey79", 0x00c9c9c9},
		{"gray80", 0x00cccccc},
		{"grey80", 0x00cccccc},
		{"gray81", 0x00cfcfcf},
		{"grey81", 0x00cfcfcf},
		{"gray82", 0x00d1d1d1},
		{"grey82", 0x00d1d1d1},
		{"gray83", 0x00d4d4d4},
		{"grey83", 0x00d4d4d4},
		{"gray84", 0x00d6d6d6},
		{"grey84", 0x00d6d6d6},
		{"gray85", 0x00d9d9d9},
		{"grey85", 0x00d9d9d9},
		{"gray86", 0x00dbdbdb},
		{"grey86", 0x00dbdbdb},
		{"gray87", 0x00dedede},
		{"grey87", 0x00dedede},
		{"gray88", 0x00e0e0e0},
		{"grey88", 0x00e0e0e0},
		{"gray89", 0x00e3e3e3},
		{"grey89", 0x00e3e3e3},
		{"gray90", 0x00e5e5e5},
		{"grey90", 0x00e5e5e5},
		{"gray91", 0x00e8e8e8},
		{"grey91", 0x00e8e8e8},
		{"gray92", 0x00ebebeb},
		{"grey92", 0x00ebebeb},
		{"gray93", 0x00ededed},
		{"grey93", 0x00ededed},
		{"gray94", 0x00f0f0f0},
		{"grey94", 0x00f0f0f0},
		{"gray95", 0x00f2f2f2},
		{"grey95", 0x00f2f2f2},
		{"gray96", 0x00f5f5f5},
		{"grey96", 0x00f5f5f5},
		{"gray97", 0x00f7f7f7},
		{"grey97", 0x00f7f7f7},
		{"gray98", 0x00fafafa},
		{"grey98", 0x00fafafa},
		{"gray99", 0x00fcfcfc},
		{"grey99", 0x00fcfcfc},
		{"gray100", 0x00ffffff},
		{"grey100", 0x00ffffff},
		{"DarkGrey", 0x00a9a9a9},
		{"DarkGray", 0x00a9a9a9},
		{"DarkBlue", 0x0000008b},
		{"DarkCyan", 0x00008b8b},
		{"DarkMagenta", 0x008b008b},
		{"DarkRed", 0x008b0000},
		{"LightGreen", 0x0090ee90}		
	};
	
	if(spec[0] == '#') {
		char buf[7];
		switch(speclen) {
			case 4:
				buf[0] = buf[1] = spec[1];
				buf[2] = buf[3] = spec[2];
				buf[4] = buf[5] = spec[3];
				break;
			case 7:
				memcpy(buf, spec + 1, 6);
				break;
			case 13:
				buf[0] = spec[1];
				buf[1] = spec[2];
				buf[2] = spec[5];
				buf[3] = spec[6];
				buf[4] = spec[9];
				buf[5] = spec[10];
				break;
		}
		buf[6] = '\0';
		*rgb = strtol(buf, NULL, 16);
		return 1;
	} else {
		int i;
		for(i = 0; i < ARRAYSIZE(known); i++)
			if(string_equal(known[i].name, spec, speclen)) {
				*rgb = known[i].rgb;
				return 1;
			}
		return 0;
	}
}

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

static char *linebuf;
static int buflen;
static const char *error;

/*
 * Read next line from the source.
 * If len > 0, it's assumed to be at least len chars (for efficiency).
 * Return NULL and set error upon EOF or parse error.
 */
static char *get_next_line(char ***lines, SDL_RWops *src, int len)
{
	if(lines) {
		return *(*lines)++;
	} else {
		char c;
		int n;
		do {
			if(SDL_RWread(src, &c, 1, 1) <= 0) {
				error = "Premature end of data";
				return NULL;
			}
		} while(c != '"');
		if(len) {
			len += 4;	/* "\",\n\0" */
			if(len > buflen){
				buflen = len;
				linebuf = (char *)realloc(linebuf, buflen);
				if(!linebuf) {
					error = "Out of memory";
					return NULL;
				}
			}
			if(SDL_RWread(src, linebuf, len - 1, 1) <= 0) {
				error = "Premature end of data";
				return NULL;
			}
			n = len - 2;
		} else {
			n = 0;
			do {
				if(n >= buflen - 1) {
					if(buflen == 0)
						buflen = 16;
					buflen *= 2;
					linebuf = (char *)realloc(linebuf, buflen);
					if(!linebuf) {
						error = "Out of memory";
						return NULL;
					}
				}
				if(SDL_RWread(src, linebuf + n, 1, 1) <= 0) {
					error = "Premature end of data";
					return NULL;
				}
			} while(linebuf[n++] != '"');
			n--;
		}
		linebuf[n] = '\0';
		return linebuf;
	}
}

#define SKIPSPACE(p)				\
do {						\
while(isspace((unsigned char)*(p)))	\
++(p);				\
} while(0)

#define SKIPNONSPACE(p)					\
do {							\
while(!isspace((unsigned char)*(p)) && *p)	\
++(p);					\
} while(0)

/* read XPM from either array or RWops */
static SDL_Surface *load_xpm(char **xpm, SDL_RWops *src)
{
	int start = 0;
	SDL_Surface *image = NULL;
	int index;
	int x, y;
	int w, h, ncolors, cpp;
	int indexed;
	Uint8 *dst;
	struct color_hash *colors = NULL;
	SDL_Color *im_colors = NULL;
	char *keystrings = NULL, *nextkey;
	char *line;
	char ***xpmlines = NULL;
	int pixels_len;
	const int noTransparentColor=0x6fffffff;
	int transparentColor=noTransparentColor;
	
	error = NULL;
	linebuf = NULL;
	buflen = 0;
	
	if ( src ) 
		start = SDL_RWtell(src);
	
	if(xpm)
		xpmlines = &xpm;
	
	line = get_next_line(xpmlines, src, 0);
	if(!line)
		goto done;
	/*
	 * The header string of an XPMv3 image has the format
	 *
	 * <width> <height> <ncolors> <cpp> [ <hotspot_x> <hotspot_y> ]
	 *
	 * where the hotspot coords are intended for mouse cursors.
	 * Right now we don't use the hotspots but it should be handled
	 * one day.
	 */
	if(sscanf(line, "%d %d %d %d", &w, &h, &ncolors, &cpp) != 4
	   || w <= 0 || h <= 0 || ncolors <= 0 || cpp <= 0) {
		error = "Invalid format description";
		goto done;
	}
	
	keystrings = (char *)malloc(ncolors * cpp);
	if(!keystrings) {
		error = "Out of memory";
		goto done;
	}
	nextkey = keystrings;
	
	/* Create the new surface */
	if(ncolors <= 256) {
		indexed = 1;
		image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8,
									 0, 0, 0, 0);
		im_colors = image->format->palette->colors;
		image->format->palette->ncolors = ncolors;
	} else {
		indexed = 0;
		image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
									 0xff0000, 0x00ff00, 0x0000ff, 0);
	}
	if(!image) {
		/* Hmm, some SDL error (out of memory?) */
		goto done;
	}
	
	/* Read the colors */
	colors = create_colorhash(ncolors);
	if (!colors) {
		error = "Out of memory";
		goto done;
	}
	for(index = 0; index < ncolors; ++index ) {
		char *p;
		
		line = get_next_line(xpmlines, src, 0);
		if(!line)
			goto done;
		
		p = line + cpp + 1;
	
		/* parse a colour definition */
		for(;;) {
			char nametype;
			char *colname;
			Uint32 rgb, pixel;
			
			SKIPSPACE(p);
			if(!*p) {
				error = "colour parse error";
				goto done;
			}
			nametype = *p;
			SKIPNONSPACE(p);
			SKIPSPACE(p);
			colname = p;
			SKIPNONSPACE(p);
			if(nametype == 's')
				continue;      /* skip symbolic colour names */
			
			if(!color_to_rgb(colname, p - colname, &rgb))
				continue;
			
			memcpy(nextkey, line, cpp);
			if(indexed) {
				SDL_Color *c = im_colors + index;
				c->r = (Uint8)(rgb >> 16);
				c->g = (Uint8)(rgb >> 8);
				c->b = (Uint8)(rgb);
				pixel = index;
			} else
				pixel = rgb;
			if(rgb==0xffffffff){
				if(transparentColor==noTransparentColor){
					transparentColor=pixel;
				}
				pixel=transparentColor;
			}
			add_colorhash(colors, nextkey, cpp, pixel);
			nextkey += cpp;
			if(rgb == 0xffffffff)
				SDL_SetColorKey(image, SDL_SRCCOLORKEY, pixel);
			break;
		}
	}
	
	/* Read the pixels */
	pixels_len = w * cpp;
	dst = (Uint8 *)(image->pixels);
	for(y = 0; y < h; y++) {
		line = get_next_line(xpmlines, src, pixels_len);
		if(indexed) {
			/* optimization for some common cases */
			if(cpp == 1)
				for(x = 0; x < w; x++)
					dst[x] = (Uint8)QUICK_COLORHASH(colors,
													line + x);
			else
				for(x = 0; x < w; x++)
					dst[x] = (Uint8)get_colorhash(colors,
												  line + x * cpp,
												  cpp);
		} else {
			for (x = 0; x < w; x++)
				((Uint32*)dst)[x] = get_colorhash(colors,
												  line + x * cpp,
												  cpp);
		}
		dst += image->pitch;
	}
	
done:
	if(error) {
		if ( src )
			SDL_RWseek(src, start, RW_SEEK_SET);
		if ( image ) {
			SDL_FreeSurface(image);
			image = NULL;
		}
		
	}
	free(keystrings);
	free_colorhash(colors);
	free(linebuf);
	if(error)
		throw error;
	return(image);
}


static SDL_Surface *IMG_ReadXPMFromArray(char **xpm)
{
	return load_xpm(xpm, NULL);
}

twSDLDC *twSDLDC::loadFromXPM(const char **xpm){
	return new twSDLDC(IMG_ReadXPMFromArray(const_cast<char **>(xpm)), twPoint(0, 0), true);
}	
