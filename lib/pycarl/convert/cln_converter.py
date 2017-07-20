import pycarl

def convert_integer(integer):
    if isinstance(integer, pycarl.cln.Integer):
        return integer
    elif isinstance(integer, pycarl.gmp.Integer):
        return pycarl.cln.Integer(integer)
    else:
        raise TypeError("Integer of type {} cannot be convert to cln".format(type(integer)))

def convert_rational(rational):
    if isinstance(rational, pycarl.cln.Rational):
        return rational
    elif isinstance(rational, pycarl.gmp.Rational):
        return pycarl.cln.Rational(rational)
    else:
        raise TypeError("Rational of type {} cannot be convert to cln".format(type(rational)))

def convert_term(term):
    if isinstance(term, pycarl.cln.Term):
        return term
    elif isinstance(term, pycarl.gmp.Term):
        coeff = convert_rational(term.coeff)
        return pycarl.cln.Term(coeff, term.monomial)
    else:
        raise TypeError("Term of type {} cannot be convert to cln".format(type(term)))

def convert_polynomial(polynomial):
    if isinstance(polynomial, pycarl.cln.Polynomial):
        return polynomial
    elif isinstance(polynomial, pycarl.gmp.Polynomial):
        terms = []
        for term in polynomial:
            terms.append(convert_term(term))
        return pycarl.cln.Polynomial(terms)
    else:
        raise TypeError("Polynomial of type {} cannot be convert to cln".format(type(polynomial)))

def convert_rational_function(ratfunc):
    if isinstance(ratfunc, pycarl.cln.RationalFunction):
        return ratfunc
    elif isinstance(ratfunc, pycarl.gmp.RationalFunction):
        numerator = convert_polynomial(ratfunc.numerator)
        denominator = convert_polynomial(ratfunc.denominator)
        return pycarl.cln.RationalFunction(numerator, denominator)
    else:
        raise TypeError("Rational function of type {} cannot be convert to cln".format(type(ratfunc)))
