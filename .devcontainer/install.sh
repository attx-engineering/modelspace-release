#/bin/bash

# Ensure our distro is updated to the latest
sudo apt update
sudo apt-get update

# Make sure configuration management tools are available
sudo apt-get install -y git

# Install build packages necessary to build C++ and FORTRAN code
sudo apt-get install -y gcc g++ cmake gfortran

# Install python and SWIG (we need Python dev to support SWIG)
sudo apt-get install -y python3-dev swig pip

# Install HDF5 package for logging
sudo apt-get install -y libhdf5-serial-dev 

# Install ogre to produce visuals + associated packages for graphics
sudo apt-get install -y ogre-1.12

# Build documentation and code coverage analysis tools
sudo apt-get install -y doxygen clang graphviz
sudo apt-get install -y gcovr lcov
sudo apt-get install -y asciidoctor
sudo apt-get install -y asciidoc

# Now install all of the python packages necessary to support ModelSpace
sudo apt-get install -y python3-matplotlib python3-plotly python3-numpy python3-pandas