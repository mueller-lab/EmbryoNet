This directory contains scripts related to the trained model export. 
  
* `create_configuration_json.py`

Generates file ```DefaultConfig.json``` used in EmbryoExperimentRunner tool.
Arguments shall be given in the order, they were given in training. 

`python create_configuration_json.py  --classes BMP NODAL NORMAL WNT CUT --number_of_classes 4 --model_name model_name.onnx  -o <output_folder>` 

*Important `number_of_classes` is classifier model output channel number and 
can be different than given "classes" count, (for example "CUT" is not classification output).*

* `export_embryo_model.py`

Exports trained model from PyTorch training pipeline to ONNX format, used for inference in EmbryoExperimentRunner tool.

`python export_embryo_model.py --path_to_model <path_to_model.pth>  --path_to_output <path_to_output_folder>
 --batch_size 8 --number_of_classes 6` 

*Important. Current ExperimentRunner tool uses batch size of 8.*

*Important. To generate Class Activation Maps, exported model with batch size of 1 should be used.*
 
