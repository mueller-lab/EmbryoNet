#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller,
# Computer Vision Studio and Friedrich Miescher Laboratory of the Max Planck Society
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

'''
License:
The pre-trained models and the CAM technique are released for unrestricted use.
Contact Bolei Zhou if  you have questions.
bzhou@ie.cuhk.edu.hk
'''

#this code was taken from
#https://github.com/zhoubolei/CAM
#reference @inproceedings{zhou2016cvpr,
#    author    = {Zhou, Bolei and Khosla, Aditya and Lapedriza, Agata and Oliva, Aude and Torralba, Antonio},
#    title     = {Learning Deep Features for Discriminative Localization},
#    booktitle = {Computer Vision and Pattern Recognition},
#    year      = {2016}
#}

import argparse

import onnx
import onnxruntime as rt

import cv2
import numpy as np
import os
import shutil

def returnCAM(feature_conv, weight_softmax, class_idx, minVal, maxVal):
    # generate the class activation maps upsample to 256x256
    size_upsample = (256, 256)
    bz, nc, h, w = feature_conv.shape
    cam = weight_softmax[class_idx].dot(feature_conv.reshape((nc, h*w)))
    cam = cam.reshape(h, w)

    limit = 2.0*np.max([np.abs(minVal), np.abs(maxVal)])
    cam_img = cam / limit
#    cam_img = cam_img * prob
    cam_img = cam_img + 0.5
    
    cam_img = np.uint8(255 * cam_img)
    output_cam = cv2.resize(cam_img, size_upsample)
#    print("Scaling Activation maps to : ", np.min(output_cam), ", ", np.max(output_cam))
    return output_cam
 
def getRangeVals(feature_conv, weight_softmax, pred):

    minVals = []
    maxVals = []
    bz, nc, h, w = feature_conv.shape    
    for class_idx in range(0, len(pred)):
        cam = weight_softmax[class_idx].dot(feature_conv.reshape((nc, h*w)))
        cam = cam.reshape(h, w)
        minVals.append(np.min(cam))
        maxVals.append(np.max(cam))

    minV = np.min(minVals)
    maxV = np.max(maxVals)
    
    #print("Range Activation maps : ", minV, ", ", maxV)
    
    return [minV, maxV]

def getCAMslimits(input_blob, model_path, t, input_size, newshape, fc_weight):

    sess = rt.InferenceSession(model_path)
    input_names = [input.name for input in sess.get_inputs()]
    output_names = [output.name for output in sess.get_outputs()]

    t_for_conv = t * np.ones(shape=((1, 1,) + input_size), dtype=np.float32)
    t_for_fc = t * np.ones(shape=(1, 1), dtype=np.float32)
    pred, feature_conv = sess.run(output_names, {input_names[0]: input_blob, input_names[1]: t_for_conv, input_names[2]: t_for_fc})

    model = onnx.load(model_path)
    weight_softmax = np.frombuffer([w for w in model.graph.initializer if w.name == fc_weight][0].raw_data, dtype=np.float32)
    weight_softmax = np.reshape(weight_softmax, newshape=newshape)
    weight_softmax = weight_softmax[:, :-1]

    vals = getRangeVals(feature_conv, weight_softmax, pred)
    
    max_prob = np.max(pred)
    pred_class = int(np.argmax(pred))

    return [vals, pred_class, max_prob]

def img_to_blob(img):
    blob = np.transpose(img, axes=(2, 0, 1))
    blob = np.float32(blob)
    blob.shape = (1,) + blob.shape
    return blob


def read_img(img_path, size):
    img = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img = cv2.resize(img, size)
    return img


def calculate_CAM(input_blob, model_path, t, input_size, newshape, fc_weight, visualize_class, vals):

    sess = rt.InferenceSession(model_path)
    input_names = [input.name for input in sess.get_inputs()]
    output_names = [output.name for output in sess.get_outputs()]

    t_for_conv = t * np.ones(shape=((1, 1,) + input_size), dtype=np.float32)
    t_for_fc = t * np.ones(shape=(1, 1), dtype=np.float32)
    pred, feature_conv = sess.run(output_names, {input_names[0]: input_blob, input_names[1]: t_for_conv, input_names[2]: t_for_fc})

    model = onnx.load(model_path)
    weight_softmax = np.frombuffer([w for w in model.graph.initializer if w.name == fc_weight][0].raw_data, dtype=np.float32)
    weight_softmax = np.reshape(weight_softmax, newshape=newshape)
    weight_softmax = weight_softmax[:, :-1]

#    vals = getRangeVals(feature_conv, weight_softmax, pred)

    if (visualize_class == -1):
        class_id = int(np.argmax(pred))
    elif (visualize_class < len(pred)):
        class_id = visualize_class
    else:
        raise Exception ("Wrong class. Please check parameters")


    CAM = returnCAM(feature_conv, weight_softmax, class_id, vals[0], vals[1])

#    print(f'probs={pred}')
#    print(f'max_prob={max_prob}')
#    print(f'class_id={class_id}')

    return [CAM, pred[class_id]]


def save_CAM(img_path, CAM, save_path):
    img = cv2.imread(img_path)
    height, width, _ = img.shape
    heatmap = cv2.applyColorMap(cv2.resize(CAM,(width, height)), cv2.COLORMAP_JET)
    result = heatmap * 0.3 + img * 0.5
    cv2.imwrite(save_path, result)
    
def create_empty_folder(folder):

    if os.path.exists(folder):
        shutil.rmtree(folder)
    os.makedirs(folder)

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--time', type=float, default=0.8)
    parser.add_argument('--model_path',"-mp", type=str, required=True)
    parser.add_argument('-wsh', type=int, default=17)
    parser.add_argument('-wsw', type=int, default=2049)
    parser.add_argument('--fc_weight', type=str, default='base_module.tail_prob.fc1.weight')
    parser.add_argument('-W', type=int, default=224)
    parser.add_argument('-H', type=int, default=224)
    parser.add_argument('--folder',"-f", type=str, required=True)
    parser.add_argument('--save_path', '-sp', type=str, required=True)

    args = parser.parse_args()

    input_size = args.H, args.W
    pathes_with_image_folders = os.listdir(args.folder)

    create_empty_folder(args.save_path)

    for path_to_image_folder in pathes_with_image_folders:

        full_path_to_image_folder = os.path.join(args.folder, path_to_image_folder)
        full_save_path = os.path.join(args.save_path, path_to_image_folder)

        create_empty_folder(full_save_path)

        images = sorted(os.listdir(os.path.join(args.folder, full_path_to_image_folder)))

        for index, image in enumerate(images):

            input_blob = img_to_blob(read_img(os.path.join(full_path_to_image_folder, image), input_size))
            time = float(index) / len(images)

            CAM = calculate_CAM(input_blob, args.model_path, time, input_size, (args.wsh, args.wsw), args.fc_weight)
            save_CAM(os.path.join(full_path_to_image_folder, image), CAM, os.path.join(full_save_path, image))
