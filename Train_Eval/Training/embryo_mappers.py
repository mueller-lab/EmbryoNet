
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import torch.utils.data as data
import random
import numpy as np
import cv2
import copy

import albumentations as Alb

class MapDataset(data.Dataset):
    """
    Idea was taken from: https://github.com/facebookresearch/detectron2/blob/main/detectron2/data/common.py
    Map a function over the elements in a dataset.
    Args:
          dataset: a dataset where map function is applied.
          map_func: a callable which maps the element in dataset
          start_epoch - start applying filter
          end_epoch - stop applying filter
    """

    def __init__(self, dataset, map_func):
        self.epoch = 0
        self._dataset = dataset
        self._map_func = map_func


    def __len__(self):
        return len(self._dataset)

    def __getitem__(self, idx):
        while True:
            sample = copy.deepcopy(self._dataset[int(idx)])
            image = sample["image"]
            image = np.transpose(image, axes=(2, 1, 0))
            transformed_image = self._map_func(image, self.epoch)
            transformed_image = np.transpose(transformed_image, axes=(2, 0, 1))
            new_sample = sample
            new_sample["image"] = transformed_image
            return new_sample




class FlipFunc:
    """
    Flip function for EmbryoDatasetMapper.
    Args:
        flip code: A flag to specify how to flip the array; 0 means flipping
        around the x-axis  and positive value (for example, 1) means flipping around
        y-axis. Negative value (for example, -1)  means flipping around both axes.
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """

    def __init__(self, flip_code, start_epoch = 0, end_epoch = 1000):
        self.flip_code = flip_code
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch


    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        return cv2.flip(image,self.flip_code)


class RotateFunc:
    """
    Rotate function for EmbryoDatasetMapper
    Args:
        rotation_code,
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """
    def __init__(self, rotation_code, start_epoch = 0, end_epoch = 1000):
        self.rotation_code = rotation_code
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image

        return cv2.rotate(image , self.rotation_code)


class EmptyFunc:
    """
    The empty function, does nothing.
    Returns input.
    """
    def __init__(self, start_epoch=0, end_epoch=1000):
        pass

    def __call__(self, image, epoch):
        return image


class SmallRotateFunc:
    """
    Rotate function for EmbryoDatasetMapper
    Args:
        rotation_angle,
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """
    def __init__(self, rotation_angle, start_epoch = 0, end_epoch =1000):
        self.rotation_angle = rotation_angle
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch

    def rotate_image(self, image):
        result = Alb.rotate(image, self.rotation_angle)
        return result

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        result = self.rotate_image(image)
        return result


class AdjustGammaFunc:
    """
    Adjust gamma function for EmbryoDatasetMapper.
    Args:
        gamma_power: the power of Gamma function.
        Powers greater than 1 makes image look brighter.
        Powers smaller than 1 makes shadows look darker.
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """
    def __init__(self, gamma_power, start_epoch = 0, end_epoch = 1000):
        self.gamma_power = gamma_power
        self.invGamma = 1.0 / gamma_power
        self.table = np.array([((i / 255.0) ** self.invGamma) * 255
                          for i in np.arange(0, 256)]).astype("uint8")

        self.start_epoch = start_epoch
        self.end_epoch = end_epoch


    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image

        image = (image * 255).astype(np.uint8)
        output = cv2.LUT(image, self.table)
        return (output / 255.0).astype(np.float32)


class SaltAndPepperNoiseFunc:
    """
      Salt and pepper function for EmbryoDatasetMapper
      Args:
          SNP prob, SNP defines probability list for each pixel on the image following will apply:
          [SNP(pixel is not changed), (1 - SNP) / 2. (pixel becomes white), (1 -SNP) / 2.( pixel becomes black)])
          start_epoch - start applying filter
          end_epoch - stop applying filter
      """
    def __init__(self, SNP, start_epoch = 0, end_epoch = 1000):
        self.SNP = SNP
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        img_ = image.copy()
        h, w, c = image.shape
        mask = np.random.choice((0, 1, 2), size=(h, w, 1), p=[self.SNP, (1 - self.SNP) / 2., (1 - self.SNP) / 2.])
        mask = np.repeat(mask, c, axis=2)  # Copy by channel to have the same shape as img
        img_[mask == 1] = 1  # salt noise
        img_[mask == 2] = 0  #
        return  img_


class CropFunc:
    """
    Adjust crops pixels for EmbryoDatasetMapper.
    Args:
        crop_size - pixels to crop,
        top - cropped from top,
        left - cropped from left,
        bottom - cropped from bottom,
        right - cropped from right,
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """
    def __init__(self, crop_size, top, left, bottom, right, start_epoch=0, end_epoch =1000):
        self.top = top
        self.left = left
        self.bottom = bottom
        self.right = right
        self.crop_size = crop_size
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        shape = image.shape
        top = self.top * self.crop_size
        left = self.left * self.crop_size
        bottom = shape[0] - self.bottom * self.crop_size
        right = shape[1]- self.right * self.crop_size
        output_image = image[top:bottom, left:right, :]
        output_image = cv2.resize(output_image, (shape[0], shape[1]))
        return output_image


