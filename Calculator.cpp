#include "Calculator.h"

#include <algorithm>
#include <iostream>
#include <cassert>
#include <fstream>

#if 0
#define dbg(x)
#else
#define dbg(x) std::cout << x
#endif


static inline size_t find_matching_parentheses(const std::string& str, size_t start) {
    size_t open_count = 1;
    for (size_t i = start + 1; i < str.size(); ++i) {
        if (str[i] == '(') {
            open_count += 1;
        } else if (str[i] == ')') {
            open_count -= 1;
        }
        if (open_count == 0) {
            return i;
        }
    }
    return start;
}

ssize_t Calculator::find_highest_precedence_operator_index() {
    for (Operator op : { Operator::Mult, Operator::Div, Operator::Add, Operator::Sub }) {
        try {
            auto iter = std::find_if(m_tokens.begin(), m_tokens.end(), [op](const Token& token) {
                return token.type == Token::Operator && std::get<::Operator>(token.data) == op;
            });
            if (iter != m_tokens.end()) {
                return iter - m_tokens.begin();
            } else {
            }
        } catch (const std::exception& e) {
            std::cerr << __FUNCTION__ << ": " << e.what() << std::endl;
        }
    }
    return -1;
}

void Calculator::print_tokens() const {
    //dbg << "Tokens: { ";
    size_t i = 0;
    for (const auto& tok : m_tokens) {
        if (tok.type == Token::Number) {
            dbg(std::get<BigFloat>(tok.data));
        } else if (tok.type == Token::Operator) {
            switch (std::get<Operator>(tok.data)) {
            case Operator::Add:
                dbg("+");
                break;
            case Operator::Sub:
                dbg("-");
                break;
            case Operator::Mult:
                dbg("*");
                break;
            case Operator::Div:
                dbg("/");
                break;
            case Operator::Mod:
                dbg("%");
                break;
            }
        } else if (tok.type == Token::Parentheses) {
            dbg("(...)");
        } else {
            dbg("???");
        }
        ++i;
        if (i != m_tokens.size()) {
            dbg(" ");
        }
    }
    dbg(std::endl);
    //dbg << " }" << std::endl;
}

void Calculator::replace_tokens(size_t from, size_t to, const Token& tok) {
    if (to > m_tokens.size()) {
        throw std::runtime_error(std::string(__FUNCTION__) + ": to >= size");
    } else if (from > to) {
        throw std::runtime_error(std::string(__FUNCTION__) + ": from > to");
    }
    m_tokens.erase(m_tokens.begin() + from, m_tokens.begin() + to);
    m_tokens.insert(m_tokens.begin() + from, tok);
}

Calculator::Calculator() {
}

template<typename FnT>
static inline size_t find_next(const std::string& str, size_t from, FnT predicate) {
    auto res = std::find_if(str.begin() + from, str.end(), predicate);
    return size_t(str.begin().base() - str.c_str());
}

template<typename ContT, typename ValueT = decltype(ContT::value_type)>
static inline bool contains(const ContT& container, ValueT value) {
    return std::find(container.begin(), container.end(), value) != container.end();
}

static inline bool is_operator(char c) {
    return contains(std::string("+-*/%"), c);
}

static inline bool is_digit_or_number_symbol(char c) {
    return std::isdigit(c) || c == '.';
}

