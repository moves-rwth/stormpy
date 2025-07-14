from stormpy import _config

if not _config.storm_with_gspn:
    raise ImportError("No support for GSPNs was built in Storm.")

from . import _gspn
from ._gspn import *
