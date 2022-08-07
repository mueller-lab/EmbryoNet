#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller,
# Computer Vision Studio and Friedrich Miescher Laboratory of the Max Planck Society
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
    parser.add_argument('--ignore_class', "-ic",  nargs="+", default=["BOOM", "CUT"])
    parser.add_argument('--timestamp_prefix', "-tp", type = str, default= "LO")
    args = parser.parse_args()
    ignore_class_names = [ignore_class.upper() for ignore_class in args.ignore_class]
    timestamp_prefix = args.timestamp_prefix
    path_to_session = args.path_to_session
    labeler_name = args.labeler_name
    
    experiments = [f for f in os.listdir(path_to_session) if os.path.isdir(os.path.join(path_to_session, f))]

    for experiment in experiments:
        path_to_experiment = os.path.join(path_to_session, experiment)
        path_to_json_folder = os.path.join(path_to_experiment, labeler_name)

        if os.path.isdir(path_to_experiment) and os.path.exists(path_to_json_folder):
            #print("Experiment : ", experiment)
            #allClasses = []
            #allSev = []
            allIds = []
            allages = [] 
    
            for json_file in os.listdir(path_to_json_folder):
                age = re.findall(timestamp_prefix+ "\d+", json_file)
                age = int(age[0][len(timestamp_prefix):])
                path2Json = os.path.join(path_to_json_folder, json_file)

               
                with open(path2Json,) as f:
                    annotation = json.load(f)   
                    #print("file : ", path2Json)
                    for detection in annotation["detection_list"]:  
                        #print(detection)
                        #if detection["className"] not in (ignore_class_names + ["UNKNOWN"]):
                        #allClasses.append(detection["className"]) 
                        #allSev.append(detection["severe"])   
                        allIds.append(detection["id"])  
                        allages.append(age)
    
            #allClasses = np.unique(allClasses)
            #allSev = np.unique(allSev)
            #allIds = np.unique(allIds)            
            #print("Experiment well: ", experiment, " -> ", allClasses, " , ", allSev, ' , ',allIds)    
            UniqueIds = np.unique(allIds)   
            T = np.max(allages)
            #print("Experiment : ", experiment, " -> ", np.max(allages), ", ", np.min(allages), ", T : ", T)
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
                            currSev = detection.get("severe")
                            
                            if not currSev:
                                detection["severe"] = 100
                            
                            if detection["className"] in (ignore_class_names + ["UNKNOWN"]):
                                detection["severe"] = 100
                           # print(detection)    
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

            for embryoId in UniqueIds:
                print("Experiment well: ", experiment, " -> ","embryo ", embryoId, " : ",  ClassEmbryo[embryoId-1], " , ", SeverEmbryo[embryoId-1])

                        
                
if __name__=="__main__":
    main()