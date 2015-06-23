// Json_Parser.cpp : �������̨Ӧ�ó������ڵ㡣
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
		//���ļ�����json
		Json json = Json::ParseFromFile("e:/in.txt");
		//���ַ�������json
		Json sjson = Json::Parse("[\"a\", \"b df\",456, {\"dicdfk\": true, \"yup\" : \"yes\"} , \"pusdfd\"]");
		//�޸�
		sjson[0] = "sdfer";
		sjson[3]["yup"] = Json("WTF");
		//��
		sjson.Add(4656);
		//ɾ
		sjson.Remove(2);
		
		//û�����������
		cout << sjson.ToString() << endl;
		/*output:
		["sdfer","b df",{"dicdfk":true,"yup":"WTF"},"pusdfd",4656]
		*/

		//�����������, ��������Ϊ4���ո�
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
		///cout << sjson; //Ч��������Ϊ4���ո�����һ��
		
		//д���ļ�
		if(!sjson.WriteFile("e:/output.txt")){
			std::cout << "д���ļ�ʧ��" << endl;
		}
	}catch(exception &e){
		cout << "����" << e.what() << endl;
	}

	return 0;
}

