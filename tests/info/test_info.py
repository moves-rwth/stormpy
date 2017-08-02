import stormpy
import stormpy.info


class TestInfo:
    def test_version(self):
        assert isinstance(stormpy.info.Version.short, str) and "." in stormpy.info.Version.short
        assert isinstance(stormpy.info.Version.long, str) and "Version" in stormpy.info.Version.long
        assert isinstance(stormpy.info.Version.build_info, str) and "Compiled" in stormpy.info.Version.build_info
