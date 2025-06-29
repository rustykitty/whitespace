from setuptools import setup, Extension

import os.path

python_dir = os.path.dirname(__file__)

setup(
    name='whitespace_module',
    py_modules=['whitespace_module'],
    ext_modules=[
        Extension(
            name='whitespace_module', 
            sources=[os.path.join(python_dir, 'whitespace_module.c')],
        ),
    ],
)
