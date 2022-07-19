# EmbryoNet

## Introduction

This is repository provides supplementary implementation materials to our paper [link to arxiv or paper]. 

During zebrafish development, seven major signaling pathways orchestrate the formation of the body plan: BMP, RA, Wnt, FGF and Nodal pattern the germ layers and regulate the formation of the orthogonal anterior-posterior and dorsal-ventral axes, whereas the elongation and morphogenesis of the body axis and later tissues is additionally shaped by the activity of Shh and PCP signaling.

<img src= "doc/phenotypes_features.png" />

EmbryoNet is a convolutional neural network, used in a pipeline that allows the automatic image classification of phenotypic changes upon treatment with a panel of compounds. Derived from ResNet-18, EmbryoNet additionally to visual information incorporates temporal data to perform classification task properly handling heterochronic effects from inhibition of some pathways. 

<img src= "doc/Neural_netrowrk_structure.png" />

The deveolped EmbryoNet-based phenotypic changes classification pipeline is accurate, lightweight and easy to deploy.
This repository contains sources implementing EmbryoNet training and export code, together with additional C++ tools for dataset labeling and inference.
The materials presented are organized in folders <a href="https://github.com/hagorms/EmbryoNet/tree/main/GUI">GUI</a> and <a href="https://github.com/hagorms/EmbryoNet/tree/main/Train_Eval">Train_Eval</a> .

## <a href="https://github.com/hagorms/EmbryoNet/tree/main/GUI"><b>GUI</b></a> 

GUI folder contains C++ tools to label and visualize datasets and to run EmbryoNet inference. 

There are two tools implemented with graphical user interface: 

* FishLabeler  - tool to visualize and manually label predetected embryos.  

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  <img src= "doc/Embryo_Labeler_RA.png" width="800" height="600" /> 

* FishClassifier  - tool to detect embryos and run EmbryoNet inference pipeline.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   <img src= "doc/Embryo_Classifier.png" width="800" height="600"  />

In order to compile follow the tutorial : [TBI]

## <a href="https://github.com/hagorms/EmbryoNet/tree/main/Train_Eval"><b>Train_Eval</b></a>

Train_Eval folder contains python tools to train EmbryoNet and analyse inference results.
Folder that has packages and tools to work with EmbryoNet Databases model converting, running analysis, training, etc:

* TrainEval/tools/Datasets: tools to work with datasets
* TrainEval/tools/Export: tools to export trained model and to create ExperimentRunner configs
* TrainEval/tools/GenerateVideos: tools to generate Class Activation Maps and to create collage video from them.
* TrainEval/tools/Run: tool to run embryo classification with python.

<img src= "doc/WNT_activation.png" /> 

## Thanks  

* https://github.com/KaimingHe/deep-residual-networks 
* https://github.com/zhoubolei/CAM
* https://github.com/nwojke/deep_sort
