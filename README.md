# EmbryoNet

## Introduction

This repository provides supplementary implementation materials to our paper "EmbryoNet: using deep learning to link embryonic phenotypes to signaling functions". 

During zebrafish development, seven major signaling pathways orchestrate the formation of the body plan: BMP, RA, Wnt, FGF and Nodal pattern the germ layers and regulate the formation of the orthogonal anterior-posterior and dorsal-ventral axes, whereas the elongation and morphogenesis of the body axis and later tissues is additionally shaped by the activity of Shh and PCP signaling.

<img src= "doc/phenotypes_features.png" />

EmbryoNet is a convolutional neural network, used in a pipeline that allows the automatic image classification of signaling-associated phenotypic changes. Derived from ResNet-18, EmbryoNet incorporates visual information and temporal data for the classification of phenotypes and can properly handle heterochronic effects. 

<img src= "doc/Neural_netrowrk_structure.png" />

The EmbryoNet classification pipeline is accurate, lightweight and easy to deploy.
This repository contains sources implementing EmbryoNet training and export code, together with additional C++ tools for dataset labeling and inference.
The materials are organized in folders <a href="https://github.com/hagorms/EmbryoNet/tree/main/GUI">GUI</a> and <a href="https://github.com/hagorms/EmbryoNet/tree/main/Train_Eval">Train_Eval</a> .

## <a href="https://github.com/hagorms/EmbryoNet/tree/main/GUI"><b>GUI</b></a> 

The GUI folder contains C++ tools to label and visualize datasets and to run EmbryoNet inference. 

There are two tools implemented with graphical user interface: 

* FishLabeler  - tool to visualize and manually label predetected embryos.  

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  <img src= "doc/Embryo_Labeler_RA.png" width="800" height="600" /> 

* FishClassifier  - tool to detect embryos and run the EmbryoNet inference pipeline.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   <img src= "doc/Embryo_Classifier.png" width="800" height="600"  />

In order to compile, please follow the tutorial.

## <a href="https://github.com/hagorms/EmbryoNet/tree/main/Train_Eval"><b>Train_Eval</b></a>

The Train_Eval folder contains python tools to train EmbryoNet and analyze inference results.
Folders with packages and tools to work with EmbryoNet database model converting, running analysis, training, etc:

* TrainEval/tools/Datasets: tools to work with datasets
* TrainEval/tools/Export: tools to export a trained model and to create ExperimentRunner configs
* TrainEval/tools/GenerateVideos: tools to generate Class Activation Maps and to create a collage video from them.
* TrainEval/tools/Run: tool to run embryo classification with python.

<img src= "doc/WNT_activation.png" /> 

## Thanks  

* https://github.com/KaimingHe/deep-residual-networks 
* https://github.com/zhoubolei/CAM
* https://github.com/nwojke/deep_sort
