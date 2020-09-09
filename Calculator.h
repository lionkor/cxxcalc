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

struct Token {
    enum Type
    {
        Number,
        Operator,
    } type;

    std::variant<BigFloat, ::Operator> data;
};

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

    std::string execute();
};

#endif // CALCULATOR_H
