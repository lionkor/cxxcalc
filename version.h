#define VERSION "v1.1"

#define HELP_TEXT                                                                              \
    "cxxcalc " VERSION                                                                         \
    "\n"                                                                                       \
    "Type in an EXPRESSION and press enter/return to calculate.\n"                             \
    "An EXPRESSION consists of one or more OPERATORS and their OPERANDS.\n"                    \
    "The following OPERATORS are supported:\n"                                                 \
    "   + : Addition\n"                                                                        \
    "   - : Subtraction\n"                                                                     \
    "   * : Multiplication\n"                                                                  \
    "   / : Division\n"                                                                        \
    "OPERANDS can be numbers or other EXPRESSIONS, such as EXPRESSIONS within PARENTHESES.\n"  \
    "OPERATORS follow a strict precedence, as listed here, in order: * / + -\n"                \
    "PARENTHESES EXPRESSIONS, such as `(1 + 2)` take higher precedence than any OPERATORS\n"   \
    "outside of the PARENTHESES.\n"                                                            \
    "SIGNS take highest precedence. An example of a SIGN infront of a number is `-5`.\n"       \
    "Both negative `-` and positive `+` SIGNS are supported. Multiple successive SIGNS are \n" \
    "supported and get simplified.\n\n"                                                        \
    "The following CONSTANTS and VARIABLES are supported:\n"                                   \
    "   pi  : Constant Pi\n"                                                                    \
    "   e   : Constant e\n"                                                                     \
    "   ans : Last result\n"

#define GOODBYE_TEXT \
    "Goodbye!"

#define WELCOME_TEXT                               \
    "cxxcalc " VERSION                             \
    "\n"                                           \
    "Press ^C (ctrl+c) or type \"exit\" to exit\n" \
    "Type \"help\" to view help\n\n"
