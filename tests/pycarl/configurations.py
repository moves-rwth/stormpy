import pytest

import pycarl

# Skip not supported functionality
cln = pytest.mark.skipif(not pycarl.has_cln(), reason="No support for CLN")
parser = pytest.mark.skipif(not pycarl.has_parser(), reason="No support for carlparser")

# Parametrize available number types
import pycarl.gmp

parameters = [pycarl.gmp]
names = ["gmp"]

if pycarl.has_cln():
    import pycarl.cln

    parameters.append(pycarl.cln)
    names.append("cln")


class PackageSelector:
    @pytest.fixture(params=parameters, ids=names)
    def package(self, request):
        return request.param
