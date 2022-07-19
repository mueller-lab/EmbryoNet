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
import numpy as np

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('--path_to_session',"-p", type=str)
    parser.add_argument('--labeler_name', "-ln",type=str)
    parser.add_argument('--updated_annotation_name',"-uan", type=str)
    parser.add_argument('--start',"-st", type=int, default=640)
    parser.add_argument('--ignore_class', "-ic",  nargs="+", default=["NORMAL", "BOOM", "CUT"])
    parser.add_argument('--timestamp_prefix', "-tp", type = str, default= "LO")
    args = parser.parse_args()
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
            
            allIds = []
            allages = [] 
    
            for json_file in os.listdir(path_to_json_folder):
                age = re.findall(timestamp_prefix+ "\d+", json_file)
                age = int(age[0][len(timestamp_prefix):])
                path2Json = os.path.join(path_to_json_folder, json_file)
               
                with open(path2Json,) as f:
                    annotation = json.load(f)    
                    for detection in annotation["detection_list"]:  
                        allIds.append(detection["id"])  
                        allages.append(age)

            UniqueIds = np.unique(allIds)   
            T = np.max(allages)
            ClassPerEmbryo = ["UNKNOWN"] * len(UniqueIds)
            SeverPerEmbryo = ["UNKNOWN"] * len(UniqueIds)
            for embryoId in UniqueIds:
                ClassPerEmbryo[embryoId-1] = ["UNKNOWN"] * T
                SeverPerEmbryo[embryoId-1] = ["UNKNOWN"] * T
                for json_file in os.listdir(path_to_json_folder):
                    age = re.findall(timestamp_prefix+ "\d+", json_file)
                    age = int(age[0][len(timestamp_prefix):])
                    path2Json = os.path.join(path_to_json_folder, json_file)
                    
                    with open(path2Json,) as f:
                        annotation = json.load(f)    
                        
                        for detection in annotation["detection_list"]:  
                            if detection["id"] == embryoId:
                                ClassPerEmbryo[embryoId-1][age-1] = detection["className"]
                                SeverPerEmbryo[embryoId-1][age-1] = detection["severe"]

            ClassEmbryo = ["UNKNOWN"] * len(UniqueIds)
            SeverEmbryo = ["UNKNOWN"] * len(UniqueIds)
            for embryoId in UniqueIds:
                allClass = []
                allSev = []
                for  t in range (0,T):
                    embryoClass = ClassPerEmbryo[embryoId-1][t]
                    if embryoClass not in (ignore_class_names + ["UNKNOWN"]):
                        allClass.append(embryoClass)
                        allSev.append(SeverPerEmbryo[embryoId-1][t])
                if len(np.unique(allClass)) > 0:       
                    ClassEmbryo[embryoId-1] = np.unique(allClass)[0]
                    SeverEmbryo[embryoId-1] = np.unique(allSev)[0] 
                else:
                     SeverEmbryo[embryoId-1] = 100

            # assign new
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
                        embryoId = detection["id"]
                        class_name = ClassEmbryo[embryoId-1]
                        severity = 1.0 * SeverEmbryo[embryoId-1]
                        if args.start == 0:
                            if detection not in ignore_class_names:
                                detection["className"] = class_name
                                detection["severe"] = severity
      
                        if (args.start > 0) and (args.start < age):
                            if detection["className"] not in ignore_class_names:
                                detection["className"] = class_name
                                detection["severe"] = severity
                            if detection["className"] in (ignore_class_names + ["UNKNOWN"]):
                                detection["severe"] = 100
                        else:
                            detection["severe"] = 100
                            if detection["className"] not in ignore_class_names:
                                detection["className"] = "UNKNOWN"

    
                    updated_annotation = annotation
                    updated_annotation["labeler"] = "semi_automatic_from_"+annotation["labeler"]                   
    
                with open(path_to_save_annotation_file,"w") as fanno:
                    json.dump(updated_annotation, fanno, indent=4)
                    


if __name__=="__main__":
    main()
