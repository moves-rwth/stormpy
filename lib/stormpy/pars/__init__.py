from . import _config

if not _config.has_storm_pars:
    raise ImportError("No support for parametric analysis was built in Storm.")

from . import pars
from .pars import *

pars._set_up()
