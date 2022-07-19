
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import json
import os

def generate_config(output_folder, name=""):
    config = {}
    config["path_to_experiments"] = [
        "path_to_experiment_1",
        "path_to_experiment_2",
        "path_to_experiment_3",
        "path_to_experiment_4"
    ]
    config["labeler_names"] = [
        "Matvey_relabel_json",
        "M_updated_NODAL25_json",
        "M_updated_NODAL50_json",
        "M_updated_NODAL75_json",
        "D_json",
        "Daniel_2_json",
        "Daniel_new_json",
        "Daniel_json",
    ]

    config["algorithm_name"] = ["Classified_result_1_json", "Classified_result_1_json"]
    config["filters"] = ["A001","B001","C001","D001","Experiment_01"]
    config["class_names"] = ["NORMAL", "NODAL", "BMP",
                             "FGF", "WNT", "SHH", "PCP", "RA",
                             "BOOM", "UNKNOWN"]
    config["with_severities"] = False
    config["classes_with_severities"] = ["BMP", "NODAL"]
    config["severities"] = [100, 75, 50, 25]
    config["fig_size"] = [11, 10]
    name_prefix = ""
    if (name):
        name_prefix = name + "_"



    with open(os.path.join(output_folder, name_prefix +'confusion_matrix_cfg.json'), 'w') as f:
        json.dump(config, f, indent=4)


def config_path_to_list(config_file_path):
    with open(config_file_path) as f:
        cfg = json.load(f)
        return [
            cfg["path_to_experiments"],
            cfg["labeler_names"],
            cfg["algorithm_name"],
            cfg["filters"],
            cfg["class_names"],
            cfg["with_severities"],
            cfg["classes_with_severities"],
            cfg["severities"],
            cfg["fig_size"] # !important! because matplot lib this  is in inches!
        ]
