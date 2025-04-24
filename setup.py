from setuptools import setup, Extension

setup(
    name='foo',
    ext_modules=[
        Extension(
            name='whitespace_module', 
            sources=['whitespace_module.c'],
        ),
    ],
)
