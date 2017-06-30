#include "parser.h"

carlparser::parser_types<Polynomial> from_string(const std::string& input) {
    return carlparser::deserialize<Polynomial>(input);
}