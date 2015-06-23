
#include "stdafx.h"
#include "Json.h"


Json::Json(void): _kind(kNull), _data(NULL){
}

Json::~Json(){
	if(_data == NULL) return;	
	FreeMemory();
}

Json::Json(Pair* p): _kind(kObject), _data(p) {}

Json::Json(Array *a) : _kind(kArray), _data(a) {}

Json::Json(Object* obj): _kind(kObject), _data(obj) {}

Json::Json(const bool& b, Kind k): _kind(k), _data(new bool(b)) {}

Json::Json(const int& num): _kind(kNumber), _data(new double(num)) {}

Json::Json(const double& num): _kind(kNumber), _data(new double(num)) {}

Json::Json(const std::string & s): _kind(kString), _data(new std::string(s)) {}

Json::Json(const Json& json){
	//TRACK("COPY");
	Copy(json);
}

void Json::Copy(const Json& json){
	_kind = json._kind;
	switch(json._kind){
	case kNull: {  _data = NULL; break;}
	case kNumber: {  _data = new double(json.GetDouble()); break; } 
	case kString: {  _data = new std::string(json.GetString()); break; }
	case kTrue: case kFalse: { _data = new bool(json.GetBool());break;  }
	case kArray: 
		{
			Array * thisArr = new Array;
			Array& arr = *(Array*)json._data;
			for(Array::iterator it = arr.begin(); it != arr.end(); it++){
				thisArr->push_back(new Json(*(*it)) );
			}
			_data = thisArr;
			break;
		}
	case kObject: 
		{
			Object* thisObj = new Object;
			Object& obj = *(Object*)json._data;
			for(Object::iterator it = obj.begin(); it != obj.end(); it++){
				Pair p;
				p.first = it->first; 
				p.second = new Json(*(it->second));
				thisObj->insert(p);
			}
			_data = thisObj;
			break;
		}
	default: break;
	}
}

int Json::GetInt () const{
	if(_kind == kNumber){
		return *((int*)_data);
	}else
		JSON_ASSERT("json of number expected");
}

bool Json::GetBool() const{
	if(_kind != kTrue &&_kind != kFalse)
		JSON_ASSERT("json of bool expected");
	return *(bool*)_data;
}

Json::Kind Json::GetKind(){
	return _kind;
}

double Json::GetDouble() const{
	if(_kind != kNumber)
		JSON_ASSERT("json of number expected");
	else
		return *((double*)_data);
}

std::string Json::GetString() const{
	if(_kind != kString)
		JSON_ASSERT("json of string expected");
	else
		return *((std::string*)_data);
}

std::string Json::GetKindString(){
	std::string skind = "";
	switch(_kind){
	case kArray: { skind = "Array"; break; }
	case kFalse: { skind = "False"; break; }
	case kTrue: { skind = "True"; break; }
	case kNumber: { skind = "Number"; break; }
	case kObject: { skind = "Object"; break; }
	case kString : { skind = "String"; break; }
	case kNull: { skind = "Null"; break; }
	}
	return skind;
}

Json::Object Json::GetObject(){
	if(_kind != kObject)
		JSON_ASSERT("json of object expected");
	return *(Object*)_data;
}

Json& Json::operator[](const char* key){
	if(_kind != kObject)
		JSON_ASSERT("json of object expected");
	Json & json = *(*(Object*)_data)[key];
	return *(*(Object*)_data)[key];
}

const Json& Json::operator[](const char* key) const{
	if(_kind != kObject)
		JSON_ASSERT("json of object expected");
	return *(*(Object*)_data)[key];
}

Json& Json::operator[] (const int& num){
	if(_kind != kArray)
		JSON_ASSERT("json of array expected");
	Json & json = *(*(Array*)_data)[num];
	return *(*(Array*)_data)[num];
}

const Json & Json::operator [] (const int& num) const {
	if(_kind != kArray)
		JSON_ASSERT("json of array expected");
	return *(*(Array*)_data)[num];
}

Json& Json::operator  = (const std::string & str){
	_kind = kString;
	_data = new string(str);
	return *this;
}

Json& Json::operator = (const char* str){
	_kind = kString;
	_data = new string(str);
	return *this;
}

Json& Json::operator = (const Json& rhs){
	if (this == &rhs) { return *this; }
	FreeMemory();
	Copy(rhs);
	return *this;
}

std::string Json::ToString( ){
	std::ostringstream ostr;
	switch(_kind){
		case kNull: { ostr << "Null" ; break; }
		case kNumber: { ostr << GetDouble() ; break; }
		case kString: { ostr << "\"" << GetString() << "\""; break; }
		case kTrue: case kFalse: { ostr << boolalpha << GetBool() ; break; }
		case kObject: 
		{
			ostr << "{";
	
			Object* obj = (Object*)_data;
			Object::const_iterator iter = obj->begin() ;
			size_t size = obj->size(), ind = 0;
			for(; iter != obj->end(); iter++){
				ostr << "\"" << iter->first << "\"" << ":" << iter->second->ToString();
				if(++ind < size) ostr << ",";
			}
		
			ostr << "}";
			break;
		}
		case kArray:
		{
			ostr << "[";
			Array& arr = *(Array*)_data;
			size_t size = arr.size(), ind = -1;

			for(; ++ind < size;){
				ostr << arr[ind]->ToString();
				if(ind+1 < size) ostr << ",";
			}

			ostr << "]";
			break;
		}
		default: break;

	}
	return ostr.str();
}

