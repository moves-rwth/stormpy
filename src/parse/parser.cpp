#include "parser.h"

#include "core/types.h"

//#include <carl/util/parser/Parser.h>
#include <carl/util/parser/ExpressionParser.h>

#include <boost/variant/get.hpp>
#include <boost/variant/static_visitor.hpp>

class to_object: public boost::static_visitor<py::object> {
public:
    template<typename T>
    py::object operator()(T expr) const {
        // Expr is on the stack, make a copy
        py::object res = py::cast(expr, py::return_value_policy::copy);
        if (!res) {
            // return void
            res = py::object(Py_None, true);
        }
        return res;
    }
};

carl::parser::ExpressionType<Polynomial> parse(const std::string& input) {
    typedef std::string::const_iterator It;
    It f(begin(input)), l(end(input));
    try {
        carl::parser::ExpressionParser<Polynomial> parser;
        carl::parser::Skipper skipper;
        carl::parser::ExpressionType<Polynomial> data;

        bool parse_result = qi::phrase_parse(f, l, parser, skipper, data);
        //bool parse_result = qi::parse(f, l, parser, data);

        if (!parse_result) {
            CARL_LOG_ERROR("carl.parser", "Parsing \"" << input << "\" failed.");
        }
        if (f != l) {
            CARL_LOG_ERROR("carl.parser", "Parsing \"" << input << "\" leaves trailing " << std::string(f,l));
        }
        return data;
    } catch(const qi::expectation_failure<It>& e) {
        std::string frag(e.first, e.last);
        throw std::runtime_error(std::string(e.what()) + ": " + frag);
    }
}

py::handle parseString(const std::string& input) {
    carl::parser::ExpressionType<Polynomial> data = parse(input);

    py::object obj = boost::apply_visitor( to_object(), data );
    auto handl = obj.release();
    return handl;
}
