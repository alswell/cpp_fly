#pragma once
#include "dict.h"
#include "type.h"

namespace JSON {
	CString Dump(const DICT(CString)& dict);

	void LoadFile(CSmartType& json, const char* filename);
	void Load(CSmartType& json, const char* str);
	char *ParseDict(CSmartType& dict, const char* str);
	char *ParseList(CSmartType& list, const char* str);
}
