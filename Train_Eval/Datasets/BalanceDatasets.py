#=======================================================================================================================
#Copyright
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller,
# Computer Vision Studio and Friedrich Miescher Laboratory of the Max Planck Society
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

from Datasets.EmbryoWithSevere_dataset import EmbryoWithSevereDataset

import random
import os

class BalancedDataset(EmbryoWithSevereDataset):
    """
    BalancedDataset is  EmbryoWithSevereDataset subclass with balanced database field
    """
    def __init__(self, database, imshow=False, classes= ["BMP", "NODAL", "NORMAL",
                                                        "UNKNOWN", "BOOM",
                                                        "WNT","FGF","SHH", "RA", "PCP" ],
                                               classes_with_severities = ["BMP","NODAL"],
                                               severities = [50, 75, 100]):
        """
           Args:
               database(list[list[str, dict]]): paths to images with annotations.
               imshow (bool): show embryo for debugging.
               classes (list[str]): phenotype class names.
               classes_with_severities (list[str]): severity phenotype class names.
           """

        super(BalancedDataset, self).__init__(imshow=imshow,
                                              classes=classes,
                                              classes_with_severities=classes_with_severities,
                                              severities=severities)
        self.database = database


def balance_datasets(datasets,
                     class_names = ["BMP", "NODAL", "NORMAL",
                                    "UNKNOWN", "BOOM",
                                    "WNT","FGF","SHH", "RA", "PCP" ],
                     classes_with_severities= ["BMP","NODAL"],
                     severities = [50, 75, 100],
                     default_class_name = "UNKNOWN100",
                     default_class_undersample_prob = 0.66):
    """
    Balance given datasets: creates a dataset with equal number of samples for each class.
    The function will not work if any dataset length is 0.
    Args:
        class_names (list[str]): phenotype class names.
        classes_with_severities (list[str]): class names with severities.
        severities (list[int]): severity list.
        default_class_name (str): most frequent class name to undersample.
        default_class_undersample_prob (float): probability to drop most frequent class.
    """

    generated_class_names = []
    for class_name in class_names:
        if class_name in classes_with_severities:
            for severity in severities:
                generated_class_names.append(class_name + str(severity))
        else:
            generated_class_names.append(class_name + "100")

    class_2_num_entries = { class_name: 0 for class_name in generated_class_names}
    class_2_embryos = { class_name: [] for class_name in generated_class_names }

    for dataset in datasets:

        database = dataset.database

        for elem in database:
            dumped_json = elem[1]
            class_name = dumped_json["className"]
            age = dumped_json["age"]
            assert age <= 1, elem
            severity = dumped_json.get("severe", 100)
            if severity is None:
                severity = 100
            if class_name + str(int(severity)) not in generated_class_names:
                continue
            class_name = class_name + str(int(severity))
            if class_name == default_class_name:
                rnd = random.random()
                if rnd < default_class_undersample_prob:
                    continue
            class_2_embryos[class_name].append(elem)
            class_2_num_entries[class_name] += 1


    max_entries_per_class = max(class_2_num_entries.values())
    print(class_2_num_entries)
    for class_name in class_2_embryos:

        current_class_idx = 0

        if len(class_2_embryos[class_name]) == 0:
            print('No {} embryos were found in loaded datasets'.format(class_name))
            print('Cannot perform dataset balancing')
            exit()

        num_elems_per_class = len(class_2_embryos[class_name])
        while len(class_2_embryos[class_name]) < max_entries_per_class:
            if current_class_idx == num_elems_per_class:
                current_class_idx = 0
            class_2_embryos[class_name].append(class_2_embryos[class_name][current_class_idx])
            current_class_idx += 1

    balanced_database = []
    for class_name in class_2_embryos:
        balanced_database.extend(class_2_embryos[class_name])

    return BalancedDataset(balanced_database, classes=class_names, classes_with_severities=classes_with_severities, severities= severities)


def get_datasets(train_dataset_folder,
                 class_names=["NORMAL", "NODAL", "BMP", "WNT","FGF",
                              "SHH", "PCP","RA","BOOM", "UNKNOWN"],
                 classes_with_severities = ["BMP", "NODAL"],
                 severities = [50, 75, 100]):
    """
       get_datasets: reads dataset annotation jsons from given folder and then forms a balanced dataset.

       Args:
           train_dataset_folder (str): folder with train datasets' annotation json files
           class_names (list[str]): phenotype class names
           classes_withs_severities (list[str]): class names with severities
       """ 

    train_datasets = []
    for dataset_json in os.listdir(train_dataset_folder):
        path_to_description_json = os.path.join(train_dataset_folder, dataset_json)
        train_datasets.append(EmbryoWithSevereDataset(path_to_description_json,
                                                      imshow=False,
                                                      classes=class_names,
                                                      classes_with_severities=classes_with_severities,
                                                      severities = severities))

    train_balanced_embryoDataset_concat = balance_datasets(train_datasets, class_names = class_names,
                                                          classes_with_severities=classes_with_severities,
                                                          severities= severities)

    return train_balanced_embryoDataset_concat
