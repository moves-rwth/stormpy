import pycarl


def convert_integer(integer):
    """
    Convert integer to gmp.
    :param integer: integer.
    :return: gmp interger.
    """
    if isinstance(integer, pycarl.cln.Integer):
        return pycarl.gmp.Integer(integer)
    elif isinstance(integer, pycarl.gmp.Integer):
        return integer
    else:
        raise TypeError("Integer of type {} cannot be convert to gmp".format(type(integer)))


def convert_rational(rational):
    """
    Convert rational number to gmp.
    :param rational: rational number.
    :return: gmp rational.
    """
    if isinstance(rational, pycarl.cln.Rational):
        return pycarl.gmp.Rational(rational)
    elif isinstance(rational, pycarl.gmp.Rational):
        return rational
    else:
        raise TypeError("Rational of type {} cannot be convert to gmp".format(type(rational)))


def convert_term(term):
    """
    Convert term to gmp.
    :param term: term.
    :return: gmp term.
    """
    if isinstance(term, pycarl.cln.Term):
        coeff = convert_rational(term.coeff)
        return pycarl.gmp.Term(coeff, term.monomial)
    elif isinstance(term, pycarl.gmp.Term):
        return term
    else:
        raise TypeError("Term of type {} cannot be convert to gmp".format(type(term)))


def convert_polynomial(polynomial):
    """
    Convert polynomial to gmp.
    :param polynomial: polynomial.
    :return: gmp polynomial.
    """
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
    """
    Convert rational function to gmp.
    :param ratfunc: rational function.
    :return: gmp rational function.
    """
    if isinstance(ratfunc, pycarl.cln.RationalFunction):
        numerator = convert_polynomial(ratfunc.numerator)
        denominator = convert_polynomial(ratfunc.denominator)
        return pycarl.gmp.RationalFunction(numerator, denominator)
    elif isinstance(ratfunc, pycarl.gmp.RationalFunction):
        return ratfunc
    else:
        raise TypeError("Rational function of type {} cannot be convert to gmp".format(type(ratfunc)))


def convert_factorized_polynomial(polynomial):
    """
    Convert factorized polynomial to gmp.
    :param polynomial: factorized polynomial.
    :return: gmp factorized polynomial.
    """
    if isinstance(polynomial, pycarl.cln.FactorizedPolynomial):
        coefficient = convert_rational(polynomial.coefficient)
        converted = pycarl.gmp.FactorizedPolynomial(coefficient)
        for (factor, exponent) in polynomial.factorization():
            pol = convert_polynomial(factor.polynomial())
            factorized = pycarl.gmp.create_factorized_polynomial(pol)
            converted *= factorized ** exponent
        return converted
    elif isinstance(polynomial, pycarl.gmp.FactorizedPolynomial):
        return polynomial
    else:
        raise TypeError("Factorized polynomial of type {} cannot be convert to gmp".format(type(polynomial)))


def convert_factorized_rational_function(ratfunc):
    """
    Convert factorized rational function to gmp.
    :param ratfunc: factorized rational function.
    :return: gmp factorized rational function.
    """
    if isinstance(ratfunc, pycarl.cln.FactorizedRationalFunction):
        numerator = convert_factorized_polynomial(ratfunc.numerator)
        denominator = convert_factorized_polynomial(ratfunc.denominator)
        return pycarl.gmp.FactorizedRationalFunction(numerator, denominator)
    elif isinstance(ratfunc, pycarl.gmp.FactorizedRationalFunction):
        return ratfunc
    else:
        raise TypeError("Factorized rational function of type {} cannot be convert to gmp".format(type(ratfunc)))


def convert_constraint(constraint):
    """
    Convert constraint to gmp.
    :param constraint: constraint.
    :return: gmp constraint.
    """
    if isinstance(constraint, pycarl.cln.formula.Constraint):
        lhs = convert_polynomial(constraint.lhs)
        return pycarl.gmp.formula.Constraint(lhs, constraint.relation)
    elif isinstance(constraint, pycarl.gmp.formula.Constraint):
        return constraint
    else:
        raise TypeError("Constraint of type {} cannot be convert to gmp".format(type(constraint)))


def convert(data):
    """
    Convert arbitrary data type to gmp.
    :param data: data structure.
    :return: gmp data structure.
    """
    if isinstance(data, pycarl.cln.Integer) or isinstance(data, pycarl.gmp.Integer):
        return convert_integer(data)
    elif isinstance(data, pycarl.cln.Rational) or isinstance(data, pycarl.gmp.Rational):
        return convert_rational(data)
    elif isinstance(data, pycarl.cln.Term) or isinstance(data, pycarl.gmp.Term):
        return convert_term(data)
    elif isinstance(data, pycarl.cln.Polynomial) or isinstance(data, pycarl.gmp.Polynomial):
        return convert_polynomial(data)
    elif isinstance(data, pycarl.cln.RationalFunction) or isinstance(data, pycarl.gmp.RationalFunction):
        return convert_rational_function(data)
    elif isinstance(data, pycarl.cln.FactorizedPolynomial) or isinstance(data, pycarl.gmp.FactorizedPolynomial):
        return convert_factorized_polynomial(data)
    elif isinstance(data, pycarl.cln.FactorizedRationalFunction) or isinstance(data,
                                                                               pycarl.gmp.FactorizedRationalFunction):
        return convert_factorized_rational_function(data)
    elif isinstance(data, pycarl.cln.formula.Constraint) or isinstance(data, pycarl.gmp.formula.Constraint):
        return convert_constraint(data)
    else:
        raise TypeError("Unknown type {} for conversion to gmp".format(type(data)))
