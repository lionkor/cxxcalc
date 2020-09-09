#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <variant>
#include <vector>

#define BigFloat long double

enum class Operator
{
    Add,
    Sub,
    Mult,
    Div,
    Mod
};

struct Token;

class Calculator
{
private:
    std::vector<Token> m_tokens;

    ssize_t find_highest_precedence_operator_index();

    void print_tokens() const;

    void replace_tokens(size_t from, size_t to, const Token& tok);


public:
    Calculator();

    bool parse(const std::string& expr);

    BigFloat execute();
};

struct Token {
    enum Type
    {
        Number,     // BigFloat
        Operator,   // ::Operator
        Parentheses // Calculator
    } type;

    std::variant<BigFloat, ::Operator, Calculator> data;
};

#endif // CALCULATOR_H
