/*
 *  twStrConv.cpp
 *  
 *
 *  Created by Nexhawks on 10/01/13.
 *  Copyright 2011 Nexhawks. All rights reserved.
 *
 */

#include <tcw/twStrConv.h>
#ifdef __MACOSX__
#include <stdlib.h>
#endif

using namespace std;


static uint32_t invalidUtf8(char ** const ptr){
	uint32_t result=(unsigned char)*(*ptr);
	(*ptr)++;
	return result;
}

static uint32_t unicodeFromUtf8(char ** const ptr){
	uint32_t result;
	if(((*(*ptr)) & 0x80) == 0){
		result=*(*ptr);
		(*ptr)+=1;
		return result;
	}
	if(((*(*ptr)) & 0x40) == 0){
		return invalidUtf8(ptr);
	}
	if(((*(*ptr)) & 0x20) == 0){
		if((*(*ptr+1))== 0){
			return invalidUtf8(ptr);
		}
		result=(uint32_t)(*(*ptr+0)&0x1f)<<6;
		result+=(uint32_t)(*(*ptr+1)&0x3f)<<0;
		(*ptr)+=2;
		return result;
	}
	if(((*(*ptr)) & 0x10) == 0){
		if((*(*ptr+1))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+2))== 0){
			return invalidUtf8(ptr);
		}
		result=(uint32_t)(*(*ptr+0)&0xf)<<12;
		result+=(uint32_t)(*(*ptr+1)&0x3f)<<6;
		result+=(uint32_t)(*(*ptr+2)&0x3f)<<0;
		(*ptr)+=3;
		return result;
	}
	if(((*(*ptr)) & 0x08) == 0){
		if((*(*ptr+1))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+2))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+3))== 0){
			return invalidUtf8(ptr);
		}
		result=(uint32_t)(*(*ptr+0)&0x7)<<18;
		result+=(uint32_t)(*(*ptr+1)&0x3f)<<12;
		result+=(uint32_t)(*(*ptr+2)&0x3f)<<6;
		result+=(uint32_t)(*(*ptr+3)&0x3f)<<0;
		(*ptr)+=4;
		return result;
	}
	if(((*(*ptr)) & 0x04) == 0){
		if((*(*ptr+1))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+2))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+3))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+4))== 0){
			return invalidUtf8(ptr);
		}
		result=(uint32_t)(*(*ptr+0)&0x3)<<24;
		result+=(uint32_t)(*(*ptr+1)&0x3f)<<18;
		result+=(uint32_t)(*(*ptr+2)&0x3f)<<12;
		result+=(uint32_t)(*(*ptr+3)&0x3f)<<6;
		result+=(uint32_t)(*(*ptr+4)&0x3f)<<0;
		(*ptr)+=5;
		return result;
	}
	if(((*(*ptr)) & 0x02) == 0){
		if((*(*ptr+1))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+2))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+3))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+4))== 0){
			return invalidUtf8(ptr);
		}
		if((*(*ptr+5))== 0){
			return invalidUtf8(ptr);
		}
		result=(uint32_t)(*(*ptr+0)&0x1)<<30;
		result+=(uint32_t)(*(*ptr+1)&0x3f)<<24;
		result+=(uint32_t)(*(*ptr+2)&0x3f)<<18;
		result+=(uint32_t)(*(*ptr+3)&0x3f)<<12;
		result+=(uint32_t)(*(*ptr+4)&0x3f)<<6;
		result+=(uint32_t)(*(*ptr+5)&0x3f)<<0;
		(*ptr)+=6;
		return result;
	}
	return invalidUtf8(ptr);
}

void twM2W(wchar_t *to, const char *from){
	while(*from){
		uint32_t unicode;
		unicode=unicodeFromUtf8((char ** const )&from);
		if(unicode<0x10000){
			*to=unicode;
		}else{
			*to=L'?';
		}
		to++;
	}
	*to=0;
}

