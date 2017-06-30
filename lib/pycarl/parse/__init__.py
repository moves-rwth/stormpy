from . import _config

if not _config.CARL_PARSER:
    raise ImportError("Parser is not available!")

from . import parse
from .parse import *

def deserialize(input, package):
    """

    :param input:
    :return:
    """
    res = package.parse.parse._deserialize(input)
    res_type = res.get_type()
    if res_type == parse._ParserReturnType.Rational:
        return res.as_rational()
    elif res_type == parse._ParserReturnType.Variable:
        return res.as_variable()
    elif res_type == parse._ParserReturnType.Monomial:
        return res.as_monomial()
    elif res_type == parse._ParserReturnType.Term:
        return res.as_term()
    elif res_type == parse._ParserReturnType.Polynomial:
        return res.as_polynomial()
    elif res_type == parse._ParserReturnType.RationalFunction:
        return res.as_rationalfunction()
    elif res_type == parse._ParserReturnType.Constraint:
        return res.as_constraint()
    elif res_type == parse._ParserReturnType.Formula:
        return res.as_formula()
    assert False, "Internal error."