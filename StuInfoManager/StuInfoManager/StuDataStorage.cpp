#define _CRT_SECURE_NO_WARNINGS

#include "StuDataStorage.h"

void Dio::StuDataStorage::initdata()
{
	FILE* fp = fopen("STUINFO.txt","rb+");
	fread(&_data_base, sizeof(StuInfo), testsize, fp);
	for (int i = 0; i < testsize; ++i)
	{
		_data.push_back(_data_base[i]);
	}
	fclose(fp);
}






