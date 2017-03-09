#/bin/sh

echo Main build script for project.
echo Performs clean build deleting all previous results

# clear all dirs
if [ -d doc ]; then
	rm -rf doc
fi
if [ -d bin ]; then
	rm -rf doc
fi
if [ -d lib ]; then
	rm -rf doc
fi

# rem recreate dirs
mkdir bin

# rem start build process from bin dir
cd bin
# rem Additional flags can be passed here or in CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Debug ../
make -j4
ctest --verbose

cd ../configs
doxygen doxyfile
cd ..