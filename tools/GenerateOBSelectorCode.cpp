//
//  GenerateOBSelectorCode.cpp
//  XTBook
//
//  Created by Nexhawks on 4/27/11.
//  Copyright 2011 Nexhawks. All rights reserved.
//



#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <limits.h>
#include <assert.h>

using namespace std;

struct key_t{
	std::wstring str;
	key_t(const std::wstring& s){
		str=s;
	}
};

struct node_t{
	uint16_t border;
	int leftChild;
	int rightChild;
	int charIndex;
};

vector<key_t> g_keys;
vector<node_t> g_nodes;

const int NodeFlag=INT_MIN;
const int NotFound=INT_MAX;

uint16_t convertToOBIndex(int idx){
	if(idx==NotFound)
		return 0xffff;
	if(idx&NodeFlag)
		return (idx&~NodeFlag)|0x8000;
	return idx;
}

bool isRightSideInNode(const node_t& node, const std::wstring& key){
	if(node.charIndex>=key.size()){
		return false;
	}else if((uint16_t)key[node.charIndex]>=node.border){
		return true;
	}else{
		return false;
	}
}

void addKey(int& parent, int keyId){
	std::wstring key=g_keys[keyId].str;
	if(parent==NotFound){
		parent=keyId;
	}else if(parent&NodeFlag){
		int nodeId=parent&~NodeFlag;
		node_t node=g_nodes[nodeId];
		
		if(isRightSideInNode(node, key)){
			addKey(node.rightChild, keyId);
		}else{
			addKey(node.leftChild, keyId);
		}
		
		g_nodes[nodeId]=node;
	}else{
		// we need to create an node to tell keyID from parent.
		int oldKeyId=parent;
		std::wstring oldKey=g_keys[parent].str;
		node_t newNode;
		
		if(oldKey==key){
			// they don't have any difference.
			// cannot tell one from another. bailing out.
			fprintf(stderr, "warning: key[%d]\"%ls\" is the same as [%d]\n", keyId, key.c_str(), parent);
			return;
		}
		
		newNode.leftChild=NotFound;
		newNode.rightChild=NotFound;
		
		newNode.charIndex=-1;
		
		if(key.size()>oldKey.size()){
			newNode.border=(uint16_t)key[oldKey.size()];
			newNode.charIndex=oldKey.size();
		}else if(key.size()<oldKey.size()){
			newNode.border=(uint16_t)oldKey[key.size()];
			newNode.charIndex=key.size();
		}
		
		// find the first difference.
		for(int i=0;i<oldKey.size() && i<key.size();i++){
			if(key[i]!=oldKey[i]){
				newNode.border=((uint16_t)key[i]+(uint16_t)oldKey[i]+1)/2;
				newNode.charIndex=i;
				break;
			}
		}
		
		assert(newNode.charIndex!=-1);
		
		// create node.
		g_nodes.push_back(newNode);
		
		
		int newParent=((int)(g_nodes.size()-1)|NodeFlag);
		parent=newParent;
		//printf("%08x\n", parent);
		assert(newParent&NodeFlag);
		assert(isRightSideInNode(newNode, key)!=
			   isRightSideInNode(newNode, oldKey));
		
		addKey(newParent, oldKeyId);
		addKey(newParent, keyId);
		newNode=g_nodes.back();
		if(isRightSideInNode(newNode, key))
			assert(newNode.rightChild==keyId);
		else
			assert(newNode.leftChild==keyId);
		
		if(isRightSideInNode(newNode, oldKey))
			assert(newNode.rightChild==oldKeyId);
		else
			assert(newNode.leftChild==oldKeyId);
		
	}
}

static unsigned int g_comparsionCounter;

static bool benchCompareString(const std::wstring& str1, const std::wstring& str2){
	for(std::wstring::size_type i=0;i<str1.size() && i<str2.size();i++){
		g_comparsionCounter++;
		if(str1[i]<str2[i])
			return true;
		else if(str1[i]>str2[i])
			return true;
	}
	g_comparsionCounter++;
	if(str1.size()!=str2.size())
		return true;
	return false;
}

static void benchLinearSearch(const std::wstring& str){
	for(int i=0;i<g_keys.size();i++){
		if(!benchCompareString(str, g_keys[i].str))
			break;
	}
}
static int benchBinarySearch(int parent, const std::wstring& str){
	if(parent==NotFound)
		return -1;
	if(parent&NodeFlag){
		node_t& node=g_nodes[parent&~NodeFlag];
		g_comparsionCounter++;
		if(isRightSideInNode(node, str)){
			return benchBinarySearch(node.rightChild, str);
		}else{
			return benchBinarySearch(node.leftChild, str);
		}
	}else{
		benchCompareString(str, g_keys[parent].str);
		return parent;
	}
}