class OpticalDistortionFunc:

    """
    For implementation details check Albumentations:
    https://github.com/albumentations-team/albumentations/blob/master/albumentations/augmentations/transforms.py#L410
    This OpticalDistortFunc is just a wrapper.
     Args:
        param - power of optical distortion[0-9]  positive integer in range(9). Param describes which default transform
                                              from pre-defined possible transform list is used.
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """

    def __init__(self, param, start_epoch=0, end_epoch=1000):
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch
        self.param = param
        self.transform = [Alb.OpticalDistortion(distort_limit=0.75, shift_limit=0.75, always_apply=True) for _ in range(9)]

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        output_image = self.transform[self.param](image=image)['image']
        return output_image


class GridDistortionFunc:
    """
    For implementation details check Albumentations:
    https://github.com/albumentations-team/albumentations/blob/master/albumentations/augmentations/transforms.py#L410
    This OpticalDistortFunc is just a wrapper.
     Args:
        param - power of grid distortion[0-9]. Positive integer in range(9). Param describes which default transform
                                              from pre-defined possible transform list is used.
        start_epoch - start applying filter
        end_epoch - stop applying filter

    """

    def __init__(self, param, start_epoch=0, end_epoch=1000):
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch
        self.param =param
        self.transform = [Alb.GridDistortion(always_apply=True) for _ in range(9)]

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        output_image = self.transform[self.param ](image=image)['image']
        return output_image

class SharpenFunc:
    """Sharpen the input image and overlays the result with the original image.
        For implementation details check Albumentations:
        https://github.com/albumentations-team/albumentations/blob/master/albumentations/augmentations/transforms.py#L410
        This OpticalDistortFunc is just a wrapper.
        Args:
            param - number of SharpenFunc to take. Positive integer.
                                            Param describes which default transform
                                            from pre-defined possible transform list is used.
            start_epoch - start applying filter
            end_epoch - stop applying filter
    """

    def __init__(self, param, start_epoch=0, end_epoch=1000):
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch
        self.param = param
        alphas = np.arange(0.1, 1.0, 0.1)
        lightness = np.arange(0.5, 1.0, 0.05)
        self.transform = [Alb.IAASharpen(alpha=(alpha, alpha),
                                         lightness=(light, light),
                                         always_apply=True)    for alpha, light in zip (alphas ,lightness)]

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image

        output_image = self.transform[self.param](image=image)['image']

        return output_image


class MotionBlurFunc:
    """ Apply motion blur to the input image using a random-sized kernel.

         For implementation details check Albumentations:
         https://github.com/albumentations-team/albumentations/blob/master/albumentations/augmentations/transforms.py#L410
         This MotionBlurFunc is just a wrapper.
         Args:
             param - number describes which Motion Blur function to take.
                                             Param describes which default transform
                                             from pre-defined possible transform list is used.
             start_epoch - start applying filter
             end_epoch - stop applying filter
     """

    def __init__(self, param, start_epoch=0, end_epoch=1000):
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch
        self.param = param
        blur_limits = np.arange(7, 31, 2)
        self.transform = [Alb.MotionBlur(blur_limit=(limit,limit), always_apply=True) for limit in blur_limits]

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        output_image = self.transform[self.param](image=image)['image']
        return output_image

class RandomBrightnessContrastFunc:
    """
    Randomly change brightness and contrast of the input image.
    For implementation details check Albumentations:
         https://github.com/albumentations-team/albumentations/blob/master/albumentations/augmentations/transforms.py#L410
         This RandomBrightnessContrastFunc is just a wrapper.
         Args:
             param - number describes which Random Brightness ContrastFunc function to take.
                                             Param describes which default transform
                                             from pre-defined possible transform list is used.
             start_epoch - start applying filter
             end_epoch - stop applying filter
    """
    def __init__(self, param, start_epoch=0, end_epoch=1000):
        self.start_epoch = start_epoch
        self.end_epoch = end_epoch
        self.param = param
        self.transform = [Alb.RandomBrightnessContrast(brightness_limit=0.3, contrast_limit=0.3, always_apply=True) for _ in range(9)]
    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image
        output_image = self.transform[self.param](image=image)['image']
        return output_image



