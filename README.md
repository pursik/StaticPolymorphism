# DataExchange
Applications for data exchange. 

1. Install Conan 2.0.7: Make sure Conan 2.0.7 is installed. If not, you can install it using pip:
```
pip install conan==2.0.7
```
2. Install Dependencies: Run Conan to install third libraries dependency using the conanfile.py:
```
conan install -of bin-release . --build=missing --settings build_type=Release
conan install -of bin-debug . --build=missing --settings build_type=Debug
```
This will:
Download and install the necessary third libraries.
Generate the CMake configuration files (like conan_toolchain.cmake and Boost-config.cmake) to integrate them into your project.
