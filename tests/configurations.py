import pytest

import stormpy._config as config

# Skip not supported functionality
has_dft = config.storm_with_dft
has_pars = config.storm_with_pars

dft = pytest.mark.skipif(not has_dft, reason="No support for DFTs")
pars = pytest.mark.skipif(not has_pars, reason="No support for parametric model checking")
