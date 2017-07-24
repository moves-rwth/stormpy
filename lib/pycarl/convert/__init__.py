from . import cln_converter
from . import gmp_converter

def convert_to_cln(data):
    return cln_converter.convert(data)

def convert_to_gmp(data):
    return gmp_converter.convert(data)
