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
prism_ma_simple = _path("ma", "simple.ma")
"""Prism file for a simple Markov automaton"""
drn_ctmc_dft = _path("ctmc", "dft.drn")
"""DRN format for a CTMC from a DFT"""
drn_pdtmc_die = _path("pdtmc", "die.drn")
"""DRN format for a pDTMC for the KY-Die"""
jani_dtmc_die = _path("dtmc", "die.jani")
"""Jani Version of Knuth Yao Die Example"""
prism_pdtmc_brp = _path("pdtmc", "brp16_2.pm")
"""Bounded retransmission protocol with parameters"""
prism_mdp_coin_2_2 = _path("mdp", "coin2-2.nm")
"""Prism example for coin MDP"""
prism_pmdp_coin_two_dice = _path("pmdp", "two_dice.nm")
"""Prism example for parametric two dice"""
prism_mdp_maze = _path("mdp", "maze_2.nm")
"""Prism example for the maze MDP"""
prism_pomdp_maze = _path("pomdp", "maze_2.prism")
"""Prism example for the maze POMDP"""
dft_galileo_hecs = _path("dft", "hecs.dft")
"""DFT example for HECS (Galileo format)"""
dft_json_and = _path("dft", "and.json")
"""DFT example for AND gate (JSON format)"""
gspn_pnpro_simple = _path("gspn", "gspn_simple.pnpro")
"""GSPN example (PNPRO format)"""
gspn_pnml_simple = _path("gspn", "gspn_simple.pnml")
"""GSPN example (PNML format)"""
