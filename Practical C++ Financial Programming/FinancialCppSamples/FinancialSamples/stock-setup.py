# to build this:
#  python stock-setup.py build_ext -i
from distutils.core import setup, Extension

setup(name="stock", version="1.0",
      ext_modules=[Extension("stock", ["Stock.cpp", "Stock_Py.cpp"])])
