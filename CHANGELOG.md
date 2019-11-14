Changelog
=============


Version 2.0.x
-------------

### Version 2.0.5 (under development)


### Version 2.0.4 (2019/11)
- Extended conversion of number types to formulae
- (minor) changed `constant_part` to function for `Polynomial`
- (minor) added an additional operator on polynomials
- Fix for compilation with Xcode 11

### Version 2.0.3 (2019/01)
Requires carl with branch `master14` or a carl version between 17.12 and 18.08

- Extended bindings for arithmetic operations
- Fixed negation comparison
- Improved error output in installation
- Extended documentation
- Improved and extended setup

### Version 2.0.2 (2017/12)
Requires carl version >= 17.12

- Adaptions to changes in carl
- Extended bindings for rational functions and factorized rational functions
- `Variable` constructor no longer returns an existing variable if a variable with that name exists
- `expand()` allows to obtain the expanded polynomial and rational function
- Added methods for getting information about pycarl configuration
- Extended build script:
    * uses config file
    * support for disabling build of CLN and parser bindings
    * fixed debug build type

### Version 2.0.1 (2017/08)
Requires carl version >= 17.08

- Added conversions between CLN and GMP
- Added variable and integer pickling support. Throw errors if pickling is not supported
- Added hash functions
- Hide factorization caches for user
- Depend on [carl-parser](https://github.com/smtrat/carl-parser) for parsing routines
- Check for carl version
- Fixed issues when CLN is not available

### Version 2.0.0 (2017/05)
- First version with support for CLN and GMP.
  This means that no longer all operators are supported as it would be ambiguous
- Using bigint


Version 1.2.x
-------------

### Version 1.2.1 (2017/04)
- Arbitrary-size integers (CLN and GMP)

### Version 1.2.0 (2017/03)
- Start of this changelog
