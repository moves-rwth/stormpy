import os
import stormpy
import stormpy.examples
import stormpy.examples.files

example_dir = stormpy.examples.files.testfile_dir


def get_example_path(*paths):
    return os.path.join(example_dir, *paths)
