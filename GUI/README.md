# EmbryoClassifier and EmbryoLabeler
How to build EmbryoClassifier
1) Install <a href="https://cmake.org/download/"><b>CMake</b></a> 
2) Install <a href= "https://docs.microsoft.com/de-de/visualstudio/releases/2019/history"> <b>Visual Studio 2019</b> </a>
3) Download <a href="https://drive.google.com/file/d/1JTOIYtY5jYUGbXGmBsgfuLHoRTNAc3qx/view?usp=sharing"><b>dependencies</b></a>
5) Open command line (cmd) and run dependencies/scripts/prepare_env.bat
6) In command that (4) command line write "cmake-gui"
7) Set "where is the source code to  "https://github.com/hagorms/EmbryoNet/tree/main/GUI"
8) Set "Where to build binaries to "https://github.com/hagorms/EmbryoNet/tree/main/GUI/build" 
9) press Configure 
10) press Generate 
11) Open Project in Microsoft Visual Studio 2019
12) Right click on "Solution in  "Embryo_classifier" and "Build solution"


Starting from 5) you can alternatively continue in command line with activated environment
1) cd \<GUI path\>
2) mkdir build && cd build
3) cmake .. -G"Visual Studio 16 2019" -A x64
4) Open Project in Microsoft Visual Studio 2019
5) Right click on "Solution in  "Embryo_classifier" and "Build solution"
