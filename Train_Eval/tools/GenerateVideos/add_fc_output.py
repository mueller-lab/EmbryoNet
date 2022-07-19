
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import argparse
import onnx

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_model_path', type=str, required=True)
    parser.add_argument('-o', '--output_model_path', type=str, required=True)
    parser.add_argument('--intermediate_tensor_name', type=str, default='275')
    args = parser.parse_args()

    model = onnx.load(args.input_model_path)
    intermediate_tensor_name = args.intermediate_tensor_name
    intermediate_layer_value_info = onnx.helper.ValueInfoProto()
    intermediate_layer_value_info.name = intermediate_tensor_name
    model.graph.output.extend([intermediate_layer_value_info])
    onnx.save(model, args.output_model_path)

