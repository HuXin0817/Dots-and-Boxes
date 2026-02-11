from setuptools import setup

setup(
    name='PyDab',
    version='0.1.0',
    description='Dots and Boxes game Python binding',
    author='Xin Hu',
    author_email='202219120810@stu.cdut.edu.cn',
    url='https://github.com/HuXin0817/Dots-and-Boxes',
    data_files=[
        ("site-packages", ["../build/python/PyDab.cpython-313-darwin.so"])
    ],
    install_requires=['pybind11'],
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: C++',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)
