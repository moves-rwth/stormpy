Changelog
=========



Version 2.0
-----------

### Version 2.0.2 (requires CArL > 17.10)
- Extended bindings for rational functions and factorized rational functions
- Variable constructor no longer returns an existing variable if a variable with that name exists
- expand allows to obtain the expanded polynomial and rational function
- Extended build script:
    * uses config file
    * support for disabling build of cln and parser bindings
    * fixed debug build type

### Version 2.0.1 (requires CArL >= 17.08)
- Added conversions between CLN and GMP
- Added variable and integer pickling support. Throw errors if pickling is not supported
- Added hash functions
- Hide factorization caches for user
- Depend on CArL-parser for parsing routines
- Check for CArL version
- Fixed issues when CLN is not available


### Version 2.0.0
- First version with support for cln and gmp.
  This means that no longer all operators are supported as it would be ambiguous
- Using bigint



Version 1.2
-----------

### Version 1.2.1 (2017/4)

- Arbitrary-size integers (CLN & GMP)


### Version 1.2.0 (2017/3)
- Start of this changelog
