
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import os
import argparse

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('--path_to_phenotype', '-p', type=str)
    parser.add_argument('--labeler_name', '-l', type=str)
    parser.add_argument('--path_to_output_folder', '-o', type=str)
    parser.add_argument('--timestamp_format', "-t", default="LO", help = "timestamp format. Default  is LO\d+")
    args = parser.parse_args()

    labeler_name = args.labeler_name
    path_to_phenotype = args.path_to_phenotype
    path_to_output_folder = args.path_to_output_folder
    timestamp_format = args.timestamp_format

    # One phenotype has several sessions, recordered in different days/microscopes etc.
    # Each session has several experiments (wells) with microscope images.
    sessions = os.listdir(path_to_phenotype)
    for session in sessions:
        print(session)
        path_to_session = os.path.join(path_to_phenotype,session)
        if not os.path.isdir(path_to_session):
            continue
        experiments = os.listdir(path_to_session)
        for experiment in experiments:
            print(experiment)
            full_experiment_path = os.path.join(path_to_session, experiment)
            if not os.path.isdir(full_experiment_path):
                continue
            folders = os.listdir(full_experiment_path)

            if labeler_name not in folders:
                continue

            path_to_images = os.path.join(path_to_session, experiment, "images")
            path_to_jsons = os.path.join(path_to_session, experiment, labeler_name)
            path_to_output = os.path.join(path_to_output_folder, session + "_" + experiment + ".json")
            os.system('python parse_experiment_dataset.py --path_to_images "' + path_to_images +
                      '" --path_to_jsons "' + path_to_jsons +
                      '" -o "' + path_to_output + '"' +
                      " --timestamp_format " + timestamp_format )

            print('python parse_experiment_dataset.py '
                   '--path_to_images "' + path_to_images +
                   '" --path_to_jsons "' + path_to_jsons +
                   '" -o "' + path_to_output + '"' +
                   " --timestamp_format " + args.timestamp_format)

if __name__ == '__main__':
    main()
