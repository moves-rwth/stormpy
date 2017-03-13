import stormpy
from stormpy.info import info


class TestInfo:
    def test_version(self):
        assert isinstance(info.Version.short, str) and "." in info.Version.short
        assert isinstance(info.Version.long, str) and "Version" in info.Version.long
        assert isinstance(info.Version.build_info, str) and "Compiled" in info.Version.build_info
