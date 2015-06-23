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
	
	//�������������
	virtual ~Json(void);
	Json(void);
	Json(const Json& json);
	Json(const int& num);
	Json(const double& num);
	Json(const std::string& s);
	Json(const bool & b, Kind k);
	//������ݽṹ����ȸ��ƺ���
	void Copy(const Json& json);
	Json& operator = (const Json& rhs);
	Json& operator = (const std::string& str);
	Json& operator = (const char* cstr);
	Json& operator[] (const char* key);
	//Object��ӳ��
	const Json& operator[] (const char* key) const;
	//Array������
	Json& operator[] (const int& i);
	const Json& operator[] (const int& i) const;
	//���json�������Ӻ�this�ͻ���Array����
	Json& Add(const Json& json);
	//��Object����Ӽ�ֵ��
	Json& Add(std::pair<std::string, Json*> pair);
	Json& Add(const std::string& key, Json* value);
	Json& Add(const std::string& key, const Json& value);
	//��������ɾ��Array�ĳ�Ա
	Json& Remove(const size_t & n);
	//ɾ����ֵ��
	Json& Remove(const std::string & key);
	//������м�ֵ
	std::vector<std::string> Keys() const;
	//��ñ�ʾ���͵��ַ���
	std::string GetKindString();
	//��ö�����ַ�����ʽ
	std::string ToString();
	//������������ַ�����ʽ
	std::string IndentString(const int & indent);
	std::string InnerIndentString(const int & indent, const int & level, bool chgLine);
	std::string IndentSpace(const int & indent, const int &level);
	friend ostream & operator << (ostream & output, Json& json);
	//��ȡ���������
	Kind GetKind();
	size_t Size();
	int GetInt() const;
	bool GetBool() const;
	double GetDouble() const ;
	std::string GetString() const;
	
	//�����ⲿʹ�ã������ַ���
	static Json Parse(const char * s);
	//��jsonд���ļ�
	bool WriteFile(const char* file, const int & indent = 4);
	//���ļ�����json
	static Json ParseFromFile(const char * file);
	
private:
	typedef std::vector<Json*> Array;
	typedef std::map<std::string, Json*> Object;
	typedef std::pair<std::string, Json*> Pair;

	Kind _kind; //��ʾjson���������
	void* _data; //ʵ����������
	void FreeMemory(); //�ͷ��ڴ�
	struct Parser{
		size_t read_pos;
		std::string buf;
		char ch;
		Parser(const char * s);
		//��ȡtoken
		char NextChar(bool skip = true); 
		//��ȡ��������һ��
		void BackCharIndex();
		void ErrorHandle(int i);
		//Ԥ��token����
		Kind KindPreview(char c);
		//����string����
		Json* ParseString();
		//����number����
		Json* ParseNumber();
		//����array����
		Json* ParseArray();
		//����object����
		Json* ParseObject();
		//����true����
		Json* ParseTrue();
		//����false����
		Json* ParseFalse();
		//��Ҫ�Ľ�������
		Json* ParsePrimary();
		//����pair
		Json* ParsePair();
		
	};

public:
	Json(Array *a);
	Json(Pair* p);
	Json(Object* obj);
	Object GetObject();
};
