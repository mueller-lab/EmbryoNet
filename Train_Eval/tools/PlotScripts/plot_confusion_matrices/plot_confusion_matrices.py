
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import argparse
import shutil
import os
from PlotScripts.PlotConfusionMatrices.config_util import generate_config, config_path_to_list
from PlotScripts.PlotConfusionMatrices.plot_confusion_matrices import plot_confusion_matrices

if __name__=="__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--config", type=str, required=False,
                        help="path to confusion matrices json config file")
    parser.add_argument("-g", "--generate", action="store_true",
                        help="generate default config file ")
    parser.add_argument("-o", "--output_folder", type=str, required=True,
                        help= "path to result folder")
    parser.add_argument("-d", "--disable_numbers", action="store_true",
                        help = "hides numbers on a generated confusion matrix")
    parser.add_argument("-n", "--name", type=str, default= "",
                        help="output name for confusion matrixes files")
    parser.add_argument("-m", "--make_dir", action = "store_true",
                        help = "make output directory")


    args = parser.parse_args()
    output_folder = os.path.join(args.output_folder, args.name)
    if (args.make_dir):
        output_folder = os.path.join(args.output_folder, args.name)
        if (os.path.exists(output_folder)):
            print("folder '{}' already exists".format(output_folder))
        else:
            os.mkdir(output_folder)

    if args.generate:
        generate_config(args.output_folder, args.name)
    else:
        plot_confusion_matrices(output_folder, args.config,
                                args.disable_numbers, args.name,
                                *config_path_to_list(args.config))
        path_to_save_config = os.path.join(output_folder, os.path.basename(args.config))
        shutil.copy(args.config,path_to_save_config)

