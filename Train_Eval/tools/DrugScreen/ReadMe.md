This folder contains three scripts used for the drug screen analysis:

1. Drug screen script 1: files_sort_for_EmbryoNet.py (optional)
2. Drug screen script 2: jsonread.py
2. Drug screen script 3: drugscreen_utils.py

The scripts were written to provide the following workflow as an automated pipeline for the analysis of drug-treated embryos in multiplates:
1. Sort images for analysis with EmbryoNet GUI based on well positions (Drug screen script 1)
2. Run EmbryoNet GUI on data (please see GUI folder)
3. Extract phenotype predictions from JSON files created by EmbryoNet GUI and save predictions, sorted by wells, to separate Excel file (Drug screen script 2)
4. Use Excel file to plot phenotype predictions as bar chart with phenotype counts, plate overviews with exact predictions and plate overviews with majority vote predictions (Drug screen script 3)

Usage examples:
python files_sort_for_EmbryoNet.py
-i C:/directory_root/ will glob for image files in this directory
-o C:/directory_output/ (optional, if not passed: images will be sorted within the root directory)

python jsonread.py
-i C:/directory_output/ will glob for subfolders with JSON files containing phenotype predictions in this directory
-d zfish_Classified_result_1_json_corrected is the standard subfolder name where JSON files containing EmbryoNets' phenotype predicitons can be found within all well subdirectories
-o C:/directory_output/plate1_phenotype_predictions.xlsx specifies the name of the Excel file where phenotype predictions for the given input files are saved
-p 1 is the number of the drug screen plate
-s 2840 is the number of the drug screen library

python drugscreen_utils.py
-i C:/directory_output/plate1_phenotype_predictions.xlsx is the path to the Excel file where phenotype predictions for the given input files are stored
-m Model_1 is the name of the model used with the EmbryoNet GUI to calculate phenotype predictions for the analyzed embryos
-o C:/directory_analysis/ is the directory, where the three different plots are saved for the specified input excel file
-p 1 is the number of the drug screen plate
-s 2840 is the number of the drug screen library