This folder contains main training script.

* `train.py`

`python train.py -i <path_to_folder_with_train_datasets>
 --start_epoch <start_epoch> --save_path <path_to_save_folder/checkpoint_name>
 --severities [severities list] --classes_with_severities <classes_with_severities>
 --num_epochs <epoch_run_number> 
 --start_model <path_to_start_model checkpoint.pth> ` 

parameter passing examples: 
 
`--severities 50 75 100`  will use 50, 75  and 100 as severity values

`--classes_with_severities "BMP" "NODAL"` - will use `--severities 50 75 100` for BMP and NODAL classes

`--start_epoch 100` is used for creating augmentation mappers 

`--num_epochs 1000 ` - will run 1000 epochs

`--start_model F:\GlobalModel\models\All_train_EmbryoNetV2_BMPSeverities_16Classes_FRESH_ALL_BMP_NODAL_SEVERE_56_.pth ` - path to the model checkpoing

`--classes "BMP", "NODAL", "NORMAL", "BOOM", "UNKNOWN"` - will use "BMP", "NODAL", "NORMAL", "BOOM", "UNKNOWN" classes 

`--num_classes 16` - output channel number for EmbryoNet model.

