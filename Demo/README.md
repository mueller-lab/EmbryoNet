# EmbryoClassifier and EmbryoLabeler
How to build EmbryoClassifier - You can also follow the step-by-step <a href="http://embryonet.de/tutorial.html"><b> Video Tutorial</b></a> 
1) Install <a href="https://cmake.org/download/"><b>CMake</b></a> 
2) Install <a href= "https://docs.microsoft.com/de-de/visualstudio/releases/2019/history"> <b>Visual Studio 2019</b> </a>
3) Download <a href="https://drive.google.com/file/d/1JTOIYtY5jYUGbXGmBsgfuLHoRTNAc3qx/view?usp=sharing"><b>dependencies</b></a> and clone the EmbryoNet repository (e.g. git clone https://github.com/mueller-lab/EmbryoNet.git)
5) Open the command line (cmd) in the folder "dependencies/scripts/" and run "prepare_env.bat"
6) In the cmd of (4) write "cmake-gui" 
7) Set "where is the source code to  "path_to_GUI" e.g. "C:\EmbryoNet\GUI" . This is the local path to the cloned repository
8) Set "Where to build binaries to "path_to_build" e.g. "C:\EmbryoNet\GUI\build" 
9) press Configure  (specify the generator of the project as "Visual Studio 16 2019")
10) press Generate 
11) Open the project in Microsoft Visual Studio 2019. The project sould be located at "path_to_build" e.g. "C:\EmbryoNet\GUI\build\Embryo_classifier.sln"
12) Change the configuration mode to "Release" 
13) Select all target in the solution explorer, right click over them and choose "Build selection"
14) The executables EmbryoLabeler and EmbryoClassifier (CPU and GPU) will be genrated at the follwing paths: "$path_to_build\CVStudio_sources\FishLabeler\Release\FishLabeler.exe", "$path_to_build\CVStudio_sources\FishClassifier_gui\CPU\Release\FishClassifier_gui_CPU.exe" and "$path_to_build\CVStudio_sources\FishClassifier_gui\GPU\Release\FishClassifier_gui_CPU.exe", respectively.

Starting from 5) you can alternatively continue in command line with the activated environment
1) cd \<GUI path\>
2) mkdir build && cd build
3) cmake .. -G"Visual Studio 16 2019" -A x64
4) Open the project in Microsoft Visual Studio 2019. The project sould be located at "path_to_build" e.g. "C:\EmbryoNet\GUI\build\Embryo_classifier.sln"
5) Change the configuration mode to "Release" 
6) Select all target in the solution explorer, right click over them and choose "Build selection"
7) The executables EmbryoLabeler and EmbryoClassifier (CPU and GPU) will be genrated at the follwing paths: "$path_to_build\CVStudio_sources\FishLabeler\Release\FishLabeler.exe", "$path_to_build\CVStudio_sources\FishClassifier_gui\CPU\Release\FishClassifier_gui_CPU.exe" and "$path_to_build\CVStudio_sources\FishClassifier_gui\GPU\Release\FishClassifier_gui_CPU.exe", respectively.