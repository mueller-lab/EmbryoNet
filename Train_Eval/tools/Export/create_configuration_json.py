
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import argparse
import json
import re
import os

from collections import OrderedDict

def generate_template_config():

    cfg = dict()
    cfg["Source"] = {
        "type": "Path",
        "folder": "%2"
    }

    cfg["Algorithm"] = {
        "type": "EmbryoClassifer",
        "classifier": {
            "path_to_net": None,
            "name": "Fish",
            "type": "OpenCV",
            "engine": "ONNX",
            "run": True,
            "outputClasses": None,
            "height": 224,
            "width": 224,
            "minus": [
                0.0,
                0.0,
                0.0
            ],
            "divide": [
                1.0,
                1.0,
                1.0
            ]
        },
        "tracker": {
            "detector": {
                "name": "FishDetector",
                "engine": "CPU",
                "fx": 0.25,
                "fy": 0.25,
                "hough": {
                    "method": "HOUGH_GRADIENT",
                    "dp": 1,
                    "minDist": 20,
                    "param1": 50,
                    "param2": 30,
                    "minRadius": 40,
                    "maxRadius": 55
                }
            }
        },
        "logic": {
            "transactions": None
        }
    }

    cfg["ResultRepresenter"] = [
        {
            "type": "labeler",
            "experiment_folder": "%1",
            "classes": None,
        }
    ]

    cfg["classes"] = None
    return cfg


def generate_possible_transactions(phenotype_classes):
    all_transactions = [
        [
            "UNKNOWN",
            "BMP",
            "BOOM"
        ],
        [
            "UNKNOWN",
            "BMP"
        ],
        [
            "UNKNOWN",
            "NODAL"
        ],
        [
            "UNKNOWN",
            "NORMAL"
        ],
        [
            "UNKNOWN",
            "FGF",
            "BOOM"
        ],
        [
            "UNKNOWN",
            "FGF"
        ],
        [
            "UNKNOWN",
            "WNT",
            "BOOM"
        ],
        [
            "UNKNOWN",
            "WNT"
        ],
        [
            "UNKNOWN",
            "SHH",
            "BOOM"
        ],
        [
            "UNKNOWN",
            "SHH"
        ],
        [
            "UNKNOWN",
            "PCP",
            "BOOM"
        ],
        [
            "UNKNOWN",
            "PCP"
        ],
        [
            "UNKNOWN",
            "RA",
            "BOOM"
        ],
        [
            "UNKNOWN",
            "RA"
        ],
        [
            "UNKNOWN",
            "BOOM"
        ]
    ]
    result_transactions = []

    for transaction in all_transactions:
        if all(phenotype_class in phenotype_classes for phenotype_class in transaction):
            result_transactions.append(transaction)
    return result_transactions


def get_phenotype_colors(phenotype_classes):
    all_classes_colormap = OrderedDict()
    all_classes_colormap["BMP"] = [255, 0, 0]
    all_classes_colormap["NODAL"] = [0, 0, 255]
    all_classes_colormap["NORMAL"] = [0, 255, 0]
    all_classes_colormap["UNKNOWN"] = [0, 0, 0]
    all_classes_colormap["BOOM"] = [255, 0, 255]
    all_classes_colormap["WNT"] = [0, 255, 255]
    all_classes_colormap["FGF"] = [255, 255, 0]
    all_classes_colormap["SHH"] = [255, 128, 128]
    all_classes_colormap["PCP"] = [0, 128, 255]
    all_classes_colormap["RA"] = [128, 0, 128]
    all_classes_colormap["CUT"] = [255, 255, 255]

    return {
        phenotype_class: color for (phenotype_class, color) in all_classes_colormap.items()
        if phenotype_class in phenotype_classes
    }

def split_class_severity(classes_with_severe):
    result_classes = OrderedDict()

    for class_with_severe in classes_with_severe:
        severity = re.findall("\d+", class_with_severe)
        class_name = re.findall("\D+", class_with_severe)

        if len(class_name)== 0:
            print("ignoring " + class_with_severe)
            continue

        class_name = class_name[0]
        if class_name not in result_classes:
            result_classes[class_name] = []

        if len(severity) > 0:
            result_classes[class_name].append(int(severity[0]))

    return result_classes


def combine_class_severity_color(class_2_severity, phenotype_colors):
    combined_classes_to_severity = []
    for id, class_with_severity in enumerate (class_2_severity):
        result_class = {}
        result_class["id"]=id
        result_class["name"] = class_with_severity
        if len(class_2_severity[class_with_severity]) :
            result_class["severities"] = class_2_severity[class_with_severity]
        result_class["color"]= phenotype_colors[class_with_severity]
        combined_classes_to_severity.append (result_class)
    return combined_classes_to_severity

def main(classes, number_of_classes, model_name, output_folder):

    cfg = generate_template_config()
    class_2_severity = split_class_severity(classes)
    phenotype_classes = list(class_2_severity.keys())

    transactions = generate_possible_transactions(phenotype_classes)
    phenotype_colors = get_phenotype_colors(phenotype_classes)
    cfg["Algorithm"]["classifier"]["path_to_net"] = "%3/" + model_name
    cfg["Algorithm"]["logic"]["transactions"] = transactions
    cfg["Algorithm"]["classifier"]["outputClasses"]= int(number_of_classes)
    created_classes = combine_class_severity_color(class_2_severity, phenotype_colors)

    cfg["ResultRepresenter"][0]["classes"] = created_classes
    cfg["classes"] = created_classes

    with open(os.path.join(output_folder, "DefaultConfig.json"),"w") as f:
        json.dump(cfg, f, indent=4)


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('--classes', nargs="+", required=True)
    parser.add_argument('--number_of_classes', type=int, required=True)
    parser.add_argument('--model_name', type=str, required=True)
    parser.add_argument("-o", "--output_folder", type=str, required=True)

    args = parser.parse_args()

    main(
        args.classes,
        args.number_of_classes,
        args.model_name,
        args.output_folder
    )
