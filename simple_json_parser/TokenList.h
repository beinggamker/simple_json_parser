#pragma once
#include "Scanner.h"
#include <vector>

class TokenIterator
{
public:
	virtual Token next() = 0;
	virtual bool hasNext() const = 0;
	virtual ~TokenIterator() = default;
protected:
private:
};

class TokenList : public TokenIterator
{
public:
	//TokenList(std::vector<Token>&& tokens) : m_tokens(std::move(tokens)){}
	TokenList(const std::string& input) {
		JsonLexer lexer(input);

		Token token = lexer.nextToken();
		while (token.getType() != TokenType::END_OF_FILE)
		{
			m_tokens.push_back(token);
			token = lexer.nextToken();
		}
	}
	Token next() override {
		return m_tokens.at(index++);
	}
	bool hasNext() const override {
		return index < m_tokens.size();
	}
protected:
private:
	std::vector<Token> m_tokens;
	size_t index = 0;
};

