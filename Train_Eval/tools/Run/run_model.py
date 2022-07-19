
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================
DEBUG = True
if DEBUG :
    from Model import Embryo_severe_model_fm as Embryo_severe_model
else :
    from Model import Embryo_severe_model

import argparse
import copy
from collections import defaultdict
import json
import shutil
import os
import re

import onnxruntime

import cv2
import numpy as np
import torch


class Detection:

    def __init__(self, x1, y1, x2, y2, conf):
        self.x1, self.y1, self.x2, self.y2 = x1, y1, x2, y2
        self.conf = conf
        self.probs = []
        self.old_probs = []
        self.class_name = "UNKNOWN"
        self.class_id = 0


def get_classes_from_config(path_to_config):
    with open(path_to_config) as f:
        config = json.load(f)
        classes = config["ResultRepresenter"][0]["classes"]
    return classes

def generate_class_name_severe_maps(path_to_config):

    with open(path_to_config) as f:
        cfg = json.load(f)

    classes_cfg = get_classes_from_config(path_to_config)
    prob_to_id = {}
    id_to_name = {}
    prob_to_severe = {}
    id_to_prob_group = defaultdict(list)

    prob_idx = 0
    for class_idx, class_cfg in enumerate(classes_cfg):
        severities = class_cfg.get("severities", [100])
        assert class_cfg["id"] == class_idx
        id_to_name[class_idx] = class_cfg["name"]
        for severity in severities:
            prob_to_id[prob_idx] = class_idx
            prob_to_severe[prob_idx] = severity
            id_to_prob_group[class_idx].append([prob_idx, severity])
            prob_idx += 1

    return prob_to_id, id_to_name, prob_to_severe, id_to_prob_group


def compute_average_score(res, batch_size = 8):
    assert len(res) % batch_size == 0
    num_classes = len(res) // batch_size
    results = copy.deepcopy(res[:num_classes])
    for i in range (1, batch_size):
        for j in range(num_classes):
            results[j] += res[i * num_classes + j]
    for j in range(len(results)):
        results[j] /= batch_size
    return results

def dump_json_results(json_base_name, detections, path_config, output_json_name, image_extension):

    prob_to_id, id_to_name, prob_to_severe, id_to_prob_group = generate_class_name_severe_maps(path_config)
    json_keys_vals = {}
    json_keys_vals["detection_list"] = []
    detection_list = []
    for id, detection in enumerate(detections):
        json_detection_field = {}
        json_detection_field["tlx"] = detection.x1
        json_detection_field["tly"] = detection.y1
        json_detection_field["brx"] = detection.x2
        json_detection_field["bry"] = detection.y2
        json_detection_field["id"] = id
        json_detection_field["confidence"] = True
        json_detection_field["oldProbs"] = [float(p) for p in detection.conf.tolist()]

        prob = [0] * len(id_to_prob_group)
        for prob_id, p in enumerate(detection.conf):
            class_idx = prob_to_id[prob_id]
            if prob[class_idx] < p:
                prob[class_idx] = p

        max_prob_idx = int(np.argmax(np.asarray(detection.conf)))

        for _, prob_idx_and_severity_lst in id_to_prob_group.items():
            for (prob_idx, severity) in prob_idx_and_severity_lst:
                if prob_idx == max_prob_idx:
                    severe = severity
                    break

        class_id = prob_to_id[max_prob_idx]
        class_name = id_to_name[class_id]

        json_detection_field["prob"] = [float(p) for p in prob]
        json_detection_field["severe"] = severe
        json_detection_field["className"] = class_name
        json_detection_field["classId"] = class_id

        detection_list.append(json_detection_field)

    json_keys_vals["detection_list"] = detection_list
    json_keys_vals["classes"] = get_classes_from_config(path_config)
    json_keys_vals["labeler"] = "python"

    image_name = json_base_name.split(".")[0] + "." + image_extension
    json_keys_vals["source_name"] = image_name

    with open(output_json_name, "w" ) as json_file:
        json.dump(json_keys_vals, json_file, indent=4)


