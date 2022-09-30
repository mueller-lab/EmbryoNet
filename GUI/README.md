# EmbryoClassifier and EmbryoLabeler
How to build EmbryoClassifier - You can also follow the step-by-step <a href="http://embryonet.de/tutorial.html"><b> Video Tutorial</b></a> 
1) Install <a href="https://cmake.org/download/"><b>CMake</b></a> 
2) Install <a href= "https://docs.microsoft.com/de-de/visualstudio/releases/2019/history"> <b>Visual Studio 2019</b> </a>
3) Download <a href="https://drive.google.com/file/d/1JTOIYtY5jYUGbXGmBsgfuLHoRTNAc3qx/view?usp=sharing"><b>dependencies</b></a> and clone the EmbryoNet repository (e.g. git clone https://github.com/mueller-lab/EmbryoNet.git)
5) Open the command line (cmd) and run "dependencies/scripts/prepare_env.bat"
6) In the cmd of (4) write "cmake-gui" 
7) Set "where is the source code to  "path_to_GUI" e.g. "C:\Users\Documents\EmbryoNet\EmbryoNet\GUI" . This is the local path to the cloned repository
8) Set "Where to build binaries to "path_to_build" e.g. "C:\Users\Documents\EmbryoNet\EmbryoNet\GUI\build" 
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
