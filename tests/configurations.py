import pytest

import pycarl._config as config

# Skip not supported functionality
has_cln = config.CARL_WITH_CLN
has_parser = config.CARL_PARSER
cln = pytest.mark.skipif(not has_cln, reason="No support for CLN")
parser = pytest.mark.skipif(not has_parser, reason="No support for carlparser")

# Parametrize available number types
import pycarl.gmp

parameters = [pycarl.gmp]
names = ["gmp"]

if has_cln:
    import pycarl.cln

    parameters.append(pycarl.cln)
    names.append("cln")


class PackageSelector:
    @pytest.fixture(params=parameters, ids=names)
    def package(self, request):
        return request.param