def save_output(path_to_image, path_to_output, detections, path_to_cofig, model_type = "onnx"):

    path_to_output_images = os.path.join(path_to_output, "images")

    path_to_json = os.path.join(path_to_output, "python_result_" + model_type + "_json")

    os.makedirs(path_to_output_images, exist_ok=True)
    os.makedirs(path_to_json, exist_ok=True)

    copy_file_name = os.path.join(path_to_output, "images", os.path.basename(path_to_image))
    ends_with = path_to_image.split(".")[-1]
    if path_to_image != copy_file_name:
        shutil.copy(path_to_image, copy_file_name)

    json_base_name = os.path.basename(path_to_image).split(".")[0] + ".json"
    save_json_name = os.path.join(path_to_json ,json_base_name)

    dump_json_results (json_base_name, detections, path_to_cofig, save_json_name, ends_with )


def prepare_image(image, batchsize=8):
    images = [image]
    if batchsize not in [1,8]:
        raise Exception("not supported batch size. Supported batch sizes: 8 and 1.")
    if batchsize==8:
        FV = cv2.flip(image, 0)
        FH = cv2.flip(image,1)
        flipped_all = cv2.flip(image,-1)
        RR = cv2.rotate(image, cv2.ROTATE_90_CLOCKWISE)
        RL = cv2.rotate(image, cv2.ROTATE_90_COUNTERCLOCKWISE)
        RL_FH = cv2.flip (RL, 1)
        FH_RL = cv2.rotate(FH, cv2.ROTATE_90_COUNTERCLOCKWISE)
        images += [FV, FH, flipped_all, RR, RL, RL_FH, FH_RL]

    for image_idx, image in enumerate(images):

        curr_image = image

        curr_image = cv2.resize(curr_image, (224, 224))
        curr_image = np.float32(curr_image)
        curr_image = np.transpose(curr_image, axes =(2,0,1))
        curr_image.shape = (1,) + curr_image.shape
        images[image_idx] = curr_image

    return images


def get_default_model_path(path_to_config):
    with open(args.config) as f:
        config = json.load(f)
        net_name = config["Algorithm"]["classifier"]["path_to_net"][3:]
        num_classes = config["Algorithm"]["classifier"]["outputClasses"]
        path_to_net = os.path.join(os.path.dirname(path_to_config), net_name)
        return path_to_net, num_classes

def get_hough_params(path_to_config):

    with open(args.config) as f:

        config = json.load(f)
        algorithm = config["Algorithm"]
        detector = algorithm["tracker"]["detector"]
        fx = float(detector["fx"])
        fy = float(detector["fy"])
        hough_params = detector["hough"]
        dp = int(hough_params["dp"])
        min_dist = int(hough_params["minDist"])
        param1 = int(hough_params["param1"])
        param2 = int(hough_params["param2"])
        min_radius = int(hough_params["minRadius"])
        max_radius = int(hough_params["maxRadius"])
        return [dp, min_dist, None, param1, param2, min_radius, max_radius], [fx, fy]


def get_timestamp(file_name, timestamp_pattern,  max_timestamp):
    time_stamps = re.findall(timestamp_pattern+"\d+", file_name)
    if len(time_stamps)!=1:
        print("Setting timestamp to 0. Found {} timestamps. Setting timestamp to 0.".format(len(time_stamps)))
        return 0
    return float(time_stamps[0][len(timestamp_pattern):]) / float(max_timestamp)

def run_onnx_model(image, time,  session_or_model, batch_size=8):

    outputs = [o.name for o in session_or_model.get_outputs()]
    t_for_conv = np.ones(shape=(batch_size, 1, 224, 224), dtype=np.float32) * time
    t_for_fc = np.ones(shape=(batch_size, 1), dtype=np.float32) * time

    image =  np.concatenate (image, axis = 0)

    inputs = {
        'data': image,
        't_for_conv': t_for_conv,
        't_for_fc': t_for_fc
    }

    res = session_or_model.run(outputs, inputs )

    return res[0]

