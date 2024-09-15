========
cppmagic
========

`cppmagic` is a tool for managing and organizing C++ source files and headers. It provides functionality to generate, sort, and synchronize C++ files based on headers.

Installation
============

This program works based on the output generate by ctags.

.. code-block:: bash

   # Runtime dependency
   sudo apt install universal-ctags
   # Optional dependency.
   sudo apt install libgtest-dev libdw-dev libunwind-dev

.. code-block:: bash

   git clone https://github.com/ms-jagadeeshan/cppmagic.git
   cd cppmagic && mkdir build && cd build
   cmake ..
   make -j$(nproc)

Usage
=====

`cppmagic <subcommand> <options> files/folders`

Subcommands
-----------

- `generate` Generate .cpp from header.
- `sort`     Sort the functions in .cpp with respect to given sorting method.
- `boom`     Sort the functions in .cpp and fills the missing functions in .cpp

Options
-------
- `-h,--help`        Print this help message and exit
- `--method TEXT`    Sorting method. header, alpha (Default : header)


Common Options
--------------
- `--ctags-path ...`       Path of ctags executable.(Default: /usr/bin/ctags)
- `--header-ext ...`       File extensions to be considered as headers.
- `--source-ext ...`       File extensions to be considered as sources.
- `-i,--in-place SUFFIX`   Edit files in place (makes backup if SUFFIX supplied)

Arguments
---------

To sort C++ files based on their header files, you can specify directories or individual files:

1. **Sorting Directories**: If you have both headers and sources in separate directories:

- **HEADER_FOLDER**: The directory containing your header files (e.g., `include/mylibrary`).
- **SOURCE_FOLDER**: The directory where you want the generated source files to be placed (e.g., `src/`).

.. code-block:: bash
   
   ./cppmagic sort [HEADER_FOLDER] [SOURCE_FOLDER]
   

2. **Sorting Single Folder**: If your headers and sources are in the same directory:

.. code-block:: bash

   ./cppmagic sort [FOLDER]

3. **Sorting Files**: if you want to sort certail files only:

.. code-block:: bash

   ./cppmagic sort [FILES]


Example
=======

To sort functions in .cpp with respect to function order in header.

.. code-block:: txt

   cppmagic/
   ├── CMakeLists.txt
   ├── include/
   │   ├── cppmagic/
   │   │   ├── CppMagic.hpp
   │   │   ├── FilePath.hpp
   │   │   ├── FileSelector.hpp
   │   │   ├── GenerateMode.hpp
   │   │   └── ... (other headers)
   ├── src/
   │   ├── CppMagic.cpp
   │   ├── FilePath.cpp
   │   ├── FileSelector.cpp
   │   ├── GenerateMode.cpp
   │   └── ... (other files)

`cppmagic sort include/cppmagic src`

.. code-block:: txt

   cppmagic/
   ├── CMakeLists.txt
   ├── src/
   │   ├── CppMagic.cpp
   │   ├── FilePath.cpp
   │   ├── FileSelector.cpp
   │   ├── GenerateMode.cpp
   │   ├── ... (other files)
   │   ├── CppMagic.hpp       
   │   ├── FilePath.hpp       
   │   ├── FileSelector.hpp   
   │   ├── GenerateMode.hpp   
   │   └── ... (other headers)

`cppmagic sort src`