bool Calculator::parse(const std::string& raw_expr) {
    // remove spaces
    std::string expr = raw_expr;
    std::string::size_type iter;

    while ((iter = expr.find(' ')) != std::string::npos) {
        expr.erase(iter, 1);
    }

    for (size_t i = 0; i < expr.size(); ++i) {
        const char& c = expr[i];
        Token tok;
        if (c == '(') {
            size_t start = i;
            size_t end = find_matching_parentheses(expr, i);
            if (end == start) { // means error
                std::cerr << "Parser error: mismatched parentheses" << std::endl;
                return false;
            }
            // create a sub calculator for parentheses expressions
            tok.type = Token::Parentheses;
            tok.data = Calculator();
            bool result = std::get<Calculator>(tok.data).parse(expr.substr(start + 1, end - start - 1));
            if (!result) {
                std::cerr << "Parser error: failed parsing parentheses at " << start << " to " << end << std::endl;
                return false;
            }
            // skip whole parentheses part
            i = end;
        } else if (is_digit_or_number_symbol(c)) {
            // find end of this number
            size_t count;
            for (count = i; count < expr.size(); ++count) {
                if (!is_digit_or_number_symbol(expr[count])) {
                    break;
                }
            }
            // we have to copy out the substring and then use std::stold because std::from_chars is a broken messy piece of shit
            // and I don't have the patience to try and ask someone about this.
            std::string number_substring = expr.substr(i, count);
            BigFloat dbl = std::stold(number_substring);
            tok.type = Token::Number;
            tok.data = dbl;
            i = count - 1;
        } else if (is_operator(c)) {
            tok.type = Token::Operator;
            switch (c) {
            case '+':
                tok.data = Operator::Add;
                break;
            case '-':
                tok.data = Operator::Sub;
                break;
            case '*':
                tok.data = Operator::Mult;
                break;
            case '/':
                tok.data = Operator::Div;
                break;
            case '%':
                tok.data = Operator::Mod;
                break;
            default:
                assert(false);
            }
        } else {
            std::cerr << "Parser error: token not caught by any parser method" << std::endl;
            return false;
        }

        m_tokens.push_back(tok);
    }

    return true;
}

BigFloat Calculator::execute() {
    BigFloat result = 0;
    try {
        for (;;) {
            print_tokens();
            // handle parentheses first
            auto iter = std::find_if(m_tokens.begin(), m_tokens.end(), [&](const Token& tok) {
                return tok.type == Token::Parentheses;
            });

            if (iter != m_tokens.end()) {
                // found parentheses, execute them
                Token tok;
                tok.type = Token::Number;
                tok.data = std::get<Calculator>(iter->data).execute();
                replace_tokens(iter - m_tokens.begin(), iter - m_tokens.begin() + 1, tok);
            }
            if (m_tokens.size() == 1) {
                if (m_tokens.at(0).type == Token::Number) {
                    result = std::get<BigFloat>(m_tokens[0].data);
                    break;
                } else {
                    throw std::runtime_error("invalid input (1)");
                }
            } else {

                // find highest precedence operator
                ssize_t index = find_highest_precedence_operator_index();
                if (index < 0) {
                    // no more operators, but not only one number left -> something's wacky
                    // should never happen
                    throw std::runtime_error("no more operators, but > 1 tokens left (should never happen)");
                }
                auto op = m_tokens.at(index);
                assert(op.type == Token::Operator);
                // should have a number left, and a number or minus operator right
                bool next_is_negative = m_tokens.at(index + 1).type != Token::Number && std::get<::Operator>(m_tokens[index + 1].data) == Operator::Sub;
                if (m_tokens.at(index - 1).type != Token::Number) {
                    throw std::runtime_error("invalid input (2)");
                } else if (m_tokens.at(index + 1).type != Token::Number
                           || (next_is_negative && m_tokens.at(index + 2).type != Token::Number)) {
                    throw std::runtime_error("invalid input (3)");
                }

                size_t start = index - 1;
                size_t end;

                BigFloat left = std::get<BigFloat>(m_tokens.at(index - 1).data);
                BigFloat right;
                if (next_is_negative) {
                    end = index + 2;
                    right = -1.0L * std::get<BigFloat>(m_tokens.at(index + 2).data);
                } else {
                    end = index + 1;
                    right = std::get<BigFloat>(m_tokens.at(index + 1).data);
                }

                Token expr_result;
                expr_result.type = Token::Number;

                switch (std::get<Operator>(op.data)) {
                case Operator::Add:
                    expr_result.data = left + right;
                    break;
                case Operator::Sub:
                    expr_result.data = left - right;
                    break;
                case Operator::Mult:
                    expr_result.data = left * right;
                    break;
                case Operator::Div:
                    expr_result.data = left / right;
                    break;
                case Operator::Mod:
                    expr_result.data = (BigFloat)(int64_t(left) % int64_t(right));
                    break;
                }

                replace_tokens(start, end + 1, expr_result);
            }
        }
    } catch (const std::exception& e) {
        m_tokens.clear();
        throw std::runtime_error(std::string("Error: ") + e.what());
    }
    m_tokens.clear();
    return result;
}
