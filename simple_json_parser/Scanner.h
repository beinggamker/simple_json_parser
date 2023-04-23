#pragma once
#include <iostream>
#include <string>
#include <vector>

//定义Token的类型
enum class TokenType {
	LEFT_BRACE,   // {
	RIGHT_BRACE,  // }
	LEFT_BRACKET,  // [
	RIGHT_BRACKET, // ]
	COLON,        // :
	COMMA,        // ,
	STRING,       // 字符串
	TRUE,         // true
	FALSE,        // false
	NULL_TOKEN,   // null
	END_OF_FILE,  // 文件结束
	NUMBER      // 数字
};

// Token结构体,用于保存token类型和值
struct Token {
	TokenType type;    //类型
	std::string value;      //值
	Token(TokenType type, const std::string& value) : type(type), value(value) {}
	TokenType getType() const;
	friend std::ostream& operator<<(std::ostream& os, const TokenType& type);
};

TokenType Token::getType() const{
	return type;
}


// JsonLexer类,解析JSON字符串并且生成Token序列
class JsonLexer {
public:
	JsonLexer(const std::string& input) : input_(input), current_pos_(0){}

	
	// 获取下一个Token
	Token nextToken() {
		skipWhiteSpace();

		if (isEndOfFile())
		{
			return Token(TokenType::END_OF_FILE, "");
		}

		char c = getCurrentChar();
		if (c == '{') 
		{
			advance();
			return Token(TokenType::LEFT_BRACE, "{");
		}
		else if (c == '}')
		{
			advance();
			return Token(TokenType::RIGHT_BRACE, "}");
		}
		else if (c == '[')
		{
			advance();
			return Token(TokenType::LEFT_BRACKET, "[");
		}
		else if (c == ']')
		{
			advance();
			return Token(TokenType::RIGHT_BRACKET, "]");
		}
		else if (c == ':')
		{
			advance();
			return Token(TokenType::COLON, ":");
		}
		else if (c == ',')
		{
			advance();
			return Token(TokenType::COMMA, ",");
		}
		else if (c == '"')
		{
			return readString();
		}
		else if (c == 't')
		{
			return readTrue();
		}
		else if (c == 'f')
		{
			return readFalse();
		}
		else if (c == 'n')
		{
			return readNull();
		}
		else if (isdigit(c) || c == '-')
		{
			return readNumber();
		}
		else
		{
			throw std::runtime_error("Unexpected character: " + c);
		}
	}
private:
	//输入的字符串
	const std::string input_;
	//当前位置
	size_t current_pos_;

	//获取当前字符
	char getCurrentChar() {
		return input_[current_pos_];
	}

	//前进一个字符
	void advance() {
		current_pos_++;
	}

	// 获取输入流中的下一个字符，但不将其消耗
	char peek() const {
		if (current_pos_ < input_.length()) {
			return input_[current_pos_];
		}
		else {
			return '\0';
		}
	}

	// 获取输入流中的下一个字符，并将其消耗
	char consume() {
		char ch = peek();
		current_pos_++;
		return ch;
	}

	//跳过空白字符
	void skipWhiteSpace() {
		while (isspace(getCurrentChar()))
		{
			advance();
		}
	}

	//是否到达文件末尾
	bool isEndOfFile() {
		return current_pos_ >= input_.size();
	}

	// 读取转义字符
	char readEscape() {
		advance();

		char c = getCurrentChar();
		switch (c) {
		case 'n': advance(); return '\n';
		case 'r': advance(); return '\r';
		case 't': advance(); return '\t';
		case 'b': advance(); return '\b';
		case 'f': advance(); return '\f';
		case '\\': advance(); return '\\';
		case '/': advance(); return '/';
		case '"': advance(); return '"';
		case '\'': advance(); return '\'';
		default: return c;
		}
	}

	//读取文件字符串
	Token readString() {
		std::string value;
		char ch;
		advance();
		while (true)
		{
			ch = getCurrentChar();

			if (ch == '\\')
			{
				value += readEscape();
			}
			else if (ch == '"')
			{
				advance();
				return Token(TokenType::STRING, value);
			}
			else if (ch == '\0')
			{
				throw std::runtime_error("Unexpected end of string");
			}
			else
			{
				value += ch;
				advance();
			}
		}
	}

	Token readNumber() {
		std::string result;

		if (peek() == '-') {
			result += consume();
		}

		while (std::isdigit(peek())) {
			result += consume();
		}

		if (peek() == '.') {
			result += consume();

			while (std::isdigit(peek())) {
				result += consume();
			}
		}

		if (peek() == 'e' || peek() == 'E') {
			result += consume();

			if (peek() == '+' || peek() == '-') {
				result += consume();
			}

			while (std::isdigit(peek())) {
				result += consume();
			}
		}

		return Token(TokenType::NUMBER, result);
	}
	
	Token readTrue() {
		// 读取 'true' 字符串
		if (consume() == 't' &&
			consume() == 'r' &&
			consume() == 'u' &&
			consume() == 'e') {
			return Token(TokenType::TRUE, "true");
		}
		else {
			throw std::runtime_error("Invalid token");
		}
	}

	Token readFalse() {
		// 读取 'false' 字符串
		if (consume() == 'f' &&
			consume() == 'a' &&
			consume() == 'l' &&
			consume() == 's' &&
			consume() == 'e') {
			return Token(TokenType::FALSE, "false");
		}
		else {
			throw std::runtime_error("Invalid token");
		}
	}

	Token readNull() {
		// 读取 'null' 字符串
		if (consume() == 'n' &&
			consume() == 'u' &&
			consume() == 'l' &&
			consume() == 'l') {
			return Token(TokenType::NULL_TOKEN, "null");
		}
		else {
			throw std::runtime_error("Invalid token");
		}
	}
};