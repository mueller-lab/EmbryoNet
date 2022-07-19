
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import Training.embryo_mappers as EM

import json


def get_flip_funcs(range):
    flip_funcs = []

    for val in range:
        flip_funcs.append(EM.FlipFunc(val))
    return flip_funcs


def get_rotate_funcs(range):
    rotate_funcs = []
    for val in range:
        rotate_funcs.append(EM.RotateFunc(val))
    return rotate_funcs


def get_small_rotation_funcs(rng):

    small_rotation_functions = []

    for i in range(rng[0], rng[1], rng[2]):
        small_rotation_functions.append(EM.SmallRotateFunc(i))

    small_rotation_functions.append(EM.EmptyFunc())
    return small_rotation_functions


def get_crop_funcs (rng):

    crop_funcs = []

    for i in range(rng[0], rng[1], rng[2]):
        crop_funcs.append(EM.CropFunc(i, 1, 1, 1, 1))
        crop_funcs.append(EM.CropFunc(i, 1, 1, 0, 0))
        crop_funcs.append(EM.CropFunc(i, 0, 0, 1, 1))
        crop_funcs.append(EM.CropFunc(i, 1, 0, 0, 1))
        crop_funcs.append(EM.CropFunc(i, 0, 1, 1, 0))

        crop_funcs.append(EM.CropFunc(i, 1, 0, 0, 0))
        crop_funcs.append(EM.CropFunc(i, 0, 1, 0, 0))
        crop_funcs.append(EM.CropFunc(i, 0, 0, 1, 0))
        crop_funcs.append(EM.CropFunc(i, 0, 0, 0, 1))

        crop_funcs.append(EM.CropFunc(i, 0, 1, 1, 1))
        crop_funcs.append(EM.CropFunc(i, 1, 0, 1, 1))
        crop_funcs.append(EM.CropFunc(i, 1, 1, 0, 1))
        crop_funcs.append(EM.CropFunc(i, 1, 1, 1, 0))

    return crop_funcs


def get_gamma_funcs (rng):

    gamma_funcs = []

    for i in range(rng[0], rng[1], rng[2]):
        gamma_positiv = float(100 + i) / 100
        gamma_negativ = float(100 - i) / 100
        gamma_funcs.append(EM.AdjustGammaFunc(gamma_positiv))
        gamma_funcs.append(EM.AdjustGammaFunc(gamma_negativ))

    return gamma_funcs


def get_snp_funcs(range):
    return [EM.SaltAndPepperNoiseFunc(range[0])]


def get_randomBrightnessContrast_funcs(range):
    return [EM.RandomBrightnessContrastFunc(range[0])]


def get_sharpen_funcs(range):
    return [EM.SharpenFunc(range[0])]


def get_motionblur_funcs(range):
    return [EM.MotionBlurFunc(range[0])]


def get_GridDistortion_funcs (range):
    return [EM.GridDistortionFunc(range[0])]


def get_opticalDistortion_funcs(range):
    return [EM.OpticalDistortionFunc(range[0])]


def get_augmentation_mappers(epoch, augmentatnion_mapper_desc = "augmenatation_mapper_example.json"):

    """
     Creates augmentation mappers according to augmentation mapper description.
     Depending on epoch different augmentation mappers are applied.

     Args:
          epoch (int): epoch to create augmentation_mappers for.
          augmentatnion_mapper_desc (str or dict): path to json file or json_file_dictionary itself.
          Json file describes augmentation mappers per epoch.
     """
    flip_funcs = [EM.EmptyFunc()]
    rotate_funcs = [EM.EmptyFunc()]
    small_rotationFunctions = [EM.EmptyFunc()]
    crop_funcs = [EM.EmptyFunc()]
    gamma_funcs = [EM.EmptyFunc()]
    randomBrightnessContrast_funcs = [EM.EmptyFunc()]
    sharpen_funcs = [EM.EmptyFunc()]
    motionblur_funcs = [EM.EmptyFunc()]
    snp_noize = [EM.EmptyFunc()]
    grid_distortion_funcs = [EM.EmptyFunc()]
    optical_distortion_funcs = [EM.EmptyFunc()]

    name_2_function_group_map = {
        "flip": (flip_funcs, get_flip_funcs),
        "rotate": (rotate_funcs, get_rotate_funcs),
        "small_rotate": (small_rotationFunctions, get_small_rotation_funcs),
        "crop": (crop_funcs, get_crop_funcs),
        "gamma": (gamma_funcs, get_gamma_funcs),
        "salt_and_pepper": (snp_noize, get_snp_funcs),
        "randomBrightnessContrast": (randomBrightnessContrast_funcs, get_randomBrightnessContrast_funcs),
        "sharpen": (sharpen_funcs, get_sharpen_funcs),
        "motionblur": (motionblur_funcs, get_motionblur_funcs),
        "GridDistortion": (grid_distortion_funcs, get_GridDistortion_funcs),
        "OpticalDistortion": (optical_distortion_funcs, get_opticalDistortion_funcs)
    }

    if isinstance(augmentatnion_mapper_desc, str):
        with open(augmentatnion_mapper_desc) as f:
            augmentation_config = json.load(f)
    else:
        assert isinstance(augmentatnion_mapper_desc, list), augmentatnion_mapper_desc
        augmentation_config = augmentatnion_mapper_desc

    for augmenatnion in augmentation_config:

        if augmenatnion["epoch"] > epoch:
            continue

        for mapper in augmenatnion["mappers"]:
            print(mapper["name"])
            aug_lst, aug_gen_func = name_2_function_group_map[mapper["name"]]
            aug_lst.extend(aug_gen_func(mapper["range"]))


    all_functions = [flip_funcs, rotate_funcs, small_rotationFunctions, crop_funcs, gamma_funcs,
                     crop_funcs, gamma_funcs, randomBrightnessContrast_funcs,
                     sharpen_funcs, motionblur_funcs, snp_noize,
                     grid_distortion_funcs, optical_distortion_funcs]

    return all_functions
