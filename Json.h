#pragma once
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;
//#include "Info.h"
#define SPACE std::string(" ")
#define JSON_ASSERT(e) do {  throw std::runtime_error(e); } while (0)
class Json
{
public:
	enum Kind{
		kTrue, 
		kFalse , 
		kNull , 
		kObject, 
		kArray , 
		kString,
		kNumber
	};
	
	//构造和析构函数
	virtual ~Json(void);
	Json(void);
	Json(const Json& json);
	Json(const int& num);
	Json(const double& num);
	Json(const std::string& s);
	Json(const bool & b, Kind k);
	//针对数据结构的深度复制函数
	void Copy(const Json& json);
	Json& operator = (const Json& rhs);
	Json& operator = (const std::string& str);
	Json& operator = (const char* cstr);
	Json& operator[] (const char* key);
	//Object的映射
	const Json& operator[] (const char* key) const;
	//Array的索引
	Json& operator[] (const int& i);
	const Json& operator[] (const int& i) const;
	//添加json对象，增加后this就会变成Array对象
	Json& Add(const Json& json);
	//在Object中添加键值对
	Json& Add(std::pair<std::string, Json*> pair);
	Json& Add(const std::string& key, Json* value);
	Json& Add(const std::string& key, const Json& value);
	//根据索引删除Array的成员
	Json& Remove(const size_t & n);
	//删除键值对
	Json& Remove(const std::string & key);
	//获得所有键值
	std::vector<std::string> Keys() const;
	//获得表示类型的字符串
	std::string GetKindString();
	//获得对象的字符串形式
	std::string ToString();
	//获得有缩进的字符串形式
	std::string IndentString(const int & indent);
	std::string InnerIndentString(const int & indent, const int & level, bool chgLine);
	std::string IndentSpace(const int & indent, const int &level);
	friend ostream & operator << (ostream & output, Json& json);
	//获取对象的类型
	Kind GetKind();
	size_t Size();
	int GetInt() const;
	bool GetBool() const;
	double GetDouble() const ;
	std::string GetString() const;
	
	//用于外部使用，解析字符串
	static Json Parse(const char * s);
	//将json写入文件
	bool WriteFile(const char* file, const int & indent = 4);
	//从文件解析json
	static Json ParseFromFile(const char * file);
	
private:
	typedef std::vector<Json*> Array;
	typedef std::map<std::string, Json*> Object;
	typedef std::pair<std::string, Json*> Pair;

	Kind _kind; //表示json对象的类型
	void* _data; //实际数据内容
	void FreeMemory(); //释放内存
	struct Parser{
		size_t read_pos;
		std::string buf;
		char ch;
		Parser(const char * s);
		//读取token
		char NextChar(bool skip = true); 
		//读取索引后退一步
		void BackCharIndex();
		void ErrorHandle(int i);
		//预测token类型
		Kind KindPreview(char c);
		//解析string类型
		Json* ParseString();
		//解析number类型
		Json* ParseNumber();
		//解析array类型
		Json* ParseArray();
		//解析object类型
		Json* ParseObject();
		//解析true类型
		Json* ParseTrue();
		//解析false类型
		Json* ParseFalse();
		//主要的解析函数
		Json* ParsePrimary();
		//解析pair
		Json* ParsePair();
		
	};

public:
	Json(Array *a);
	Json(Pair* p);
	Json(Object* obj);
	Object GetObject();
};
