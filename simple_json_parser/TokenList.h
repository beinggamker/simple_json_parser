#pragma once
#include "Scanner.h"
#include <vector>

class TokenIterator
{
public:
	virtual Token next() = 0;
	virtual Token current() = 0;
	virtual bool hasNext() const = 0;
	virtual ~TokenIterator() = default;
protected:
private:
};

//TokenListIterator类，实现迭代器接口
class TokenListIterator : public TokenIterator
{
public:
	TokenListIterator(std::vector<Token>::iterator begin, std::vector<Token>::iterator end) : current_(begin), end_(end) {}
	Token next() override {
		return *(current_++);
	}
	Token current() override {
		return *(current_);
	}
	bool hasNext() const override {
		return current_ != end_;
	}

private:
	std::vector<Token>::iterator current_;
	std::vector<Token>::iterator end_;
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

		iterator_ = getIterator();
	}
	Token next() override {
		//return m_tokens.at(index++);
		return iterator_->next();
	}
	Token current() override {
		return iterator_->current();
	}
	bool hasNext() const override {
		//return index < m_tokens.size();
		return iterator_->hasNext();
	}
	TokenIterator* getIterator() {
		return new TokenListIterator(m_tokens.begin(), m_tokens.end());
	}
protected:
private:
	std::vector<Token> m_tokens;
	size_t index = 0;
	TokenIterator* iterator_;
};

