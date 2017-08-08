from . import _config

if not _config.storm_with_dft:
    raise ImportError("No support for DFTs was built in Storm.")

from . import dft
from .dft import *

dft._set_up()
