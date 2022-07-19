
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================
import matplotlib.pyplot as plt
import json
import numpy as np
import os
import csv

class2color = {"BMP": 'blue', "NODAL": 'red', "NORMAL": "lime", "UNKNOWN": 'black', "BOOM": 'fuchsia',
               "WNT": "yellow", "FGF": 'cyan', "SHH": 'mediumslateblue', "PCP": "darkorange",
               "RA": 'darkmagenta',"CUT":'gray'}

class PlotExperimentGraph:

    def __init__(self, path_to_experiment, labeler_name):
        self.path_to_experiment = path_to_experiment
        self.labeler_name = labeler_name

    def __call__(self):
        legend_magic_number = 1.4
        path = self.path_to_experiment
        labeler = self.labeler_name
        pathToJsons = os.path.join(path, labeler)
        annotation_jsons = os.listdir(pathToJsons)
        num_frames = int(len(annotation_jsons) * legend_magic_number)

        class2count = {
            "BMP": [0] * num_frames,
            "NODAL": [0] * num_frames,
            "NORMAL": [0] * num_frames,
            "UNKNOWN": [0] * num_frames,
            "BOOM": [0] * num_frames,
            "WNT": [0] * num_frames,
            "FGF": [0] * num_frames,
            "SHH": [0] * num_frames,
            "PCP": [0] * num_frames,
            "RA": [0] * num_frames,
            "CUT": [0] * num_frames,
        }

        for frame_idx, annotation_json in enumerate(sorted(annotation_jsons)):
            path2Json = os.path.join(pathToJsons, annotation_json)
            with open(path2Json) as f:
                annotation = json.load(f)
                for detection in annotation["detection_list"]:
                    class2count[detection["className"]][frame_idx] += 1

        self.plotGraph(class2count, path, labeler)

        return class2count, labeler


    def plotGraph(self, class2count, path, labeler):

        num_frames = len(class2count[list(class2count.keys())[0]])
        embryoSum = [0] * num_frames
        for i in range(num_frames):
            for className in class2count.keys():
                embryoSum[i] += class2count[className][i]
        max_embryo_count = max (embryoSum)
        plt.ylim(ymax=max_embryo_count + 5)
        plt.ylim(ymin=0)

        x = np.linspace(0, num_frames, num_frames)

        for embryo_class in class2count:
            plt.plot(x, class2count[embryo_class], '.', color=class2color[embryo_class], label=embryo_class)
        plt.legend(loc="upper right", borderaxespad=0.5)

        pathToSave = os.path.join(path, labeler + ".svg")
        pathToSave_csv = os.path.join(path, labeler + ".csv")
        with open(pathToSave_csv, "w", newline='') as fp:
            writer = csv.writer(fp, delimiter=",")
            for key, value in class2count.items():
                writer.writerow([key, value])
        plt.savefig(pathToSave)

        plt.clf()
        plt.cla()
        plt.close()

