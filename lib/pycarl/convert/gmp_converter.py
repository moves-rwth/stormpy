import pycarl

def convert_integer(integer):
    if isinstance(integer, pycarl.cln.Integer):
        return pycarl.gmp.Integer(integer)
    elif isinstance(integer, pycarl.gmp.Integer):
        return integer
    else:
        raise TypeError("Integer of type {} cannot be convert to gmp".format(type(integer)))

def convert_rational(rational):
    if isinstance(rational, pycarl.cln.Rational):
        return pycarl.gmp.Rational(rational)
    elif isinstance(rational, pycarl.gmp.Rational):
        return rational
    else:
        raise TypeError("Rational of type {} cannot be convert to gmp".format(type(rational)))

def convert_term(term):
    if isinstance(term, pycarl.cln.Term):
        coeff = convert_rational(term.coeff)
        return pycarl.gmp.Term(coeff, term.monomial)
    elif isinstance(term, pycarl.gmp.Term):
        return term
    else:
        raise TypeError("Term of type {} cannot be convert to gmp".format(type(term)))

def convert_polynomial(polynomial):
    if isinstance(polynomial, pycarl.cln.Polynomial):
        terms = []
        for term in polynomial:
            terms.append(convert_term(term))
        return pycarl.gmp.Polynomial(terms)
    elif isinstance(polynomial, pycarl.gmp.Polynomial):
        return polynomial
    else:
        raise TypeError("Polynomial of type {} cannot be convert to gmp".format(type(polynomial)))

def convert_rational_function(ratfunc):
    if isinstance(ratfunc, pycarl.cln.RationalFunction):
        numerator = convert_polynomial(ratfunc.numerator)
        denominator = convert_polynomial(ratfunc.denominator)
        return pycarl.gmp.RationalFunction(numerator, denominator)
    elif isinstance(ratfunc, pycarl.gmp.RationalFunction):
        return ratfunc
    else:
        raise TypeError("Rational function of type {} cannot be convert to gmp".format(type(ratfunc)))

def convert_factorized_polynomial(polynomial):
    if isinstance(polynomial, pycarl.cln.FactorizedPolynomial):
        coefficient = convert_rational(polynomial.coefficient())
        converted = pycarl.gmp.FactorizedPolynomial(coefficient)
        for (factor, exponent) in polynomial.factorization():
            pol = convert_polynomial(factor.polynomial())
            factorized = pycarl.gmp.FactorizedPolynomial(pol, pycarl.gmp.factorization_cache)
            converted *= factorized ** exponent
        return converted
    elif isinstance(polynomial, pycarl.gmp.FactorizedPolynomial):
        return polynomial
    else:
        raise TypeError("Factorized polynomial of type {} cannot be convert to gmp".format(type(polynomial)))

def convert_factorized_rational_function(ratfunc):
    if isinstance(ratfunc, pycarl.cln.FactorizedRationalFunction):
        numerator = convert_factorized_polynomial(ratfunc.numerator)
        denominator = convert_factorized_polynomial(ratfunc.denominator)
        return pycarl.gmp.FactorizedRationalFunction(numerator, denominator)
    elif isinstance(ratfunc, pycarl.gmp.FactorizedRationalFunction):
        return ratfunc
    else:
        raise TypeError("Factorized rational function of type {} cannot be convert to gmp".format(type(ratfunc)))
