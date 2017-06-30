#pragma once

#include "src/common.h"
#include "src/types.h"
#include <string>
#include <carl/core/MultivariatePolynomial.h>
#include <carl-parser/Parser.h>

carlparser::parser_types<Polynomial> from_string(const std::string& input);
