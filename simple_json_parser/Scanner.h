#pragma once
#include <iostream>
#include <string>
#include <vector>

//����Token������
enum class TokenType {
	LEFT_BRACE,   // {
	RIGHT_BRACE,  // }
	LEFT_BRACKET,  // [
	RIGHT_BRACKET, // ]
	COLON,        // :
	COMMA,        // ,
	STRING,       // �ַ���
	TRUE,         // true
	FALSE,        // false
	NULL_TOKEN,   // null
	END_OF_FILE,  // �ļ�����
	NUMBER      // ����
};

// Token�ṹ��,���ڱ���token���ͺ�ֵ
struct Token {
	TokenType type;    //����
	std::string value;      //ֵ
	Token(TokenType type, const std::string& value) : type(type), value(value) {}
	TokenType getType() const;
	friend std::ostream& operator<<(std::ostream& os, const TokenType& type);
};

TokenType Token::getType() const{
	return type;
}


// JsonLexer��,����JSON�ַ�����������Token����
class JsonLexer {
public:
	JsonLexer(const std::string& input) : input_(input), current_pos_(0){}

	
	// ��ȡ��һ��Token
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
	//������ַ���
	const std::string input_;
	//��ǰλ��
	size_t current_pos_;

	//��ȡ��ǰ�ַ�
	char getCurrentChar() {
		return input_[current_pos_];
	}

	//ǰ��һ���ַ�
	void advance() {
		current_pos_++;
	}

	// ��ȡ�������е���һ���ַ���������������
	char peek() const {
		if (current_pos_ < input_.length()) {
			return input_[current_pos_];
		}
		else {
			return '\0';
		}
	}

	// ��ȡ�������е���һ���ַ�������������
	char consume() {
		char ch = peek();
		current_pos_++;
		return ch;
	}

	//�����հ��ַ�
	void skipWhiteSpace() {
		while (isspace(getCurrentChar()))
		{
			advance();
		}
	}

	//�Ƿ񵽴��ļ�ĩβ
	bool isEndOfFile() {
		return current_pos_ >= input_.size();
	}

	// ��ȡת���ַ�
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

	//��ȡ�ļ��ַ���
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
		// ��ȡ 'true' �ַ���
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
		// ��ȡ 'false' �ַ���
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
		// ��ȡ 'null' �ַ���
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