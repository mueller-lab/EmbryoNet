This directory contains scipt `plot_confusion_matrices.py` to generate confusion matrices in the following two steps.

1\) Generate config for plotting confusion matrix

`python plot_confusion_matrices.py -g -o <path_to_output_folder>`

When called this way, the script generates default config. The user then has to manually specify exact paths to experiments in `path_to_experiments` list.
Each of this experiments is expected to have annotations from ```labeler_names``` list. 

Important:  one has use "\\\\" in windows to set `path_to_experiments` in generated json.

Example:
`"F:\\processed_data\\EVALUATION\\200512_swr_n_oep\\mixed"`

2\) Generate confusion matrix with the prepared config

After the config for plotting confusion matrix is ready, use the following command

`python plot_confusion_matrices.py -o <path_to_output>  -c <path_to_modified_cfg.json>`

to perform the actual confusion matrix generation.

The resulting confusion matrix is saved in `<path_to_output>` folder in two forms:
 * <cfg_filename_cm.svg> visualized in SVG format
 * <cfg_filename_cm.csv> in textual CSV format.

Also CSV file called 
 * <cfg_filename_bins.csv>
 
 is generated with the exact number of entries in each of confusion matrix cells.

See --help for more details. 