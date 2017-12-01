import os
import re
import sys


def ensure_dir_exists(path):
    """
    Check whether the directory exists and creates it if not.
    """
    assert path is not None
    try:
        os.makedirs(path)
    except FileExistsError:
        pass
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise IOError("Cannot create directory: " + path)
    except BaseException:
        raise IOError("Path " + path + " seems not valid")


def check_carl_compatible(carl_dir, carl_major, carl_minor, carl_patch):
    """
    Check if the given carl lib is compatible with this pycarl version.
    :param carl_dir: Path of carl.
    :param carl_major: Major version of carl.
    :param carl_minor: Minor version of carl.
    :param carl_patch: Patch version of carl.
    """
    if carl_major < 17 or (carl_major == 17 and carl_minor < 8) or (
                        carl_major == 17 and carl_minor == 8 and carl_patch < 0):
        sys.exit(
            'Sorry, carl version {}.{}.{} from \'{}\' is not supported anymore!'.format(carl_major, carl_minor,
                                                                                        carl_patch, carl_dir))


def parse_carl_version(version_string):
    """
    Parses the version of carl.

    :param version_string: String containing version information.
    :return: Triple (major, minor, patch)
    """
    elems = version_string.split(".")
    if len(elems) == 2:
        elems.append("0")
    if len(elems) != 3:
        sys.exit('Carl version string is ill-formed: "{}"'.format(version_string))
    patch = elems[2].split('-')[0]
    return int(elems[0]), int(elems[1]), int(patch)


def obtain_version():
    """
    Obtains the version as specified in pycarl.

    :return: Version
    """
    verstr = "unknown"
    try:
        verstrline = open('lib/pycarl/_version.py', "rt").read()
    except EnvironmentError:
        pass  # Okay, there is no version file.
    else:
        VSRE = r"^__version__ = ['\"]([^'\"]*)['\"]"
        mo = re.search(VSRE, verstrline, re.M)
        if mo:
            verstr = mo.group(1)
        else:
            raise RuntimeError("unable to find version in pycarl/_version.py")
    return verstr
