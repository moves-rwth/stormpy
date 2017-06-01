
class TestCore:
    def test_init(self):
        import pycarl

    def test_import(self):
        import pycarl
        import pycarl.formula

    def test_pickle(self):
        import pycarl
        import pycarl.gmp
        import pycarl.cln
        import pycarl.formula

        import inspect
        import enum
        import builtins

        for mod in [pycarl, pycarl.formula, pycarl.gmp, pycarl.cln]:
            for name, obj in inspect.getmembers(mod):
                if inspect.isclass(obj):
                    assert  issubclass(obj, enum.Enum) or issubclass(obj, builtins.Exception) or (hasattr(obj, '__getstate__') and inspect.isroutine(getattr(obj, '__getstate__')))
                    assert  issubclass(obj, enum.Enum) or issubclass(obj, builtins.Exception) or (hasattr(obj, '__setstate__') and inspect.isroutine(getattr(obj, '__setstate__')))

