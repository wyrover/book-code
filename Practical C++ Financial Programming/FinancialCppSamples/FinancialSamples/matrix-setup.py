from distutils.core import setup, Extension

setup(name="matrix", version="1.0",
      ext_modules=[Extension("matrix", ["Matrix.cpp", "matrix_Py.cpp"],
                             include_dirs=["/opt/local/include/"],
                             library_dirs=["/opt/local/lib/"],
                             libraries=["boost_python-mt"])])
