from . import cln_converter
from . import gmp_converter


def convert_to_cln(data):
    """
    Convert data type to cln.
    :param data: data structure.
    :return: cln data structure.
    """
    return cln_converter.convert(data)


def convert_to_gmp(data):
    """
    Convert data type to gmp.
    :param data: data structure.
    :return: gmp data structure.
    """
    return gmp_converter.convert(data)
