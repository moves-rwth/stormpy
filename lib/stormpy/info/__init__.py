from . import info
from .info import *

from . import _config


def storm_version():
    """
    Get storm version.
    :return: Storm version
    """
    return _config.storm_version


def storm_exact_use_cln():
    """
    Check if exact arithmetic in Storm uses CLN.
    :return: True if exact arithmetic uses CLN.
    """
    return _config.storm_cln_ea


def storm_ratfunc_use_cln():
    """
    Check if rational functions in Storm use CLN.
    :return: True iff rational functions use CLN.
    """
    return _config.storm_cln_rf
