
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

from PlotScripts.PlotSigmoids.session_graph import PlotSessionGraph

import argparse

import pandas as pd

if __name__ =="__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-x", "--path_to_excel", type=str, required=True)
    args = parser.parse_args()

    sheet_names = pd.ExcelFile(args.path_to_excel).sheet_names

    for sheet_name in sheet_names:
        curr_sheet = pd.read_excel(args.path_to_excel, sheet_name)

        train_paths = []
        test_paths = []
        no_train_no_test_paths = []
        for indx, path_to_experiment in enumerate(curr_sheet.path_to_labeled_data):
            if bool(int(curr_sheet.Train[indx])):
                train_paths.append(curr_sheet.path_to_labeled_data[indx])
            if bool(int(curr_sheet.Test[indx])):
                test_paths.append(curr_sheet.path_to_labeled_data[indx])
            if not bool(int(curr_sheet.Train[indx])) and not bool(int(curr_sheet.Test[indx])):
                no_train_no_test_paths.append(curr_sheet.path_to_labeled_data[indx])

            if train_paths:
                PlotSessionGraph(train_paths, "train_" )()
            if test_paths:
                PlotSessionGraph(test_paths, "test_")()
            if no_train_no_test_paths:
                PlotSessionGraph(no_train_no_test_paths, "no_train_no_test_")()
