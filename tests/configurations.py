import pytest

import stormpy._config as config

# Skip not supported functionality
has_xml = config.storm_with_xerces
has_dft = config.storm_with_dft
has_gspn = config.storm_with_gspn
has_pars = config.storm_with_pars
has_spot = config.storm_with_spot
has_pomdp = config.storm_with_pomdp

try:
    import numpy

    has_numpy = True
except ImportError:
    has_numpy = False

try:
    import matplotlib

    has_matplotlib = True
except ImportError:
    has_matplotlib = False

try:
    import scipy

    has_scipy = True
except ImportError:
    has_scipy = False

xml = pytest.mark.skipif(not has_xml, reason="No support for XML via xerces")
dft = pytest.mark.skipif(not has_dft, reason="No support for DFTs")
gspn = pytest.mark.skipif(not has_gspn, reason="No support for GSPNs")
pars = pytest.mark.skipif(not has_pars, reason="No support for parametric model checking")
pomdp = pytest.mark.skipif(not has_pomdp, reason="No support for POMDPs")
spot = pytest.mark.skipif(not has_spot, reason="No support for LTL via spot")
numpy_avail = pytest.mark.skipif(not has_numpy, reason="Numpy not available")
plotting = pytest.mark.skipif(not has_matplotlib or not has_scipy, reason="Libraries for plotting not available")
