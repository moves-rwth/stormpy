from . import utility
from .utility import *

# Extend JSON container for simplified access
utility.JsonContainerDouble.__eq__ = lambda s, o: str(s) == str(o)
utility.JsonContainerDouble.__int__ = lambda s: int(str(s))
utility.JsonContainerDouble.__hash__ = lambda s: str(s).__hash__()


utility.JsonContainerRational.__eq__ = lambda s, o: str(s) == str(o)
utility.JsonContainerRational.__int__ = lambda s: int(str(s))
utility.JsonContainerRational.__hash__ = lambda s: str(s).__hash__()
