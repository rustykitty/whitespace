from setuptools import setup, Extension

setup(
    name='foo',
    ext_modules=[
        Extension(
            name='parse_python_tree', 
            sources=['parse_python_tree.c'],
            # extra_compile_args=['-O0', "-g", "-UNDEBUG"],
        ),
    ],
)
