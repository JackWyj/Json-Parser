// Json_Parser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Json.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
		
	try{
		//从文件构造json
		Json json = Json::ParseFromFile("e:/in.txt");
		//从字符串构造json
		Json sjson = Json::Parse("[\"a\", \"b df\",456, {\"dicdfk\": true, \"yup\" : \"yes\"} , \"pusdfd\"]");
		//修改
		sjson[0] = "sdfer";
		sjson[3]["yup"] = Json("WTF");
		//增
		sjson.Add(4656);
		//删
		sjson.Remove(2);
		
		//没有缩进的输出
		cout << sjson.ToString() << endl;
		/*output:
		["sdfer","b df",{"dicdfk":true,"yup":"WTF"},"pusdfd",4656]
		*/

		//有缩进的输出, 设置缩进为4个空格
		cout << sjson.IndentString(4) << endl;
		/*output:
		[
			"sdfer",
			"b df",
			{
				"dicdfk":true,
				"yup":"WTF"
			},
			"pusdfd",
			4656
		]
		*/
		///cout << sjson; //效果跟缩进为4个空格的输出一样
		
		//写入文件
		if(!sjson.WriteFile("e:/output.txt")){
			std::cout << "写入文件失败" << endl;
		}
	}catch(exception &e){
		cout << "错误：" << e.what() << endl;
	}

	return 0;
}

