from ._info import *
from . import _config


def storm_version() -> str:
    """
    Get Storm version.

    :return: Storm version
    :rtype: str
    """
    return _config.STORM_VERSION


def storm_exact_use_cln() -> bool:
    """
    Check if exact arithmetic in Storm uses CLN or GMP.

    :return: True iff exact arithmetic uses CLN.
    :rtype: bool
    """
    return _config.STORM_CLN_EA


def storm_ratfunc_use_cln() -> bool:
    """
    Check if rational functions in Storm use CLN or GMP.

    :return: True iff rational functions use CLN.
    :rtype: bool
    """
    return _config.STORM_CLN_RF
