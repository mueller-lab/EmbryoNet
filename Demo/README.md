# Demo on using EmbryoClassifier

1) <a href="https://github.com/mueller-lab/EmbryoNet/tree/main/GUI"><b>  Build EmbryoClassifier </b></a> 
2) Run the exectuable file FishClassifier_gui_CPU.exe or FishClassifier_gui_GPU.exe - if you want to use CPU or GPU for the classification
3) In the GUI, open the folder with experiments, e.g. "your_path\EmbryoNet\Demo", pressing "Load"
4) Press "Start"
5) You can see the results using the GUI: FishLabeler.exe


Requirements

- Standard PC, 16Gb RAM, Windows 10
- For GPU calculations: Nvidia Graphic Card and CUDA 11.4 installed. The installation and demo were tested in Windows10

Execution Time for Demo (10 embryos and 720 time points):

- CPU : 68 minutes,  in a Laptop Intel(R) Core(TM) i7-10870H CPU @ 2.20GHz, 16 GB RAM
- GPU : 5.5 minutes,  in a Laptop with NVIDIA GeForce RTX 3070, Intel(R) Core(TM) i7-10870H CPU @ 2.20GHz, 16 GB RAM, 

To run the software on your data have it organized in the following way:

1) Make one parent directory that contains your experiment folder. e.g. "D:/Parent/experiment_1/". In this experiment folder your images should be stored inside a folder called "images"
2) Measure the diameter of your embryos in Fiji.
3) Load the folder by selecting the Parent directory.
4) Adjust the settings accordingly. At the moment two different time stamp formats are supported: pick LO[0-9]+ if the timepoint in your image files is designated by LO (e.g. Acquifer imaging machine) or change it to T[0-9]+ if it is defined by t. Set the minimum and maximum diameter (in pixels) for an embryo in your images.
5) In the GUI, open the folder with experiments, e.g. "your_path\EmbryoNet\Demo", pressing "Load"
6) Press "Start"
 
** To reproduce the results from the manuscript, download the test data from https://doi.org/10.48606/29 and https://doi.org/10.48606/30 and run the classifier on it. For data recorded with the Acquifer imaging machine use the default settings, for Keyence data set the minimum diameter to 300µm and the maximum to 390µm, and change the time format to T[0-9]+.
