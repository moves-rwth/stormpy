import pycarl
import pycarl.cln.parse, pycarl.gmp.parse
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from package_selector import PackageSelector


class TestParse(PackageSelector):
    def test_parse_number(self, package):
        num = package.parse.parse("2")
        assert str(num) == "2"
