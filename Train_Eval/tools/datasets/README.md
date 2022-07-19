This directory contains scripts to work with annotated datasets. 
 
 
* `parse_experiment_dataset.py`

Given path to folder with images and folder with annotation files in json format will form a single dataset
 used in training with "embryo ages" added.

* `create_phenotype_datasets.py`

`python create_phenotype_datasets.py -p F:/BMP-TRAINING -l J_json -o F:/datasets/J_bmp_training -t LO`

Calls `parse_experiment_dataset.py` to  parse phenotype database by converting EmbryoLabeler tool annotation
representation to representation used in training.  
One phenotype has several sessions, recordered in different days/microscopes etc.
Each session has several experiments (wells) with microscope images. 

* `relabel_embryo_phenotype.py`

Sets embryo labels in dataset after given timestamp into a given "class name" with given "severity".
Embryos labeled to a class from "ignore_classes" list are not going to be overriden. 

`python relabel_embryo_phenotype.py -p "F:/processed_data/BMP-TRAINING\200218_BMP_inibitor" -ln "J_json" -st 100 -se 50 -cn BMP -uan updated_johanna_test `

* `collect_datasets_description.py` 

Creates an .xlsx file with dataset woth several pages (page_name = experiment_name) and experiment description:
 - Drug (has to be filled by human)
 - Concentration (has to be filled by human)
 - Test  0 or 1 (1 if it was used for test)
 - Train 0 or 1 (1 if it was used for train)
 - path_to_labeled_data path to data, visualized by EmbryoLabeler tool 
 - path_to_dataset path to dataset created by create_phenotype_datasets.py 
 - path_to_experiment path to experiment (experiment is a folder inside "Phenotype")
 - images_count 
 - labeler_info - labeler names and json count for each labeler 
 
 *Important! Test and Train split is needed for `plot_sessions_from_excel.py`.* 
 *If no test/train folder was provided, custom modifications have to be made in `plot_sessions_from_excel.py`"*

`python collect_datasets_description.py 
                -l <path_to_embryo_labeler_datasets (this folder is an original input for create_phenotype_datasets)>
                -d <path_to_train_datasets_made_by_create_phenotype_datasets> (test and train is splitted by user)
                    <path_to_test_datasets_made_by_create_phenotype_datasets> 
                -o <path_to_output_folder>` 
                
                
  