This directory contains script to run embryo classification with python. 

* `run_model.py` 

Reimplements EmbryoExperimentRunner detection and classification functionality without tracking stage. 

pytorch or onnx model can be used. 
EmbryoExperimentRunner config can be used to simulate EmbryoExperimentRunner behaviour. 

Important! User defined model output channels number must be equal to config "outputClasses" field.

`python run_model -c <path_to_config.json>  -i <path_to_folder_with_images> -t <max_timestamp> -o <save_path>`

`python run_model -c <path_to_config.json> -m <path_to_model.onnx/path_to_model.pth>  -i <path_to_folder_with_images> -t <max_timestamp> -o <save_path>`
 
See --help for more details. 
