from configurations import cln


class TestCore:
    def pickle(self, mod):
        import inspect
        import enum
        import builtins

        for name, obj in inspect.getmembers(mod):
            if inspect.isclass(obj):
                assert issubclass(obj, enum.Enum) or issubclass(obj, builtins.Exception) or (
                    hasattr(obj, '__getstate__') and inspect.isroutine(getattr(obj, '__getstate__')))
                assert issubclass(obj, enum.Enum) or issubclass(obj, builtins.Exception) or (
                    hasattr(obj, '__setstate__') and inspect.isroutine(getattr(obj, '__setstate__')))

    def test_init(self):
        import pycarl

    def test_import(self):
        import pycarl
        import pycarl.formula

    def test_config(self):
        import pycarl
        import pycarl._config as config
        assert config.CARL_VERSION_MAJOR >= 17
        assert config.CARL_PARSER == True or config.CARL_PARSER == False
        assert config.CARL_WITH_CLN == True or config.CARL_WITH_CLN == False

    def test_pickle(self):
        import pycarl
        import pycarl.formula
        for mod in [pycarl, pycarl.formula]:
            self.pickle(mod)

    def test_pickle_gmp(self):
        import pycarl
        import pycarl.gmp
        self.pickle(pycarl.gmp)

    @cln
    def test_pickle_cln(self):
        import pycarl
        import pycarl.cln
        self.pickle(pycarl.cln)
