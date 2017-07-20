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