def run_pytorch_model(image, age, model_or_session, batch_size=8, device="cuda"):

    if not model_or_session:
        raise Exception("Model was not found")

    image_tensors = [torch.from_numpy(i) for i in image]
    image = torch.cat(image_tensors, dim=0)
    age = torch.tensor(age).view(-1).repeat(batch_size)
    data, age, = image.to(device), age.to(device)
    output = model_or_session(data, age)

    output = output.detach().cpu().numpy()
    return output


def run_model(image, time, session_or_model, batch_size, device):
    if path_to_model.endswith('onnx'):
        return run_onnx_model(image, time, session_or_model, batch_size)
    if path_to_model.endswith('pth'):
        return run_pytorch_model(image, time, session_or_model, batch_size, device)

    raise Exception('unsupported model format')



if (__name__ == "__main__"):

    arguments = argparse.ArgumentParser()
    arguments.add_argument("--config", "-c", type=str, help = "path to config ", required=True)
    arguments.add_argument("--model", "-m", type=str, help = "path to model if different from default path")
    arguments.add_argument("--input_folder", "-i", type=str, help = "path to folder with images", required=True)
    arguments.add_argument("--output_folder", "-o", type=str, help="path to folder with images", required=True)
    arguments.add_argument("--batch_size", "-b", type = int, choices=[1, 8], help = "model batchsize. default 8 ", default=8)
    arguments.add_argument("--pattern", "-p", type=str, help = "timestamp pattern", default="L0")
    arguments.add_argument("--max_timestamp", "-t", type=int, help = "maximum timestamp", required=True)
    arguments.add_argument("--device", "-d", type=str, help="Device to run on", choices = ["cuda", "cpu"], default="cuda")

    args = arguments.parse_args()

    hough_params, fx_fy = get_hough_params(args.config)

    path_to_model, num_classes = get_default_model_path(args.config)

    if args.model:
        path_to_model = args.model

    device = torch.device("cuda:0")
    if args.device == "cpu":
        device = torch.device("cpu")

    if path_to_model.endswith("onnx"):
        session_or_model = onnxruntime.InferenceSession(path_to_model)
        model_type = "onnx_" + str(args.batch_size)
    elif path_to_model.endswith("pth"):
        session_or_model = Embryo_severe_model.get_model(path_to_model, num_classes, final_softmax= True)
        session_or_model = session_or_model.to(device)
        model_type = "pytorch_" + str(args.batch_size)
    else:
        raise Exception ("Not known model type \n" + path_to_model )

    image_names = os.listdir(args.input_folder)
    input_images = [os.path.join(args.input_folder, image_name) for image_name in image_names]
    for input_image in input_images:

        age = get_timestamp(input_image, args.pattern, int(args.max_timestamp))

        image = cv2.imread(input_image)
        image_for_detection = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        image_for_detection = cv2.medianBlur(image_for_detection, 5)
        image_for_detection = cv2.resize(image_for_detection, None, fx= fx_fy[0], fy= fx_fy[1])
        circles = cv2.HoughCircles(image_for_detection, cv2.HOUGH_GRADIENT, *hough_params)[0]

        bounding_boxes = []
        for circle in circles:
            cx, cy = circle[0],circle[1]
            r = circle[2]
            cx, cy, r = map(round, [cx, cy, r])
            x1 = int(max(cx - r, 0) / fx_fy[0])
            y1 = int(max(cy - r, 0) / fx_fy[1])
            x2 = int(min(cx + r, image_for_detection.shape[1]) / fx_fy[0])
            y2 = int(min(cy + r, image_for_detection.shape[0]) / fx_fy[1])

            bounding_boxes.append([x1, y1, x2, y2])

        detections = []
        for bounding_box in bounding_boxes:
            x1, y1, x2, y2 = bounding_box
            detection_image = image[y1 : y2, x1 : x2,:]

            detection_images = prepare_image(detection_image, batchsize=args.batch_size)

            results = run_model(detection_images, age,  session_or_model, args.batch_size ,device)
            results = compute_average_score(results, args.batch_size)
            detections.append(Detection(x1, y1, x2, y2, results))

        save_output(input_image, args.output_folder, detections, args.config, model_type = model_type)