std::string Json::IndentString(const int & indent){
	return InnerIndentString(indent, 0, true);
}

std::string Json::InnerIndentString(const int & indent, const int &level, bool chgLine){
	std::string ret;
	if(chgLine)
		ret += "\n" + IndentSpace(indent, level);
	switch(_kind){
	case kString: case kNumber: case kTrue: case kFalse: case kNull:
		{
			ret += ToString(); break;
		}
	case kArray: 
		{	
			ret += "[";			
			Array& arr = *(Array*)_data;
			size_t size = arr.size(), ind = -1;
			for(; ++ind < size;){
				ret += arr[ind]->InnerIndentString(indent, level+1, true);
				if(ind+1 < size) ret += ",";
			}
			ret += "\n" + IndentSpace(indent, level) + "]";
			break; 
		}
	case kObject: 
		{
			ret += "{";
			Object* obj = (Object*)_data;
			Object::const_iterator iter = obj->begin() ;
			size_t size = obj->size(), ind = 0;
			for(; iter != obj->end(); iter++){
				ret += "\n" +  IndentSpace(indent, level+1) + std::string("\"") +
					  iter->first +  std::string("\"") + ":";
				ret += iter->second->InnerIndentString(indent, level+1, false);
				if(++ind < size) ret += ",";
			}
			ret += "\n" + IndentSpace(indent, level) + "}";
			break;
		}
	default: break;
	}
	return ret;
}

std::string Json::IndentSpace(const int& indent, const int & level){
	std::string ret = "";
	for(int i = 0; i < indent * level; i++){
		ret += SPACE;
	}
	return ret;
}

void Json::FreeMemory(){
	switch(_kind){
	case kNumber: { double* ptr = (double*)_data; delete ptr; break; }
	case kString: { std::string* ptr = (std::string*)_data; delete ptr; break; }
	case kFalse: case kTrue: { bool* ptr = (bool*)_data; delete ptr; break; }
	case kArray: 
		{
			Array& arr = *(Array*)_data;
			Array::iterator iter = arr.begin();
			for(; iter != arr.end(); iter++){
				delete *iter;
				*iter = NULL;
			}
			break;
		}
	case kObject:
		{
			Object* obj = (Object*)_data;
			Object::iterator iter = obj->begin();
			for(; iter != obj->end(); iter++){
				delete iter->second;
				iter->second = NULL;
			}
			break;
		}
	default: break;
	}
	_data = NULL;
	_kind = kNull;
}

size_t Json::Size(){
	size_t size = 0;
	switch(_kind){
	case kNull:  { size = 0; break;}
	case kNumber: case kFalse: case kTrue: case kString: 
		{ size = 1; break;}
	case kArray: { size = ((Array*)_data)->size(); break; }
	case kObject: { size = ((Object*)_data)->size(); break; }
	default: break;
	}
	return size ;
}

std::vector<std::string> Json::Keys() const {
	std::vector<std::string> vkeys;
	if(_kind == kObject) {
		Object& obj = *(Object*)_data;
		Object::const_iterator it = obj.begin();
		for(; it != obj.end(); it++){
			vkeys.push_back(it->first);
		}
	}
	return vkeys;
}

Json & Json::Add(const Json& json){
	switch(_kind){
	case kArray:
		{
			Json * zjson = new Json(json);
			((Array*)_data)->push_back(zjson);
			break;
		}
	case kNull: case kTrue: case kFalse: case kString: case kObject:
		{
			_kind = kArray;
			Array * thisArr = new Array;
			thisArr->push_back(new Json(*this));
			thisArr->push_back(new Json(json));
			_data = thisArr;
			break;
		}
	default: break;
	}
	return *this;
}

Json& Json::Add(std::pair<std::string, Json*> pair){
	if(kObject == _kind &&	pair.first.size() > 0
		&&  pair.second != NULL && pair.second->_kind != kNull) {
		((Object*)_data)->insert(pair);
	}
	return *this;
}

Json& Json::Add(const std::string& key, Json* value){
	if(key.size() > 0 && value->_kind != kNull){
		((Object*)_data)->insert(make_pair(key, value));
	}
	return *this;
}

Json& Json::Add(const std::string& key, const Json& value){
	if(key.size() > 0 && value._kind != kNull){
		if(_kind != kObject) 
			JSON_ASSERT("json of object expected");
		((Object*) _data)->insert(make_pair(key, new Json(value)));
	}
	return *this;
}

Json& Json::Remove(const size_t & n){
	if(_kind != kArray) 
		JSON_ASSERT("json of array expected");
	Array & arr = *(Array*)_data;
	if(n >= 0 && n < arr.size()){
		Array::iterator it = arr.begin() + n;
		delete *it;
		arr.erase(it);
	}
	return *this;
}

