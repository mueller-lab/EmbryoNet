
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

from .experiment_graph import PlotExperimentGraph

import os

class SessionStatistics:

    def __init__(self):
        self.class2count = {}

    def collect(self, class2count, labeler_name):
        if labeler_name not in self.class2count:
            self.class2count[labeler_name] = class2count
        else:
            for class_key in self.class2count[labeler_name]:
                assert len(self.class2count[labeler_name][class_key]) == len(class2count[class_key]), \
                        (labeler_name, class_key)

                for idx in range(len(self.class2count[labeler_name][class_key])):
                    self.class2count[labeler_name][class_key][idx] += class2count[class_key][idx]


class PlotSessionGraph:

    def __init__(self, target_path, prefix=""):
        self.prefix = prefix
        if isinstance(target_path, list):
            self.experiment_folder_list = target_path
            for folder in self.experiment_folder_list:
                if not os.path.isdir(folder):
                    raise Exception('Element {} is not folder'.format(folder))
        else:
            if os.path.isdir(target_path):
                self.experiment_folder_list = [os.path.join(target_path, x) for x in os.listdir(target_path)]
            else:
                raise Exception('Unsupported target_path type {}'.format(target_path))

        self.session_statistics = SessionStatistics()

    def __call__(self):

        for experiment_folder in self.experiment_folder_list:

            if not os.path.isdir(experiment_folder):
                continue

            labelers = [folder for folder in os.listdir(experiment_folder)
                        if folder.find('json') > 0 and os.path.isdir(os.path.join(experiment_folder, folder))]
            for labeler in labelers:
                plotter = PlotExperimentGraph(experiment_folder, labeler)
                class2count, labeler = plotter()
                self.session_statistics.collect(class2count, labeler)

        for labeler_name in self.session_statistics.class2count.keys():
            for experiment_folder  in self.experiment_folder_list:
                parentPath = os.path.join(experiment_folder, os.pardir)

                plotter.plotGraph(self.session_statistics.class2count[labeler_name], parentPath, self.prefix+labeler_name)


