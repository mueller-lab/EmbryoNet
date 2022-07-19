
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

from  PlotScripts.PlotSigmoids.session_graph import PlotSessionGraph
import  argparse

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--path_to_experiment', type=str, required=True)


    args = parser.parse_args()

    plot_graph = PlotSessionGraph(args.path_to_experiment)
    plot_graph()
