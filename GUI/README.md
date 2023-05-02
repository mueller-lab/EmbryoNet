# EmbryoClassifier and EmbryoLabeler
How to build EmbryoClassifier - You can also follow the step-by-step <a href="https://embryonet.uni-konstanz.de/tutorial.html"><b> Video Tutorial</b></a> 
1) Install <a href="https://cmake.org/download/"><b>CMake</b></a> 
2) Install <a href="https://developer.nvidia.com/cuda-11-4-4-download-archive/"><b>Cuda 11.4</b></a>   
3) Install <a href= "https://docs.microsoft.com/de-de/visualstudio/releases/2019/history"> <b>Visual Studio 2019</b> </a>
4) Download the <a href="https://cloud.uni-konstanz.de/index.php/s/YqdYsBFZmW2PxYi"><b>dependencies</b></a> and clone the EmbryoNet repository (e.g. git clone https://github.com/mueller-lab/EmbryoNet.git)
5) Open the command line (cmd) in the folder "dependencies/scripts/" and run "prepare_env.bat"
6) In the cmd of (4) write "cmake-gui" 
7) Set "where is the source code" to  "path_to_GUI" e.g. "C:\EmbryoNet\GUI". This is the local path to the cloned repository.
8) Set "Where to build binaries" to "path_to_build" e.g. "C:\EmbryoNet\GUI\build". 
9) Click "Configure" (specify the generator of the project as "Visual Studio 16 2019").
10) Click "Generate". 
11) Open the project in Microsoft Visual Studio 2019. The project should be located at "path_to_build", e.g. "C:\EmbryoNet\GUI\build\Embryo_classifier.sln".
12) Change the configuration mode to "Release". 
13) Select all targets in the solution explorer, right-click on them and choose "Build selection". If you want to use GPU for the EmbryoClassifier, modifiy the line 20 in "your_path\EmbryoNet\GUI\CVStudio_sources\FishClassifier_lib\include\EmbryoClassifier.h" to "#define WITH_ONNXRUNTIME 1", and Rebuild "FishClassifier_gui" and "FishClassifier_lib".
15) The executables EmbryoLabeler and EmbryoClassifier (CPU and GPU) will be genrated at the follwing paths: "$path_to_build\CVStudio_sources\FishLabeler\Release\FishLabeler.exe", "$path_to_build\CVStudio_sources\FishClassifier_gui\CPU\Release\FishClassifier_gui_CPU.exe" and "$path_to_build\CVStudio_sources\FishClassifier_gui\GPU\Release\FishClassifier_gui_CPU.exe", respectively.

Starting from 5) you can alternatively continue in command line with the activated environment
1) cd \<GUI path\>
2) mkdir build && cd build
3) cmake .. -G"Visual Studio 16 2019" -A x64
4) Open the project in Microsoft Visual Studio 2019. The project should be located at "path_to_build" e.g. "C:\EmbryoNet\GUI\build\Embryo_classifier.sln"
5) Change the configuration mode to "Release"
6) Select all targets in the solution explorer, right-click on them and choose "Build selection"
7) The executables EmbryoLabeler and EmbryoClassifier (CPU and GPU) will be generated at the following paths: "$path_to_build\CVStudio_sources\FishLabeler\Release\FishLabeler.exe", "$path_to_build\CVStudio_sources\FishClassifier_gui\CPU\Release\FishClassifier_gui_CPU.exe" and "$path_to_build\CVStudio_sources\FishClassifier_gui\GPU\Release\FishClassifier_gui_CGU.exe", respectively.


Requirements

- Standard PC, 16Gb RAM, Windows 10
- For GPU calculations: Nvidia Graphic Card and CUDA 11.4 installed. The installation and demo were tested in Windows10.

Installation takes 10 minutes after Visual Studio is installed (10 minutes) and the data is downloaded (23 minutes).
