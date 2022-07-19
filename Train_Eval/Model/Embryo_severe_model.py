#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller,
# Computer Vision Studio and Friedrich Miescher Laboratory of the Max Planck Society
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import torch
import torch.nn as nn

import torch.nn.functional as F

class ResidualModule(nn.Module):

    def __init__(self, n_channels, multiplicators, downSample = False):

        super(ResidualModule, self).__init__()
        self.runShortBlock = True

        multiplicator_1_1 = multiplicators[11]
        multiplicator_1_3 = multiplicators[13]
        multiplicator_2_1 = multiplicators[21]

        outputChannels_1_1 = int(n_channels * multiplicator_1_1)
        outputChannels_1_3 = int(outputChannels_1_1 * multiplicator_1_3)
        outputChannels_2_1 = int(n_channels * multiplicator_2_1)


        self.conv_1_1 = nn.Conv2d(n_channels, outputChannels_1_1, kernel_size = 1, stride = 1, padding= 0, bias = False )
        self.bn_1_1 = nn.BatchNorm2d(outputChannels_1_1)

        self.conv_1_2 = nn.Conv2d(outputChannels_1_1, outputChannels_1_1, kernel_size=3, stride=(2 if downSample else 1) , padding=1, bias=False)
        self.bn_1_2 = nn.BatchNorm2d(outputChannels_1_1)

        self.conv_1_3 = nn.Conv2d(outputChannels_1_1, outputChannels_1_3, kernel_size=1, stride=1, padding=0, bias=False)
        self.bn_1_3 = nn.BatchNorm2d(outputChannels_1_3)

        self.runShortBlock = (outputChannels_2_1 != 0)
        if self.runShortBlock:
            self.conv_2_1 = nn.Conv2d( n_channels, outputChannels_2_1, kernel_size=1, stride=(2 if downSample else 1), padding=0, bias=False)
            self.bn_2_1 = nn.BatchNorm2d(outputChannels_2_1)

    def forward(self, x):
        x1 = self.conv_1_1(x)
        x1 = self.bn_1_1(x1)
        x1 = F.relu(x1)
        x1 = self.conv_1_2(x1)
        x1 = self.bn_1_2 (x1)
        x1 = F.relu(x1)
        x1 = self.conv_1_3(x1)
        x1 = self.bn_1_3(x1)
        x2 = x

        if self.runShortBlock:
            x2 = self.conv_2_1(x)
            x2 = self.bn_2_1(x2)

        return F.relu(x2 + x1)

class TailModule(nn.Module):

    def __init__(self, n_channels, n_outputs):
        super(TailModule, self).__init__()
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc1 = nn.Linear(n_channels + 1, n_outputs)

    def forward(self, x, t):
        x = self.avgpool(x)
        x = x.view(-1, x.shape[1])
        x = torch.cat((x, t), dim=1)
        x = self.fc1(x)
        output = x
        return output

class EmbryoNet2(nn.Module):

    """
    EmbryoNet model description.
    Model is used in EmbryoExperimentRunner and in run_model.py.

    Architecture is similar to ResNet18 with extra timestamp input:
    1) added time to first convolution as 4-th input channel
    2) added time to fully-connected layer

    """

    def __init__(self, num_classes, final_softmax=False, prepare_time=True):

        """
          Args:
              num_classes (int): number of embryo classes
              final_softmax (bool): add softmax classification, True in evaluation, False in training
              prepare_time (bool): prepare time channels t_for_concat and t_for_fc
          """

        super(EmbryoNet2, self).__init__()
        self.final_softmax = final_softmax
        self.prepare_time = prepare_time
        self.base_conv = nn.Conv2d(3 + 1, 64, kernel_size=7, stride=2, padding=3, bias=False)
        self.base_bn = nn.BatchNorm2d(64)
        self.M1 = ResidualModule(64, {11:1, 13:4, 21:4})
        self.M2 = ResidualModule(256, {11: 0.25, 13: 4, 21:0})
        self.M3 = ResidualModule(256, {11: 0.5, 13:4, 21:2}, True)
        self.M4 = ResidualModule(512, {11:0.25 ,13:4,21:0})
        self.M5 = ResidualModule(512, {11:0.5 ,13:4,21:2}, True)
        self.M6 = ResidualModule(1024,{11:0.25 ,13:4,21:0})
        self.M7 = ResidualModule(1024, {11:0.5, 13:4, 21:2 }, True)
        self.M8 = ResidualModule(2048,{11:0.25 ,13:4,21:0})
        self.tail_prob = TailModule(2048, num_classes)


    def forward(self, x, t):
        """
            Args:
                x (image): input detection image (embryo)
                t (float): timestamp 0 = relative experiment start, 1 - relative experiment end.
                returns logits if train, probabilities if evaluation.
            """
        x = x / 255.0
        x = x - 0.5

        if self.prepare_time:
            t = t.to(dtype=torch.float32)

            t_for_conv = t.view(size=(t.shape[0], 1, 1, 1)).repeat([1, 1, 224, 224])
            t_for_fc = t.view(size=(t.shape[0], 1))
        else:
            t_for_conv, t_for_fc = t

        x = torch.cat((x, t_for_conv), dim=1)
        x = self.base_conv(x)
        x = self.base_bn(x)
        x = F.relu(x)
        x = F.max_pool2d(x, kernel_size=3, padding=1, stride=2)
        x = self.M1(x)
        x = self.M2(x)
        x = self.M3(x)
        x = self.M4(x)
        x = self.M5(x)
        x = self.M6(x)
        x = self.M7(x)
        x = self.M8(x)
        x_prob = self.tail_prob(x, t_for_fc)

        if self.final_softmax:
            x_prob = F.softmax(x_prob, dim=-1)
            x_prob = x_prob.view(-1)

        return x_prob

def get_model(state_dict_path=None, num_classes=17, final_softmax=False):
    """
    Builds and returns a model from given state_dict_path and final_softmax.
    Args:
         state_dict_path (str): path to saved pytorch checkpoint file
         num_classes (int): number of output classes
         final_softmax False if training, True if evaluation.
    """
    EmbryoNet2_model = EmbryoNet2(num_classes=num_classes, final_softmax=final_softmax)
    if state_dict_path:
        EmbryoNet2_model.load_state_dict(torch.load(state_dict_path), strict=False)
    return EmbryoNet2_model



