
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import torch
import argparse
import os
from Model.Embryo_severe_model import EmbryoNet2


class ModelWrapper(torch.nn.Module):

    def __init__(self, base_module, transpose=False, mean_std=False):

        super(ModelWrapper, self).__init__()
        self.base_module = base_module
        self.transpose = transpose
        self.mean_std = mean_std

    def forward(self, x, t_for_conv, t_for_fc):

        if self.transpose:
            x = x.permute(0, 3, 1, 2)

        if self.mean_std:
            x = x / 255.0
            x = x - 0.5

        return self.base_module(x, (t_for_conv, t_for_fc))


def run_export(path_to_model,
               path_to_output,
               number_of_classes,
               batch_size):

    model_name = os.path.basename(path_to_model).split(".")[0]
    newModel = EmbryoNet2(number_of_classes, True, prepare_time = False)
    newModel.load_state_dict(torch.load(path_to_model))
    newModel = ModelWrapper(newModel, transpose=False, mean_std=False)
    newModel.eval()

    dummy_input = torch.randn(batch_size, 3, 224, 224, device='cpu')

    dummy_t = torch.tensor(0, device='cpu', dtype=torch.float32).view(-1).repeat([batch_size])

    t_for_conv = dummy_t.view(size=(dummy_t.shape[0], 1, 1, 1)).repeat([1, 1, 224, 224])
    t_for_fc = dummy_t.view(size=(dummy_t.shape[0], 1))

    input_names = ["data", "t_for_conv", "t_for_fc"]
    output_names = ["output"]
    batch_str = str(batch_size)
    path_to_output_model =  os.path.join(path_to_output,
                      model_name +
                      '_batch_' + batch_str +'.onnx')
    torch.onnx.export(newModel, (dummy_input, t_for_conv, t_for_fc), path_to_output_model,
                      verbose=True, input_names=input_names, output_names=output_names)


if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument('--path_to_model', '-m', type=str, required=True)
    parser.add_argument('--path_to_output', '-o', type=str, required=True)
    parser.add_argument('--batch_size', '-b', type=int, required=True)
    parser.add_argument('--number_of_classes',"-n", type=int, required=True)

    args = parser.parse_args()
    path_to_model = args.path_to_model
    path_to_output = args.path_to_output
    batch_size = args.batch_size
    number_of_classes = args.number_of_classes

    run_export(path_to_model, path_to_output, number_of_classes,  batch_size)
