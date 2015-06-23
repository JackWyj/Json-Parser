# Json-Parser

这是参考 ggicci--json（https://github.com/ggicci/ggicci--json） 的一个基于C++的json工具库, 而与之不同的是，解析代码是自己写的，参考的是接口的设计。
其主要功能是能够解析字符串形式的json, 形成C++对象，能够对该对象进行增删改等操作，如下代码所示


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
		
	
