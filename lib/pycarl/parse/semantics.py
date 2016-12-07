from pycarl import Rational, Variable, VariableType, Polynomial
from pycarl.formula import Formula, FormulaType, Constraint, Relation

class ExpressionSemantics(object):
    def number(self, ast):
        numstr = str(ast)
        return Rational(numstr)

    def _make_poly(self, poly):
        if isinstance(poly, Polynomial):
            return poly
        return Polynomial(poly)

    def _make_formula(self, forms):
        newforms = []
        for form in forms:
            if isinstance(form, Formula):
                newforms.append(form)
            else:
                newforms.append(Formula(form))
        return newforms

    def identifier(self, ast):
        #TODO: encode variable type (maybe 'XXX:TYPE' ?)
        return Variable(str(ast), VariableType.REAL)

    def bool(self, ast):
        if ast == 'true':
            return Formula(True)
        elif ast == 'false':
            return Formula(False)
        else:
            assert False

    def parse_form(self, ast, form_type, op):
        form = ast.lhs
        if not ast.rhs:
            return form
        for op, rhs in ast.rhs:
            assert op == op
        rhs_forms = [rhs for _, rhs in ast.rhs]
        return Formula(form_type, self._make_formula([form] + rhs_forms))

    def iff_form(self, ast):
        return self.parse_form(ast, FormulaType.IFF, '<->')

    def imp_form(self, ast):
        return self.parse_form(ast, FormulaType.IMPLIES, '->')

    def xor_form(self, ast):
        return self.parse_form(ast, FormulaType.XOR, '^')

    def or_form(self, ast):
        return self.parse_form(ast, FormulaType.OR, '|')

    def and_form(self, ast):
        return self.parse_form(ast, FormulaType.AND, '&')

    def eq_expr(self, ast):
        expr = ast.lhs
        if ast.rhs:
            op, rhs = ast.rhs
            if op in ['==', '=']:
                expr = Constraint(self._make_poly(expr - rhs), Relation.EQ)
            elif op == '!=':
                expr = Constraint(self._make_poly(expr - rhs), Relation.NEQ)
            else:
                assert False, "Unknown equality op {}".format(op)
        return expr

    def rel_expr(self, ast):
        expr = ast.lhs
        if ast.rhs:
            # Subtraction ensures polynomial is passed to Constraint constructor
            op, rhs = ast.rhs
            if op == '<':
                expr = Constraint(self._make_poly(expr - rhs), Relation.LESS)
            elif op == '<=':
                expr = Constraint(self._make_poly(expr - rhs), Relation.LEQ)
            elif op == '>':
                expr = Constraint(self._make_poly(expr - rhs), Relation.GREATER)
            elif op == '>=':
                expr = Constraint(self._make_poly(expr - rhs), Relation.GEQ)
            else:
                assert False, "Unknown relational op {}".format(op)
        return expr

    def add_expr(self, ast):
        expr = ast.lhs
        for op, rhs in ast.rhs:
            if op == '+':
                expr = expr + rhs
            elif op == '-':
                expr = expr - rhs
            else:
                assert False, "Unknown add op {}".format(op)
        return expr

    def scale_expr(self, ast):
        expr = ast.lhs
        for op, rhs in ast.rhs:
            if op == '*':
                expr = expr * rhs
            elif op == '/':
                expr = expr / rhs
            else:
                assert False, "Unknown scale op {}".format(op)
        return expr

    def pow_expr(self, ast):
        expr = ast.lhs
        if not ast.rhs:
            return expr
        assert ast.op == '**', "Unknown pow op {}".format(ast.op)
        return expr ** int(ast.rhs)

    def unary_expr(self, ast):
        if ast.op is None:
            return ast.rhs
        elif ast.op == '-':
            return -ast.rhs
        elif ast.op == '+':
            return +ast.rhs
        elif ast.op == '~':
            return not ast.rhs
        else:
            assert False, "Unknown unary op {}".format(op)
