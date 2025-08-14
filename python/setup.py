from setuptools import setup, Extension

import os.path
from os import environ as env

python_dir = os.path.dirname(__file__)

setup(
    name='whitespace_module',
    py_modules=['whitespace_module'],
    ext_modules=[
        Extension(
            name='whitespace_module', 
            sources=[os.path.join(python_dir, 'whitespace_module.c')],
            extra_compile_args=env.get('CFLAGS', '').split(),
            extra_link_args=env.get('LDLIBS', '').split()
        ),
    ],
)