Json& Json::Remove(const std::string & key){
	if(_kind != kObject)
		JSON_ASSERT("json of object expected");
	Object& obj = *(Object*)_data;
	Object::iterator it = obj.find(key);
	if (it != obj.end())
	{
		delete it->second;
		obj.erase(it);
	}
	return *this;
}

ostream & operator << (ostream & output, Json & json){
	output << json.IndentString(4);
	return output;
}

Json Json::Parse(const char * s){
	Parser parse(s);
	Json* root = parse.ParsePrimary();
	Json json(*root);
	delete root;
	return json;
}

bool Json::WriteFile(const char* file, const int & indent){
	std::ofstream out(file);
	if(!out.is_open()) 
		return false;
	out << IndentString(indent);
	out.close();
	return true;
}

Json Json::ParseFromFile(const char * file){
	std::ifstream in(file); 
    std::istreambuf_iterator<char> beg(in), end; 
    std::string str(beg, end); 
	if(str.size() == 0) return Json();
	return Parse(str.c_str());
}

/*以下是Json::Parser的一些函数实现*/

char Json::Parser::NextChar(bool skip){
	
	while (true)
	{
		read_pos++;
		if(read_pos >= buf.size()) JSON_ASSERT("unexpected end of input");
		ch = buf[read_pos];
		if (skip && (' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch)) { ; }
		else break;
	}

	return ch;
}

Json::Kind Json::Parser::KindPreview(char c){
	Kind kind;
	switch(c){
	case '\"':{ kind = kString; break;}
	case 't': { kind = kTrue; break;}
	case 'f': { kind = kFalse; break;}
	case '{': { kind = kObject; break;}
	case '[': { kind = kArray; break;}
	case '0': case '1': case '2': case '3': 
	case '4': case '5': case '6': 
	case '7': case '8': case '9':
		{
			kind = kNumber; break;
		}
	default:  { kind = kNull; break;}
	};
	return kind;
}

void Json::Parser::ErrorHandle(int t){

}

void Json::Parser::BackCharIndex(){
	--read_pos;
}

Json::Parser::Parser(const char * s){
	buf = std::string(s);
	read_pos = -1;
}

Json* Json::Parser::ParsePrimary(){
	NextChar();
	Kind kind = KindPreview(ch);
	Json* json = NULL;
	switch(kind){
	case kString: { json = ParseString(); break;}
	case kNumber: { json = ParseNumber(); break;}
	case kFalse: { json = ParseFalse(); break; }
	case kTrue: { json = ParseTrue(); break; }
	case kArray: { json = ParseArray(); break; }
	case kObject: { json = ParseObject(); break; }
	case kNull: break;
	default: break;
	};
	return json;
}

Json* Json::Parser::ParseString(){
	std::string s = "";
	NextChar();
	while(ch != '\"'){
		s += ch;
		NextChar(false);
	}
	return new Json(s);
}

Json* Json::Parser::ParseNumber(){
	std::string snum = "";
	bool isDouble = false;
	while(isdigit(ch) || ch == '.'){
		if(ch == '.') {
			if(isDouble) JSON_ASSERT("unexpected char \".\" came out again");
			isDouble = true;
		}else
			snum += ch;
		NextChar();
	}
	BackCharIndex();	
	if(isDouble){
		double num = atof(snum.c_str());
		return new Json(num);
	}else{
		int num = atoi(snum.c_str());
		return new Json(num);
	}
}

Json* Json::Parser::ParseTrue(){
	std::string sbool;
	sbool += ch;
	for(int i = 0; i < 3; i++)
		sbool += NextChar();		
	if(sbool != "true"){
		JSON_ASSERT("expect \"true\" ");
		return NULL;
	}
	else return new Json(true, kTrue);
}

Json* Json::Parser::ParseFalse(){
	std::string sbool;
	for(int i = 0; i < 4; i++)
		sbool += NextChar();
	if(sbool != "false"){
		JSON_ASSERT("expect \"false\" ");
		return NULL;
	}
	else return new Json(false, kFalse);
}

Json* Json::Parser::ParseArray(){
	NextChar();
	Array* array = new Array;
	Json* member;
	while(ch != ']'){
		BackCharIndex();
		member = ParsePrimary();
		array->push_back(member);
		NextChar();
		if(ch == ',') NextChar();
	}
	return new Json(array);
}

Json* Json::Parser::ParsePair(){
	Json* jstring = ParsePrimary();
	if(jstring->_kind != kString) JSON_ASSERT("json of string expected");
	
	NextChar();
	if(ch != ':') JSON_ASSERT("expect \":\"");
	Json* value = ParsePrimary();
	Pair* pairs = new Pair(*(std::string*)(jstring->_data), value);
	return new Json(pairs);
}

Json* Json::Parser::ParseObject(){
	NextChar();
	Object obj;
	Json* p;
	while(ch != '}'){
		BackCharIndex();
		p = ParsePair();

		if(p) {
			if(p->_kind == kObject)
				obj.insert(*(Pair*)(p->_data));
			else 
				JSON_ASSERT("json of object expected");
		}
		NextChar(true);
		if(ch == ',') NextChar();
	}
	return new Json(new Object(obj));
}

