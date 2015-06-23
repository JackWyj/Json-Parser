# Json-Parser
##简介
这是参考[ggicci--json](https://github.com/ggicci/ggicci--json)的一个基于C++的json工具库, 而与之不同的是，解析代码是自己写的，参考的是接口的设计。<br>
其主要功能是能够解析字符串形式的json, 形成C++对象，能够对该对象进行增删改等操作，如下代码所示



##构造对象
```cpp
//从字符串构造json
Json sjson = Json::Parse("[\"a\", \"b df\",456, {\"dicdfk\": true, \"yup\" : \"yes\"} , \"pusdfd\"]");
/*该json对象如下所示
[
	"a",
	"b df",
	456,
	{
		"dicdfk":true,
		"yup":"yes"
	},
	"pusdfd"
]
*/
//从文件构造json
Json json = Json::ParseFromFile("e:/in.txt");
```
`static Json Json::Parse(const char * str)`从字符串中解析json对象<br>
`static Json Json::ParseFromFile(const char * file)` 从文件中读取字符串，然后解析

##修改
```cpp
sjson[0] = "sdfer";
sjson[3]["yup"] = Json("WTF");
/*修改后该json对象如下所示
[
	"sdfer",
	"b df",
	456,
	{
		"dicdfk":true,
		"yup":"WTF"
	},
	"pusdfd"
]
*/
```
##增删
```cpp
sjson.Add(4656);
sjson.Remove(2);
/*修改后该json对象如下所示
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
```
##打印	
```cpp
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
cout << sjson; //效果跟缩进为4个空格的输出一样
```
##文件读写
```cpp
//从文件构造json
Json json = Json::ParseFromFile("e:/in.txt");
//写入文件
if(!sjson.WriteFile("e:/output.txt")){
	std::cout << "写入文件失败" << std::endl;
}
```
`bool Json::WriteFile(const char * file)` 将json写入指定文件，操作成功就返回`true`，否则返回`false`
##编译
这是用VS2012写的，整个工程比较大，这里只有源码，如果你要使用该代码，请在能够接受的版本的VS上新建工程然后添加。
##Thanks
谢谢[ggicci](https://github.com/ggicci)，我从你的代码学了很多东西。

