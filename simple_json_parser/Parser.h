#pragma once
#include <iostream>
#include <string>
#include <unordered_map> //json������Ա�ʾΪһ��std::unordered_map<std::string,JsonValue>
#include <vector>
#include <stdexcept>
#include "TokenList.h"

class JsonValue {
public:
	virtual ~JsonValue() = default;
};

class JsonNull : public JsonValue {};

class JsonBoolean : public JsonValue {
public:
	JsonBoolean(bool value) : value_(value){}
	bool getvalue() const { return value_; }
private:
	bool value_;
};

class JsonNumber : public JsonValue {
public:
	JsonNumber(double value) : value_(value){}
	double getvalue() const { return value_; }
protected:
private:
	double value_;
};

class JsonString : public JsonValue {
public:
	JsonString(std::string value) : value_(std::move(value)){}
	const std::string& getvalue() const { return value_; }
private:
	std::string value_;
};

class JsonArray : public JsonValue {
public:
	void append(JsonValue* value) { values_.emplace_back(value); }
	const std::vector<JsonValue*>& getvalues() const { return values_; }
private:
	std::vector<JsonValue*> values_;
};

class JsonObject : public JsonValue{
public:
	void set(const std::string& key, JsonValue* value) { values_[key] = value; }
	const std::unordered_map<std::string, JsonValue*>& getvalues() const { return values_; };
private:
	std::unordered_map<std::string, JsonValue*> values_;
};

class JsonParser {
public:
	//Ϊ�˷�ֹ������������������ת������Ҫ��ʽ�ĵ��ù��캯��
	explicit JsonParser(const std::string& json) : tokens_(json){}
	JsonValue* parse() {
		TokenIterator* it = tokens_.getIterator();
		return parseValue(it);
	}
private:
	TokenList tokens_;
	JsonValue* parseValue(TokenIterator* it) {
		Token token = it->next();
		switch (token.getType())
		{
		case TokenType::NULL_TOKEN:
			return new JsonNull();
		case TokenType::TRUE:
			return new JsonBoolean(token.value == "true");//��һ����bool�ж��ǲ���������
		case TokenType::FALSE:
			return new JsonBoolean(token.value == "true");
		case TokenType::NUMBER:
			return new JsonNumber(std::stod(token.value));
		case TokenType::STRING:
			return new JsonString(token.value);//��Ӧ��дһ���ӿڵģ���������value��������˵
		case TokenType::LEFT_BRACKET:
			return parseArray(it);
		case TokenType::LEFT_BRACE:
			return parseObject(it);
		default:
			throw std::runtime_error("unexpected token");
		}
	}

	JsonArray* parseArray(TokenIterator* it) {
		JsonArray* array = new JsonArray();
		Token token = it->next();
		while (token.getType() != TokenType::RIGHT_BRACKET)
		{
			JsonValue* value = parseValue(it);
			array->append(value);


			//��Ҫ�������������һ���ӿ�
			token = it->current();
			if (token.getType() == TokenType::COMMA)
			{
				it->next();
				token = it->current();
			}//�ҵðѿ��ܵ��������ȫ����
			else if (token.getType() != TokenType::RIGHT_BRACKET)
			{
				throw std::runtime_error("expected comma or right bracket");
			}
		}

		return array;
	}

	JsonObject* parseObject(TokenIterator* it) {
		JsonObject* jsonObject = new JsonObject();

		while (true)
		{
			Token token = it->current();
			if (token.getType() == TokenType::RIGHT_BRACE)
			{
				it->next();
				break;
			}

			//��
			if (token.getType() != TokenType::STRING)
			{
				throw std::runtime_error("expected string for object key");
			}
			std::string key = token.value; //���value��û��д�ӿ�
			it->next();

			//ð��
			token = it->current();
			if (token.getType() != TokenType::COLON)
			{
				throw std::runtime_error("expected colon after object key");
			}

			//ֵ
			JsonValue* value = parseValue(it);
			jsonObject->set(key, value);

			//���ţ������Ҵ�����
			token = it->current();
			if (token.getType() == TokenType::RIGHT_BRACE)
			{
				it->next();
				break;
			}
			else if (token.getType() == TokenType::COMMA)
			{
				it->next();
			}
			else {
				throw std::runtime_error("expected comma or right brace after object");
			}
		}
		return jsonObject;
	}
};