
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import json
import itertools
import os

import matplotlib.pyplot as plt
import numpy as np

class ExperimentCollector():
    def __init__(self, pathes_to_experiment_folders,
                 labeler_names,
                 algorithm_name,
                 filters = [],
                 test_mode=False ):

        self.pathes_to_experiment_folders = pathes_to_experiment_folders
        self.labeler_names = labeler_names
        self.algorithm_result_name = algorithm_name
        self.filters = filters
        self.test_mode = test_mode
        self.plotWells = []
        self.algotightm_labels = []
        self.human_labels = []
        self.jsonDict = {}
        self.cm_size = 0


    def map_klass_severity(self, phoenotype, severity):
        if severity is None:
            severity = 25
        if 0.0 <= severity <= 35.0:
            return phoenotype + "_25"
        if 35.0 < severity <= 60.0:
            return phoenotype + "_50"
        if 60.0 < severity <= 80.1:
            return phoenotype + "_75"
        if 80.1 < severity <= 100.0:
            return phoenotype + "_100"

    def map_class_severity(self, phenotype, severity, with_severity=True, phenotype_with_severity = ["BMP","NODAL"]):
        if (with_severity):
            if phenotype in phenotype_with_severity:
                return self.map_klass_severity(phenotype, severity)
        return phenotype

    def map_class_list(self, class_list,
                       with_severity,
                       phenotype_with_severity = ["BMP","NODAL"],
                       severities=[100,75,50,25]):


        if not with_severity:
            return class_list

        new_class_list = []
        if with_severity:
            for phenotype_class in class_list:
                if phenotype_class in phenotype_with_severity:
                    for severity in severities:
                        new_class_list.append(phenotype_class+"_"+str(severity))
                else:
                    new_class_list.append(phenotype_class)

        return new_class_list


    def filter_well(self, well):
        for filter in self.filters:
            if filter in well:
                return self.test_mode
        return not self.test_mode

    def initiate_cm(self, with_severity, class_names=["BMP", "NODAL","NORMAL",
                                       "UNKNOWN", "BOOM",
                                       "WNT","FGF","SHH","PCP","RA"],  severe_classes = ["BMP", "NODAL"]):
        for class_name in class_names:
            self.human_labels.append(self.map_klass_severity(class_name,100) )
            self.algotightm_labels.append(self.map_klass_severity(class_name,100))
            self.cm_size +=1
        if with_severity:
            for severity in [25, 50, 75, 100]:
                for class_name in severe_classes:
                    self.human_labels.append(self.map_klass_severity(class_name,severity) )
                    self.algotightm_labels.append(self.map_klass_severity(class_name,severity))
                    self.cm_size += 1

    def collect_experimetns(self):
        for path_to_experoment_folder in self.pathes_to_experiment_folders:
            print("Collecting : ", path_to_experoment_folder)
            wells = os.listdir(path_to_experoment_folder)

            for well in wells:

                if not os.path.isdir(os.path.join(path_to_experoment_folder, well)):
                    continue

                if not self.filter_well(well):
                    continue
   
                path_to_well = os.path.join(path_to_experoment_folder, well)
                folders_in_well = os.listdir(path_to_well)

                labeler_found = False
                labeler_file_count = 0
                for labeler in self.labeler_names:
                    if labeler in folders_in_well:
                        labeler_found = True
                        labeler_file_count = len(os.listdir(os.path.join(path_to_well, labeler)))
                        break

                algorithm_result_found = False
                algorithm_file_count = 0

                if self.algorithm_result_name in folders_in_well:
                    algorithm_result_found = True
                    algorithm_file_count = len(os.listdir(os.path.join(path_to_well, self.algorithm_result_name)))

                if algorithm_result_found and labeler_found and (labeler_file_count == algorithm_file_count):
                    self.plotWells.append(path_to_well)
                #print("well: ", well, "-> labeler ", labeler_found, ": ", labeler_file_count, ", algorithm ", algorithm_result_found, ": ", algorithm_file_count)    
        
            print(len(self.plotWells), " wells collected")

    def collect_jsonFiles(self):
        for well in self.plotWells:
            for labeler_name in self.labeler_names:
                if labeler_name in os.listdir(well):
                    for json_file in sorted(os.listdir(os.path.join(well, labeler_name))):
                        json_full_path = os.path.join(well, labeler_name, json_file)                        
                        with open(json_full_path) as jf:
                            self.jsonDict[json_file] = [json.loads(jf.read())] 
                    break
                
            
            for json_file in sorted(os.listdir(os.path.join(well, self.algorithm_result_name))):
                json_full_path = os.path.join(well, self.algorithm_result_name, json_file)
                with open(json_full_path) as jf:
                     self.jsonDict[json_file].append(json.loads(jf.read()))
            
            print("well : ", well, " -> ", len(self.jsonDict)/720, " json collected")         


    def collect_detections(self, with_severity):
        #self.initiate_cm(with_severity)
        for json_file in self.jsonDict:
            jsonPair = self.jsonDict[json_file]
            detection_human = jsonPair[0]["detection_list"]
            detection_machine = jsonPair[1]["detection_list"]

            for detection in detection_human:
                if "severe" in detection:
                    human_detection_severiity = detection["severe"]
                else:
                    human_detection_severiity = 100
                human_detection_class = detection["className"]
                self.human_labels.append(
                    self.map_class_severity(human_detection_class,
                                            human_detection_severiity,
                                            with_severity))


            for i, detection in enumerate(detection_machine):
                if "severe" in detection:
                    machine_detection_severity = detection["severe"]
                else:
                    machine_detection_severity = 100

                machine_detection_class = detection["className"]
                if (detection["tlx"] == detection_human[i]["tlx"]) and (
                        detection["tly"] == detection_human[i]["tly"]):
                    self.algotightm_labels.append(
                        self.map_class_severity(machine_detection_class, machine_detection_severity,with_severity))
                else:
                    print("Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    print(json_file)
                    print (detection)
                    print("algorithm detection coordinates are not compatible with human detection coordinates."
                          "\n probably new version of algorithm is used. "
                          " \n Exiting")
                    #TODO: чтобы ругался но не выходил.
                    exit()


    def plot_confusion_matrix(self, cm, class_names):
        """
        Returns a matplotlib figure containing the plotted confusion matrix.

        Args:
           cm (array, shape = [n, n]): a confusion matrix of integer classes
           class_names (array, shape = [n]): String names of the integer classes
        """

        newMatrix = np.zeros(cm.shape)
        figure = plt.figure(figsize=(8, 8))
        plt.imshow(cm, interpolation='nearest', cmap=plt.cm.viridis)
        plt.title("Confusion matrix")
        plt.colorbar()
        tick_marks = np.arange(len(class_names))
        plt.xticks(tick_marks, class_names, rotation=45)
        plt.yticks(tick_marks, class_names)

        # Normalize the confusion matrix.
        cm = np.around(cm.astype('float') / cm.sum(axis=1)[:, np.newaxis], decimals=2)

        # Use white text if squares are dark; otherwise black.
        threshold = cm.max() / 2.

        for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
            color = "white" if cm[i, j] > threshold else "black"

            plt.text(j, i, cm[i, j], horizontalalignment="center", color=color)
            newMatrix[i][j] = cm[i, j]
            if np.isnan(newMatrix[i][j]):
                newMatrix[i,j]=0


        plt.tight_layout()
        plt.ylabel('Labeler 1 ')
        plt.xlabel('Labeler 2 ')
        return figure , newMatrix

    def get_csv_list(self, mat, class_names):

        assert mat.shape[0] == len(class_names)

        first_line = ["LABELS[gt,pred]"] # rows = gt, cols = predicted
        first_line = first_line + class_names
        csv_list = [first_line]
        for c_idx, class_name in enumerate(class_names):
            list_elem = [class_name]
            list_elem.extend(map(str, mat[c_idx].tolist()))
            csv_list.append(list_elem)

        return csv_list
