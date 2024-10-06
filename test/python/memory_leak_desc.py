from cpp2py.wrap_generator import *

# The module
module = module_(full_name = "memory_leak", doc = r"", app_name = "memory_leak")

# Imports


module.add_include("memory_leak.hpp")

module.add_preamble("""
#include <nda_py/cpp2py_converters.hpp>

""")

module.add_function ("void memory_leak(size_t size)", doc = r"""""")

module.generate_code()
