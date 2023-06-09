#include <iostream>
#include <string>
#include "Scanner.h"
#include "TokenList.h"
#include "Parser.h"

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
	switch (type) {
	case TokenType::LEFT_BRACE:
		os << "LEFT_BRACE";
		break;
	case TokenType::RIGHT_BRACE:
		os << "RIGHT_BRACE";
		break;
	case TokenType::LEFT_BRACKET:
		os << "LEFT_BRACKET";
		break;
	case TokenType::RIGHT_BRACKET:
		os << "RIGHT_BRACKET";
		break;
	case TokenType::COMMA:
		os << "COMMA";
		break;
	case TokenType::COLON:
		os << "COLON";
		break;
	case TokenType::STRING:
		os << "STRING";
		break;
	case TokenType::NUMBER:
		os << "NUMBER";
		break;
	case TokenType::TRUE:
		os << "TRUE";
		break;
	case TokenType::FALSE:
		os << "FALSE";
		break;
	case TokenType::NULL_TOKEN:
		os << "NULL";
		break;
	case TokenType::END_OF_FILE:
		os << "END_OF_FILE";
		break;
	default:
		os << "INVALID";
		break;
	}
	return os;
}

int main() {
	std::string json = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";
	auto source = R"(
		{
			"name": "John",
			"age": 30,
			"address": "123 Main St, Any town USA"
		}
	)";
	//JsonLexer lexer(source);

	//Token token = lexer.nextToken();
	//while (token.getType() != TokenType::END_OF_FILE)
	//{
	//	std::cout << "Token type: " << token.getType() << std::endl;
	//	std::cout << "Token value: " << token.value << std::endl;
	//	token = lexer.nextToken();
	//}

	TokenList tokens(source);

	//使用迭代器或者循环来遍历,出现了“absort has been called”的情况
	while (tokens.hasNext())
	{
		Token token = tokens.next();
		std::cout << "Token type: " << token.getType() << std::endl;
		std::cout << "Token value: " << token.value << std::endl;
	}

	//如果想要检查我的解析器是否正常工作，我怕就需要将它们解析成JsonValue对象
	//再将JsonValue对象转换成字符串
	JsonParser parse(source);
	JsonValue* value = parse.parse();


	return 0;
}