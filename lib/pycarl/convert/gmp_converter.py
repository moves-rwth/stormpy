import pycarl

def convert_rational(number):
    if isinstance(number, pycarl.cln.Rational):
        return pycarl.gmp.Rational(number)
    elif isinstance(number, pycarl.gmp.Rational):
        return number
    else:
        raise TypeError("Rational of type {} cannot be convert to gmp".format(type(number)))
