import pytest

import stormpy._config as config

# Skip not supported functionality
has_dft = config.storm_with_dft
has_gspn = config.storm_with_gspn
has_pars = config.storm_with_pars

# Check if numpy is available
try:
    import numpy

    has_numpy = True
except ImportError:
    has_numpy = False

dft = pytest.mark.skipif(not has_dft, reason="No support for DFTs")
gspn = pytest.mark.skipif(not has_gspn, reason="No support for GSPNs")
pars = pytest.mark.skipif(not has_pars, reason="No support for parametric model checking")
numpy_avail = pytest.mark.skipif(not has_numpy, reason="Numpy not available")
