import os

testfile_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "files")

def _path(folder, file):
    """Internal method for simpler listing of examples"""
    return os.path.join(testfile_dir, folder, file)


prism_dtmc_die = _path("dtmc", "die.pm")
"""Knuth Yao Die Example"""
prism_dtmc_brp = _path("dtmc", "brp-16-2.pm")
"""Bounded Retransmission Protocol"""