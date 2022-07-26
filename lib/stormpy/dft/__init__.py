from . import _config

if not _config.storm_with_dft:
    raise ImportError("No support for DFTs was built in Storm.")

from . import dft
from .dft import *

dft._set_up()


def analyze_dft(ft, properties, symred=True, allow_modularisation=False, relevant_events=RelevantEvents(), allow_dc_for_relevant=False):
    if isinstance(ft, DFT_double):
        return dft._analyze_dft_double(ft, properties, symred, allow_modularisation, relevant_events, allow_dc_for_relevant)
    else:
        assert isinstance(ft, DFT_ratfunc)
        return dft._analyze_dft_ratfunc(ft, properties, symred, allow_modularisation, relevant_events, allow_dc_for_relevant)


def build_model(ft, symmetries, relevant_events=RelevantEvents(), allow_dc_for_relevant=False):
    if isinstance(ft, DFT_double):
        return dft._build_model_double(ft, symmetries, relevant_events, allow_dc_for_relevant)
    else:
        assert isinstance(ft, DFT_ratfunc)
        return dft._build_model_ratfunc(ft, symmetries, relevant_events, allow_dc_for_relevant)


def transform_dft(ft, unique_constant_be, binary_fdeps, exponential_distributions):
    if isinstance(ft, DFT_double):
        return dft._transform_dft_double(ft, unique_constant_be, binary_fdeps, exponential_distributions)
    else:
        assert isinstance(ft, DFT_ratfunc)
        return dft._transform_dft_ratfunc(ft, unique_constant_be, binary_fdeps, exponential_distributions)


def compute_dependency_conflicts(ft, use_smt=False, solver_timeout=0):
    if isinstance(ft, DFT_double):
        return dft._compute_dependency_conflicts_double(ft, use_smt, solver_timeout)
    else:
        assert isinstance(ft, DFT_ratfunc)
        return dft._compute_dependency_conflicts_ratfunc(ft, use_smt, solver_timeout)


def prepare_for_analysis(ft):
    compute_dependency_conflicts(ft, use_smt=False)
    return transform_dft(ft, unique_constant_be=True, binary_fdeps=True, exponential_distributions=True)


def is_well_formed(ft, check_valid_for_analysis=True):
    if isinstance(ft, DFT_double):
        return dft._is_well_formed_double(ft, check_valid_for_analysis)
    else:
        assert isinstance(ft, DFT_ratfunc)
        return dft._is_well_formed_ratfunc(ft, check_valid_for_analysis)


def compute_relevant_events(ft, properties, additional_relevant_names=[]):
    if isinstance(ft, DFT_double):
        return dft._compute_relevant_events_double(ft, properties, additional_relevant_names)
    else:
        assert isinstance(ft, DFT_ratfunc)
        return dft._compute_relevant_events_ratfunc(ft, properties, additional_relevant_names)
