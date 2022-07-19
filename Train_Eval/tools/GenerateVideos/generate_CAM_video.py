
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

from tools.GenerateVideos import generate_cam

import argparse
from collections import defaultdict
import json
import os

import cv2
import numpy as np
import numpy.polynomial.polynomial as poly

def combine(background, image, offsetx, offsety):
    assert len(image.shape) == 3, image.shape
    rows, cols, _ = image.shape
    background[offsety : offsety + rows, offsetx : offsetx + cols] = image
    return background


def size(label, font, fontScale, lineType):
    (label_width, label_height), baseline = cv2.getTextSize(label, font, fontScale, lineType)
    return label_width, label_height + baseline, baseline


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", "--path_to_experiment",
                        type=str,
                        required=True,
                        help= "path to folder with 'images' folder and 'json' folders")

    parser.add_argument("-json", "--json_name", type=str, required=False,
                        default="Classified_result_1_json_corrected",  help="json folder name")

    parser.add_argument("-o", "--path_to_output", type=str, required=True, help="path to save output")
    parser.add_argument("--ids", type=int, nargs="+", required=True, help="ids to process")
    parser.add_argument("--width", type=int, required=False, help="width of result image", default=800)
    parser.add_argument("--height", type=int, required=False, help="height of result image",default=530)
    parser.add_argument("-s", "--shift", type=int,  help="shift of result image", default=5)
    parser.add_argument("-name", "--experiment_name", type=str, required=False, help="name of experiment")
    parser.add_argument('--model_path', type=str)
    parser.add_argument('-wsh', type=int, default=17, help= "classifier class number. Can be seen in netron. "
                                                            "GEMM.output")
    parser.add_argument('-wsw', type=int, default=2049,  help = "fully connected input number, Can be seen in netron."
                                                                "GEMM.input")
    parser.add_argument('--fc_weight', type=str, default='base_module.tail_prob.fc1.weight')
    parser.add_argument('-W', type=int, default=224, help = "network input width")
    parser.add_argument('-H', type=int, default=224, help = "network input height")
    parser.add_argument('-p', "--polynom", type=int, default=5, help = "smoothing polynom  power")
    parser.add_argument("-vc", "--visualize_class", type = int, default=-1, help = "a class to visualize reaction for."
                                                                        "Default -1: visualizes argmax for classification.")


    args = parser.parse_args()
    shift = args.shift

    path_to_experiment = args.path_to_experiment

    json_folder = os.path.join(path_to_experiment, args.json_name)
    path_to_images = os.path.join(path_to_experiment, "images")
    output_path = args.path_to_output

    width = args.width
    height = args.height

    ids = args.ids

    json_files = sorted(os.listdir(json_folder))
    image_files = sorted(os.listdir(path_to_images))

    if len(json_files) != len(image_files):
        print("wrong file numbers")

    wdh = defaultdict(list)
    hih = defaultdict(list)

    centers_x = defaultdict(list)
    centers_y = defaultdict(list)

    for index in range(len(json_files)):

        json_full_path = os.path.join(json_folder, json_files[index])

        with open(json_full_path) as jf:
            json_dict = json.loads(jf.read())

        detections = json_dict["detection_list"]

        for detection in detections:

            if detection["id"] in ids:

                if index % 100 == 0:
                    print("loading...", os.path.join(str(index), str(len(json_files))), "images", len(detections), "detections")

                tlx = detection["tlx"]
                tly = detection["tly"]
                brx = detection["brx"]
                bry = detection["bry"]
                wdh[detection["id"]].append(brx - tlx)
                hih[detection["id"]].append(bry - tly)
                centers_x[detection["id"]].append( (tlx + brx) // 2)
                centers_y[detection["id"]].append( (tly + bry) // 2)


    poly_coeffs_x = dict()
    poly_coeffs_y = dict()

    for detection_id in wdh.keys():
        wdh[detection_id] = max(wdh[detection_id])
        hih[detection_id] = max(hih[detection_id])
        poly_coeffs_x[detection_id] = np.polyfit(list(range(len(centers_x[detection_id]))),
                                                  centers_x[detection_id], args.polynom)

        poly_coeffs_y[detection_id] = np.polyfit(list(range(len(centers_y[detection_id]))),
                                                    centers_y[detection_id], args.polynom)


    for index in range(0, len(json_files)):
        path_to_image = os.path.join(path_to_images, image_files[index])
        image = cv2.imread(path_to_image)
        json_full_path = os.path.join(json_folder, json_files[index])

        with open(json_full_path) as jf:
            json_dict = json.loads(jf.read())

        print("compiling...", os.path.join(str(index), str(len(json_files))), "images")
        detections = json_dict["detection_list"]

        max_h, max_w, chan = image.shape

        for ind, detection in enumerate(detections):
            detection_id = detection["id"]
            if detection_id not in wdh:
                continue

            detection = detections[ind]

            center_x = int(poly.polyval(index, poly_coeffs_x[detection_id][::-1]))
            center_y = int(poly.polyval(index, poly_coeffs_y[detection_id][::-1]))

            new_tlx = center_x - wdh[detection_id] // 2
            new_tly = center_y - hih[detection_id] // 2
            new_brx = center_x + wdh[detection_id] // 2
            new_bry = center_y + hih[detection_id] // 2

            new_tlx = max(0, new_tlx)
            new_tly = max(0, new_tly)
            new_brx = min(new_brx, max_w)
            new_bry = min(new_bry, max_h)

            embryo = image[new_tly:new_bry, new_tlx:new_brx, :]
            track_id = detection_id
            if track_id not in ids:
                continue

            os.listdir(output_path)
            os.makedirs(os.path.join(os.path.join(output_path, str(track_id))), exist_ok=True)

            strindex = str(index)
            while len(strindex) < 4:
                strindex = "0" + strindex

            print(os.path.join(output_path, str(track_id), strindex + ".png"))
            
            img = np.zeros((height, width, 3))

            rows, cols, _ = embryo.shape
            font                   = cv2.FONT_HERSHEY_SIMPLEX
            fontScale              = 1
            fontColor              = (255,255,255)
            lineType               = 2

            img = combine(img, embryo, shift, shift)
            time = float(index) / len(json_files)
            input_size = args.H, args.W

            original_embryo = embryo
            embryo = np.float32(embryo)

            embryo_width, embryo_height, embryo_channels = embryo.shape
            resized_embryo = cv2.resize(embryo, (224,224))

            embryo = generate_cam.img_to_blob(resized_embryo)
            embryo_cam = generate_cam.CAM = generate_cam.calculate_CAM(embryo,
                                                               args.model_path, time,
                                                               input_size,
                                                               (args.wsh, args.wsw),
                                                               args.fc_weight,
                                                               args.visualize_class)
            embryo_cam = cv2.equalizeHist(embryo_cam)
            embryo_cam = cv2.resize(embryo_cam, (embryo_width,embryo_height))

            heatmap = cv2.applyColorMap(cv2.resize(embryo_cam, (embryo_width, embryo_height)), cv2.COLORMAP_JET)
            result = heatmap * 0.3 + original_embryo * 0.5

            img = combine(img, result, width - shift - cols, shift)

            y = 2 * shift + rows

            label = "registered embryo image"

            lwidth, lheight, baseline = size(label,font,fontScale,lineType)

            cv2.putText(img,label,(shift, y+lheight), font, fontScale, fontColor,lineType,cv2.LINE_AA)

            label="activation map"
            lwidth, lheight, baseline = size(label,font,fontScale,lineType)

            cv2.putText(img,label,(width - shift - lwidth, shift*2+rows+lheight), font, fontScale, fontColor,lineType,cv2.LINE_AA)

            y += lheight + shift

            label = "classification result: "
            lwidth, lheight, baseline = size(label,font,fontScale,lineType)
            cv2.putText(img,label,(shift, y+lheight), font, fontScale, fontColor,lineType,cv2.LINE_AA)

            label=detection["className"] + " : " + str(detection.get("severe", 100))
            lwidth, lheight, baseline = size(label,font,fontScale,lineType)
            cv2.putText(img,label,(width - shift - lwidth, y+lheight), font, fontScale, fontColor,lineType,cv2.LINE_AA)
            y += lheight + shift

            label=args.experiment_name
            lwidth, lheight, baseline = size(label,font,fontScale,lineType)
            cv2.putText(img,label,(width // 2 - lwidth // 2, y+lheight), font, fontScale, fontColor,lineType,cv2.LINE_AA)
            cv2.imwrite(os.path.join(output_path, str(track_id), strindex + ".png"), img)


if (__name__=="__main__"):
    main()
