from configurations import cln


class TestCore:
    def pickle(self, mod):
        import inspect
        import enum
        import builtins

        for name, obj in inspect.getmembers(mod):
            if inspect.isclass(obj):
                assert (
                    issubclass(obj, enum.Enum)
                    or issubclass(obj, builtins.Exception)
                    or (hasattr(obj, "__getstate__") and inspect.isroutine(getattr(obj, "__getstate__")))
                )
                assert (
                    issubclass(obj, enum.Enum)
                    or issubclass(obj, builtins.Exception)
                    or (hasattr(obj, "__setstate__") and inspect.isroutine(getattr(obj, "__setstate__")))
                )

    def test_init(self):
        import pycarl

    def test_import(self):
        import pycarl
        import pycarl.formula

    def test_config(self):
        import pycarl

        assert pycarl.carl_version()
        assert pycarl.has_parser() == True or pycarl.has_parser() == False
        assert pycarl.has_cln() == True or pycarl.has_cln() == False

    def test_version(self):
        import pycarl
        from packaging.version import Version

        version = Version(pycarl.__version__)
        assert version > Version("2.0.0")
        carl_version = Version(pycarl.carl_version())
        assert carl_version > Version("17.07.50") or str(carl_version).startswith("14.")

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
