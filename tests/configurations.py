import pytest

import stormpy._config as config

# Skip not supported functionality
has_xml = config.storm_with_xerces
has_dft = config.storm_with_dft
has_gspn = config.storm_with_gspn
has_pars = config.storm_with_pars

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

xml = pytest.mark.skipif(not has_xml, reason="No support for XML via xerces")
dft = pytest.mark.skipif(not has_dft, reason="No support for DFTs")
gspn = pytest.mark.skipif(not has_gspn, reason="No support for GSPNs")
pars = pytest.mark.skipif(not has_pars, reason="No support for parametric model checking")
numpy_avail = pytest.mark.skipif(not has_numpy, reason="Numpy not available")
plotting = pytest.mark.skipif(not has_matplotlib, reason="Matplotlib not available")