class ShadowFunc:
    """
    Shadow augmentation for EmbryoDatasetMapper
    Args:
        Shadow code: a flag to specify shadow direction:

        0 means horisontal from right to left
        1 means horisontal from left to right
        2 means vertical from top to bottm
        3 means vertical from bottom to top
        4 means diagonal to be implemented
        5 means diagonal to be implemented
        6 means diagonal to be implemented
        7 means diagonal to be implemented
        start_epoch - start applying filter
        end_epoch - stop applying filter
    """

    def __init__(self, param, start_epoch=0, end_epoch=1000,
                 default_width = 224, default_height=224):

        self.shadow_dict = {0:"horizontal",
                            1:"horizontalr",
                            2:"vertical",
                            3:"verticalr",
                            4:"diagonal",
                            5:"diagonal2",  # 3
                            6:"diagonalr",
                            7:"diagonal2r"}

        self.shadow_code = param[0]
        self.min_val = param[1]
        self.max_val = param[2]
        self.shadow_mask = self.generate_shadow(self.shadow_dict[self.shadow_code],
                                                default_width, default_height,
                                                max_val= self.max_val,
                                                min_val= self.min_val)

        self.start_epoch = start_epoch
        self.end_epoch = end_epoch

    def generate_shadow(self, shadow_code, width, height, min_val = 0, max_val = 255):
        base = np.array([np.arange(min_val, max_val, dtype=np.float32)] * 255) / 255.
        # because we want to have pixel step of 1.
        diag_base = []
        for y in range(256):
            #for every line
            diag_base.append([])
            for x in range(min_val, max_val):
                diag_base[-1].append((x + y) / 2)

        diag_base = np.array(diag_base, dtype=np.float32) / 255

        shadows = {
            "horizontal": base,                        # 1
            "vertical": np.rot90(base, 3),             # 2    diag
            "horizontalr": np.fliplr(base),            # 3
            "verticalr": np.rot90(base, 1),            # 1
            "diagonal": diag_base,                     # 2    diag2
            "diagonal2": np.rot90(diag_base, 3),       # 3
            "diagonalr": np.rot90(diag_base, 2),
            "diagonal2r": np.rot90(diag_base, 1),
        }
        shadow_image = shadows[shadow_code]
        return shadow_image


    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image

        shape = image.shape

        shadow_mask =np.float32(cv2.resize(self.shadow_mask, (shape[1], shape[0])))
        shadow_mask = cv2.cvtColor(shadow_mask, cv2.COLOR_GRAY2RGB)

        return cv2.multiply(np.float32(image), shadow_mask) # check if sizes are equal



class ApplyDirtFunc:
    def __init__(self, param, start_epoch=0, end_epoch=1000):
        self.param = param
        self.start_epoch =start_epoch
        self.end_epoch = end_epoch
        self.precipitate = cv2.imread(param, 0)

    def __call__(self, image, epoch):
        if epoch < self.start_epoch or epoch > self.end_epoch:
            return image

        shape = image.shape
        tlx = 0
        tly = 0
        brx = 224
        bry = 224
        precipitate_h, precipitate_w = self.precipitate.shape
        image_h, image_w, _ = image.shape

        tlx = random.randrange(0, precipitate_w -image_w)
        tly = random.randrange(0, precipitate_h - image_h)
        brx = tlx + image_w
        bry = tly + image_h

        precipitate =self.precipitate[tly:bry, tlx:brx]
        precipitate = cv2.cvtColor(precipitate, cv2.COLOR_GRAY2RGB)
        precipitate = precipitate/255.0

        result = cv2.multiply(np.float32(image), np.float32(precipitate))

        return  result # check if sizes are equal

    def apply(self, image):
        image = np.float32(image.copy())
        shadow = self.add[0]
        height, width, channels = image.shape
        for y in range(height):
            for x in range(width):
                image[y, x] = image[y, x] * shadow.get_pixel(x / width, y / height)
        return image


class CombinedMapperFunc:
    """
    Combine different mapper functions for Embryo Dataset mappers
    Args:
        function_groups list[list[MapperFunc]]: list of grouped mapper functions
                                                Each external list element is a list of similar mapper functions
         always_true (bool) - If True from each group one mapper function is always applied.
                              If False applying probability of a function from the group is 0.5
    """

    def __init__(self, function_groups, always_true=False):
        self.function_groups = function_groups
        self.always_true = always_true


    def __call__(self, image, epoch):

        output_image = image / 255.0
        for funtion_group in self.function_groups:
            apply = self.always_true
            if not apply:
                apply = random.getrandbits(1)
            if apply:
                rnd = random.choice(funtion_group)
                output_image = rnd(output_image, epoch)
        return output_image * 255


if __name__=="__main__":
    img = cv2.imread(r"F:\embryolearning\Sashas_code\orig.png")

    applyDirtFunc = ApplyDirtFunc("F:\\embryolearning\\Sashas_code\\Precipitate.tif", 0, 1000)
    #applyDirtFunc = ApplyDirtFunc(r"F:\embryolearning\Sashas_code\dirt.png", 0, 1000)

    dirty_image = applyDirtFunc(img, 100)

    #for i in range (0,100):
    #    dirty_image = applyDirtFunc(img, 100)
    #    cv2.imwrite(r"F:\embryolearning\Sashas_code\Precipitation_embryos\real_precipitation_"+ str(i)+".png", dirty_image)

    for min_v in range(0, 200, 50):
        for max_v in range(200, 500,50):
            for i in range(8):
                shadowFunc = ShadowFunc([i, min_v, max_v])
                shadow_img = shadowFunc(img, 100)
                cv2.imwrite(r"F:\embryolearning\Sashas_code\Precipitation_embryos\Shadows\shadow_"
                            + str(i)+"min_" +str(min_v) +"_"
                            "max_" + str(max_v) + "_" + ".png", shadow_img)
