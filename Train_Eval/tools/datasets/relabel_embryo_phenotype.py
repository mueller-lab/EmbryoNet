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

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('--path_to_session',"-p", type=str)
    parser.add_argument('--labeler_name', "-ln",type=str)
    parser.add_argument('--updated_annotation_name',"-uan", type=str)
    parser.add_argument('--start',"-st", type=int, default=640)
    parser.add_argument('--severe',"-se", type=int, default=100)
    parser.add_argument('--class_name',"-cn", type=str, default="NORMAL")
    parser.add_argument('--ignore_class', "-ic",  nargs="+", default=["BOOM", "CUT"])
    parser.add_argument('--timestamp_prefix', "-tp", type = str, default= "LO")
    args = parser.parse_args()
    class_name = args.class_name.upper()
    ignore_class_names = [ignore_class.upper() for ignore_class in args.ignore_class]
    timestamp_prefix = args.timestamp_prefix
    path_to_session = args.path_to_session
    labeler_name = args.labeler_name
    updated_annotation_name = args.updated_annotation_name
    if "json" not in updated_annotation_name:
        updated_annotation_name += "_json"
    experiments = [f for f in os.listdir(path_to_session) if os.path.isdir(os.path.join(path_to_session, f))]

    for experiment in experiments:
        path_to_experiment = os.path.join(path_to_session, experiment)
        path_to_json_folder = os.path.join(path_to_experiment, labeler_name)

        if os.path.isdir(path_to_experiment) and os.path.exists(path_to_json_folder):
    
            path_to_save_annotation = os.path.join(path_to_experiment, updated_annotation_name)
            if os.path.exists(path_to_save_annotation) == False:
                os.makedirs(path_to_save_annotation, exist_ok=False)
    
            for json_file in os.listdir(path_to_json_folder):
                age = re.findall(timestamp_prefix+ "\d+", json_file)
                age = int(age[0][len(timestamp_prefix):])
                path2Json = os.path.join(path_to_json_folder, json_file)
                path_to_save_annotation_file = os.path.join(path_to_save_annotation, json_file)
                with open(path2Json,) as f:
                    annotation = json.load(f)
    
                    for detection in annotation["detection_list"]:
                        if args.start == 0:
                            if detection not in ignore_class_names:
                                detection["className"] = class_name
                                detection["severe"] = args.severe
    
    
                        if (args.start > 0) and (args.start < age):
                            if detection["className"] not in ignore_class_names:
                                detection["className"] = class_name
                                detection["severe"] = args.severe
                            if detection["className"] in (ignore_class_names + ["UNKNOWN"]):
                                detection["severe"] = 100
                        else:
                            detection["severe"] = 100
    
    
                    updated_annotation = annotation
    
    
                with open(path_to_save_annotation_file,"w") as fanno:
                    json.dump(updated_annotation, fanno, indent=4)


if __name__=="__main__":
    main()
