#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <variant>
#include <vector>

#define BigFloat long double

enum class TokenType
{
    Number,      // BigFloat
    Operator,    // ::Operator
    Parentheses, // Calculator
    Sign,
};

enum class Operator
{
    Add,
    Sub,
    Mult,
    Div,
};

enum class Sign
{
    Positive,
    Negative
};

struct Token;



class Calculator
{
private:
    std::vector<Token> m_tokens;

    ssize_t find_highest_precedence_operator_index();
    ssize_t find_next_sign_index();

    std::vector<Token>::iterator find_token_sequence(std::initializer_list<TokenType> list);

    void print_tokens() const;

    void replace_tokens(size_t from, size_t to, const Token& tok);
    void replace_tokens(std::vector<Token>::const_iterator from, std::vector<Token>::const_iterator to, const Token& tok);


public:
    Calculator();

    bool parse(const std::string& expr);

    BigFloat execute();
};

struct Token {
    using Type = TokenType;
    Type type;
    std::variant<BigFloat, Operator, Calculator, Sign> data;
};

#endif // CALCULATOR_H