void twW2M(char *to, const wchar_t *from){
	while(*from){
		uint32_t unicode=(uint32_t)*from;
		if(unicode<0x80){
			*to=(char)unicode; to++;
		}else if(unicode<0x800){
			*to=0xc0|(char)((unicode>>6)&0x1f); to++;
			*to=0x80|(char)((unicode)&0x3f); to++;
		}else if(unicode<0x10000){
			*to=0xe0|(char)((unicode>>12)&0xf); to++;
			*to=0x80|(char)((unicode>>6)&0x3f); to++;
			*to=0x80|(char)((unicode)&0x3f); to++;
		}else if(unicode<0x200000){
			*to=0xf0|(char)((unicode>>18)&0x7); to++;
			*to=0x80|(char)((unicode>>12)&0x3f); to++;
			*to=0x80|(char)((unicode>>6)&0x3f); to++;
			*to=0x80|(char)((unicode)&0x3f); to++;
		}else if(unicode<0x4000000){
			*to=0xf8|(char)((unicode>>24)&0x3); to++;
			*to=0x80|(char)((unicode>>18)&0x3f); to++;
			*to=0x80|(char)((unicode>>12)&0x3f); to++;
			*to=0x80|(char)((unicode>>6)&0x3f); to++;
			*to=0x80|(char)((unicode)&0x3f); to++;
		}else if(unicode<0x80000000){
			*to=0xfc|(char)((unicode>>30)&0x1); to++;
			*to=0x80|(char)((unicode>>24)&0x3f); to++;
			*to=0x80|(char)((unicode>>18)&0x3f); to++;
			*to=0x80|(char)((unicode>>12)&0x3f); to++;
			*to=0x80|(char)((unicode>>6)&0x3f); to++;
			*to=0x80|(char)((unicode)&0x3f); to++;
		}
		from++;
	}
	*to=0;
}


std::wstring twM2W(const std::string& str){
	const char *from=str.c_str();
	std::wstring to;
	while(*from){
		uint32_t unicode;
		unicode=unicodeFromUtf8((char ** const )&from);
		if(unicode<0x10000){
			to+=unicode;
		}else{
			to+=L'?';
		}
		
	}
	return to;
}
std::string twW2M(const std::wstring& str){
	std::wstring::size_type pos=0;
	std::string to;
	while(pos<str.size()){
		uint32_t unicode=(uint32_t)str[pos];
		if(unicode<0x80){
			to+=(char)unicode; 
		}else if(unicode<0x800){
			to+=0xc0|(char)((unicode>>6)&0x1f); 
			to+=0x80|(char)((unicode)&0x3f); 
		}else if(unicode<0x10000){
			to+=0xe0|(char)((unicode>>12)&0xf); 
			to+=0x80|(char)((unicode>>6)&0x3f); 
			to+=0x80|(char)((unicode)&0x3f); 
		}else if(unicode<0x200000){
			to+=0xf0|(char)((unicode>>18)&0x7); 
			to+=0x80|(char)((unicode>>12)&0x3f); 
			to+=0x80|(char)((unicode>>6)&0x3f); 
			to+=0x80|(char)((unicode)&0x3f); 
		}else if(unicode<0x4000000){
			to+=0xf8|(char)((unicode>>24)&0x3); 
			to+=0x80|(char)((unicode>>18)&0x3f); 
			to+=0x80|(char)((unicode>>12)&0x3f); 
			to+=0x80|(char)((unicode>>6)&0x3f); 
			to+=0x80|(char)((unicode)&0x3f); 
		}else if(unicode<0x80000000){
			to+=0xfc|(char)((unicode>>30)&0x1); 
			to+=0x80|(char)((unicode>>24)&0x3f); 
			to+=0x80|(char)((unicode>>18)&0x3f); 
			to+=0x80|(char)((unicode>>12)&0x3f); 
			to+=0x80|(char)((unicode>>6)&0x3f); 
			to+=0x80|(char)((unicode)&0x3f); 
		}
		pos++;
	}
	return to;
}