int main(int argc, char **argv){
	
	if(argc==1){
		puts("USAGE: GenerateOBSelectorCode PREFIXNAME");
		return 0;
	}
	
	wchar_t buf[4096];
	fprintf(stderr, "reading the strings...\n");
	while(fgetws(buf, 4096, stdin)){
		
		if(wcschr(buf, L'\n'))
			*wcschr(buf, L'\n')=0;
		if(wcschr(buf, L'\r'))
			*wcschr(buf, L'\r')=0;
		if(wcschr(buf, L'#'))
			*wcschr(buf, L'#')=0;
		
		wchar_t *ptr=buf;
		while(*ptr==L' ' || *ptr==L'\t')
			ptr++;
		
		std::wstring str=buf;
		
		g_keys.push_back(str);
		
	}
	
	int rootNode=NotFound;
	for(int i=0;i<g_keys.size();i++){
		fprintf(stderr, "building tree... (%d of %d keys, %d nodes)\n", i+1, (int)g_keys.size(),
				(int)g_nodes.size());
		addKey(rootNode, i);
	}
	
	if(g_nodes.size()>=32768){
		fprintf(stderr, "error: too many nodes. (%d, max:%d)\n", (int)g_nodes.size(), 32767);
	}
	if(g_keys.size()>=32768){
		fprintf(stderr, "error: too many keys. (%d, max:%d)\n", (int)g_keys.size(), 32767);
	}
	
	printf("static const OBNode %sNodes[]={", argv[1]);
	for(int i=0;i<g_nodes.size();i++){
		if(i%2==0)
			printf("\n");
		node_t& node=g_nodes[i];
		printf("{0x%04x, 0x%04x, ", (int)node.charIndex, (int)node.border);
		printf("0x%04x, ", convertToOBIndex(node.leftChild));
		printf("0x%04x}, ", convertToOBIndex(node.rightChild));
	}
	printf("};\n");
	
	printf("static const OBIndex %sIndices[]={", argv[1]);
	for(int i=0;i<g_keys.size();i++){
		wprintf(L"\n{L\"%ls\"}, ", g_keys[i].str.c_str());
	}
	printf("};\n");
	
	g_comparsionCounter=0;
	
	fprintf(stderr, "testing...\n");
	for(int i=0;i<g_keys.size();i++){
		int ind;
		ind=benchBinarySearch(NodeFlag, g_keys[i].str);
		if(ind==-1){
			fprintf(stderr, "bug:[%d]\"%ls\" reaches no leaves\n", i, g_keys[i].str.c_str());
		}else{
			std::wstring& str2=g_keys[ind].str;
			if(str2!=g_keys[i].str){
				fprintf(stderr, "bug:[%d]\"%ls\" reaches [%d]\"%ls\"\n", i, g_keys[i].str.c_str(),
						ind, str2.c_str());
			}
		}
	}
	
	fprintf(stderr, "benchmarking...\n");
	
	{
		unsigned int minCount, maxCount, totalCount;
		minCount=UINT_MAX; maxCount=0; totalCount=0;
		fprintf(stderr, "linear search:\n");
		for(int i=0;i<g_keys.size();i++){
			g_comparsionCounter=0;
			benchLinearSearch(g_keys[i].str);
			if(g_comparsionCounter<minCount)
				minCount=g_comparsionCounter;
			if(g_comparsionCounter>maxCount)
				maxCount=g_comparsionCounter;
			totalCount+=g_comparsionCounter;
		}
		fprintf(stderr, "min=%u\n", minCount);
		fprintf(stderr, "max=%u\n", maxCount);
		fprintf(stderr, "average=%f\n", (float)totalCount/(float)g_keys.size());
	}
	
	{
		unsigned int minCount, maxCount, totalCount;
		minCount=UINT_MAX; maxCount=0; totalCount=0;
		fprintf(stderr, "binary search:\n");
		for(int i=0;i<g_keys.size();i++){
			g_comparsionCounter=0;
			benchBinarySearch(NodeFlag, g_keys[i].str);
			if(g_comparsionCounter<minCount)
				minCount=g_comparsionCounter;
			if(g_comparsionCounter>maxCount)
				maxCount=g_comparsionCounter;
			totalCount+=g_comparsionCounter;
		}
		fprintf(stderr, "min=%u\n", minCount);
		fprintf(stderr, "max=%u\n", maxCount);
		fprintf(stderr, "average=%f\n", (float)totalCount/(float)g_keys.size());
	}
	
	
}
