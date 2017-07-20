import pycarl

def convert_rational(number):
    if isinstance(number, pycarl.cln.Rational):
        return number
    elif isinstance(number, pycarl.gmp.Rational):
        return pycarl.cln.Rational(number)
    else:
        raise TypeError("Rational of type {} cannot be convert to cln".format(type(number)))
