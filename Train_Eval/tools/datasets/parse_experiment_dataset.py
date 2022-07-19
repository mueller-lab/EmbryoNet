#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import argparse
import json
import os
import re

def main(args):
    timestamp_format = args.timestamp_format
    timestamp_format_length = len(timestamp_format)
    concatenated_json = []
    jsons = [os.path.abspath(os.path.join(args.path_to_jsons, fj)) for fj in os.listdir(args.path_to_jsons)]
    for json_file_path in jsons:
        with open(json_file_path, "r") as json_file:
            json_dict = json.load(json_file)
            img_path = os.path.basename(json_dict["source_name"])
            age_fmt = re.findall(timestamp_format + "\d+", img_path)
            age = float(age_fmt[0][timestamp_format_length:]) / float(len(os.listdir(args.path_to_images)))
            for detection in json_dict["detection_list"]:
                if detection["className"] == "CUT":
                    continue
                detection["age"] = age
                path_to_save = os.path.join(args.path_to_images, img_path)
                concatenated_json.append([path_to_save, detection])

    with open(args.path_to_output_json, 'w') as f:
        json.dump(concatenated_json, f)


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('--path_to_images', '-i')
    parser.add_argument('--path_to_jsons' , '-j')
    parser.add_argument('--path_to_output_json' , '-o')
    parser.add_argument('--timestamp_format', "-t", default="LO", help = "timestamp format. Default  is LO\d+")

    args = parser.parse_args()
    main(args)
