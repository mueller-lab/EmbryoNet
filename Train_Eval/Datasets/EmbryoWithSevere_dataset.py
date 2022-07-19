#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller,
# Computer Vision Studio and Friedrich Miescher Laboratory of the Max Planck Society
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import torch
from torch.utils.data import Dataset
import cv2
import numpy as np
import json
import os


class EmbryoWithSevereDataset(Dataset):
    """
    torch.utils.data.Dataset interface for embryo classification training.
    The actual annotations are contained in self.database field
    self.database is filled either by passing json_path or in a subclass, such as BalancedDataset
    """
    def __init__(self,
                 json_path = None,
                 imshow = False,
                 classes = ['BMP', 'NODAL','NORMAL',
                            'UNKNOWN', 'BOOM','CUT',
                            'WNT','FGF','SHH','PCP','RA'],
                 classes_with_severities = ["BMP","NODAL"],
                 severities = [50, 75, 100],
                 network_image_input_size = (224, 224)):

        """
        Args:
            json_path (str or None): path to json annotation file or None, if database is filled in a subclass.
            imshow (bool): debug flag to display embryo image.
            classes (list[str]): phenotype class names.
            classes_with_severities (list[str])): phenotype with severities class names.
            severities (list[int]): severity list.
            network_image_input_size (tuple[int]): network image size.
        """
        self.imshow = imshow
        self.classes = classes
        self.classes_with_severities = classes_with_severities
        self.severities = severities
        self.network_input_image_size =  network_image_input_size
        if json_path is not None:
            with open(json_path, "r") as f:
                self.database = json.load(f)

    def __len__(self):
        return len(self.database)

    def __getitem__(self, idx):

        extensions = ['.png', '.jpg', '.jpeg', '.bmp', '.tif', '.tiff']
        img_path = self.database[idx][0]

        image = None
        def load_image(_img_path):
            nonlocal image
            if os.path.exists(_img_path):
               image = cv2.imread(_img_path, cv2.IMREAD_COLOR)
               return True
            return False

        paths_to_try = [img_path]
        paths_to_try.extend(os.path.splitext(img_path)[0] + e for e in extensions)
        for path in paths_to_try:
            if load_image(path):
                break

        assert image is not None, img_path

        dumped_json = self.database[idx][1]
        tlx = dumped_json["tlx"]
        tly = dumped_json["tly"]
        brx = dumped_json["brx"]
        bry = dumped_json["bry"]
        image = image[tly:bry, tlx:brx]

        if self.imshow:
            cv2.imshow("image", image)
            cv2.waitKey(0)

        class_name = dumped_json["className"]
        if class_name not in self.classes_with_severities:
            concentration_confident = True
        else:
            concentration_confident = dumped_json.get("concentrationConfident", True)

        severe = int(dumped_json.get("severe", 100))

        offset = self.class_name_2_offset[class_name]

 #       print("class_name : ", class_name, " -> offset : ", offset)
        if class_name in self.classes_with_severities:
            if severe in self.severe_2_offset:
                offset = offset + self.severe_2_offset[severe]

        prob = [0.0] * (max(self.class_name_2_offset.values()) + 1)
        prob[offset] = 1.0
        prob = torch.Tensor(prob)
        #if (class_name=="BMP"):
        #    print(class_name)
        #    print(severe)
        #    print (prob)

        image = cv2.resize(image, self.network_input_image_size)
        image = np.float32(image)

        image = np.transpose(image, axes=(2,0,1))
        image_path = self.database[idx][0]
        age = dumped_json["age"]

        sample = {'image': image,
                  'prob': prob ,
                  "concentrationConfident" : concentration_confident ,
                  "path": image_path,
                  "age": age}

        return sample

    @property
    def class_name_2_offset(self):
        offset = 0
        out_class_name_2_offset = {}
        for class_name in self.classes:
            #print("class_name : ", class_name, " -> len(self.severities) : ", len(self.severities))
            out_class_name_2_offset[class_name] = offset
            offset += len(self.severities) if class_name in self.classes_with_severities else 1   
        return out_class_name_2_offset

    @property
    def severe_2_offset(self):
        severity_map = {
            severity : severity_idx for
            (severity_idx ,severity ) in enumerate(sorted(self.severities, reverse=True))
        }
        return severity_map


