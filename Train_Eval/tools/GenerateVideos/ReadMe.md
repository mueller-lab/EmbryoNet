This directory contains scripts to visualize classifier model internal activations via <a href="https://github.com/zhoubolei/CAM">Class Activation Map</a> (CAM) algorithm.

To use CAM visualization scripts model must be exported to ONNX with batch size of 1.

* `add_fc_output.py`

Given embryo classification model adds the last fully connected layer output to ONNX format output, so that it can be accessed for CAM calculation.

`python add_fc_output.py  -i <path_to_model_with_batch_1.onnx> -o <path_to_output_model.onnx>`
 
 
* `generate_cam.py`

Given path to folder with subfolders containing only embryo ROI images, generates class activation maps, and stores them in output path.    

`python generate_cam.py -f <path_to_input_images_folder> -mp <path_to_prepared_model> -sp <path_to_output_folder>`

* `generate_CAM_video.py` 

Generates CAM input/output collage for a given prepared model, track_ids and experiment. 
Model exported with batch size of 1 and added fc_output is required. 
`wsh` and `wsw` should be set manually and can be visualized with 'netron'.

See --help for details. 
 
 
`python generate_CAM_video.py --model_path <path_to_prepared_model.onnx> -e <path_to_experiment_with_json_and_images_folders> -name test --ids <1 2 3 .. i > -o <output_folder_path> `
