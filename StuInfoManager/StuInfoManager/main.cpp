#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string>
#include <time.h>
#include <iostream>
#include "StuDataStorage.h"
#include "FuncTable.h"
#include "Parser.h"
#define testsize 50


std::string surname[] = { "¶Å","Èî","À¶","³Â","ÖÜ","Îâ","Ö£","ÄÏ¹ù","ÓîÎÄ","ÉÏ¹Ù","ñÒ","ÓÎ","Û£","óÃ","¶Î¸É","ÍØ°Ï","Óİ","¶ûÂü","°¢","×İ",
"ÕÅÍõ","ºØÀ¼","±±¹¬","ÇüÍ»","ÄÏÃÅ","¶ËÄ¾","ÍõËï" ,"Ä¾","¾©","ºü","Û¨","»¢","Ã¶","¿¹","´ï","è½","ÜÉ",
"ÕÛ","Âó","Çì","¹ı","Öñ","¶Ë","ÏÊ","»Ê","ØÁ","ÀÏ","ÊÇ","ÃØ","³©","Ú÷","»¹","±ö" };

std::string sec_name[] = { "¾²âù"
,  "Û­Ó±"
, "ÎÄ¾ı"
, "ÅÎÈ½"
, "âı·á"
,  "âıçù"
, "¸¦·¼"
, "Åå¾ê"
, "ÓêæÌ"
, "ÃÃñÄ"
, "ÏşÏ¼"
, "ÂşÑ©"
, "ÂşæÃ"
, "ÂşÁÕ"
, "Âşâù"
, "Ø¹İæ"
,  "Óêç÷"
, "Ê®°Ë""Óô","ñã","ÄÜ","²Ô","Ë«","ÎÅ","İ·","µ³","µÔ","Ì·","¹±","ÀÍ","åÌ","¼§","Éê","·ö","¶Â","È½","Ô×","Ûª","Óº","È´",
"è³","É£","¹ğ","å§","Å£","ÊÙ","Í¨","±ß","ìè","Ñà","¼½","ÆÖ","ÉĞ","Å©","ÎÂ","±ğ","×¯","êÌ","²ñ","öÄ","ÑÖ","³ä","Ä½","Á¬","Èã","Ï°",
"»Â","°¬","Óã","Èİ","Ïò","¹Å","Ò×","É÷","¸ê","ÁÎ","â×","ÖÕ","ôß","¾Ó","ºâ","²½","¶¼","¹¢","Âú","ºë","¿ï","¹ú","ÎÄ","¿Ü","¹ã","Â»",
"ãÚ","¶«","Å·","ì¯","ÎÖ","Àû","Îµ","Ô½" };



std::string getrandomname()
{
	std::string _tmp;
	int nPos_1 = rand() % 53;
	int nPos_2 = rand() %100 ;
	_tmp.append(surname[nPos_1]);
	_tmp.append(sec_name[nPos_2]);
	return _tmp;
}


using namespace Dio;

//StuInfo test[500000];
int main()
{

	//Éú³ÉÎÄµµµÄ´úÂë
	//FILE* fp;
	//
	//srand(time(NULL));
	//fp = fopen("STUINFO.txt", "wb+");
	//for (int i = 0; i < 500000; ++i)
	//{

	//	test[i]._class_num = rand() % (200 - 1 + 1);
	//	test[i]._stu_no = rand() % 1000001;
	//	test[i]._sex = rand() % (true - false + 1);
	//	test[i]._stu_name = getrandomname();
	//	fwrite(&test[i], sizeof(test[i]), 1, fp);
	//}
	//fclose(fp);
	//std::cout << "ÊäÈë¸ñÊ½£º¡°²éÑ¯µÄÄÚÈİ¡±+¡°¿Õ¸ñ¡±+¡°²éÑ¯µÄÀàĞÍ¡±£¬²éÑ¯µÄÀàĞÍÓĞName Class No" << std::endl << std::endl;

	Dio::Parser parser;
	Dio::FuncTable func;
	char input[20];
	do
	{
		std::cout << ">" ;
		std::cin.getline(input, 20);
		parser.Parse(input);
	

		
		if (strstr(input,"Find"))
		{
			Parser::findmode(func, parser, parser.GetType());
		}
		if (strstr(input,"Erase"))
		{
			Parser::erasemode(func, parser, parser.GetType());
		}
		if (strstr(input,"Modify"))
		{
			parser.modifymode(func);
		}
		if (strstr(input, "Add"))
		{
			parser.addmode(func);
		}
		func.Dump();
		parser.clear();
	} while (true);

	
	
	return 0;
}