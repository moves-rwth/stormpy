from . import parse
from .parse import *

from .semantics import ExpressionSemantics
from .grammar import parserParser

def parseExpr(text):
    parser = parserParser(parseinfo=False)
    ast = parser.parse(text, rule_name='expression', semantics=ExpressionSemantics())
    return ast
