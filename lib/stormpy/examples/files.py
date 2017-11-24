import os

testfile_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "files")


def _path(folder, file):
    """
    Internal method for simpler listing of examples.
    :param folder: Folder.
    :param file: Example file.
    :return: Complete path to example file.
    """
    return os.path.join(testfile_dir, folder, file)


prism_dtmc_die = _path("dtmc", "die.pm")
"""Knuth Yao Die Example"""
prism_pdtmc_die = _path("pdtmc", "parametric_die.pm")
"""Knuth Yao Die -- 2 unfair coins Example"""
prism_dtmc_brp = _path("dtmc", "brp-16-2.pm")
"""Bounded Retransmission Protocol"""
drn_ctmc_dft = _path("ctmc", "dft.drn")
"""DRN format for a CTMC from a DFT"""
drn_pdtmc_die = _path("pdtmc", "die.drn")
"""DRN format for a pDTMC for the KY-Die"""
jani_dtmc_die = _path("dtmc", "die.jani")
"""Jani Version of Knuth Yao Die Example"""