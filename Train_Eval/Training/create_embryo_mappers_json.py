
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import json
import os

def create_embryo_mappers_list():
    augmenatation_mapper =  [
            {
            'epoch': 0,
            'mappers': [
                {
                    'name': 'rotate',
                    'range': [0, 1, 2],
                },
                {
                    'name': 'flip',
                    'range': [1, 0, -1],
                },

            ]
        },

        {
            'epoch': 2,
            'mappers': [
                {
                    'name': 'small_rotate',
                    'range': [0, 45, 45],
                }
            ]
        },
        {
            'epoch': 5,
            'mappers': [
                {
                    'name': 'small_rotate',
                    'range': [30, 60, 15],
                }
            ]
        },
        {
            'epoch': 7,
            'mappers': [
                {
                    'name': 'small_rotate',
                    'range': [15, 75, 15],
                }
            ]
        },
        {
            'epoch':8,
            'mappers': [
                {
                    'name': 'small_rotate',
                    'range': [5, 85, 5],
                }
            ]
        },
        {
            'epoch': 10,
            'mappers': [
                {
                    'name': 'small_rotate',
                    'range': [0, 90 , 1],
                }
            ]
        },
        {
                'epoch': 12,
                'mappers': [
                    {
                        'name': 'crop',
                        'range': [0, 10, 1],
                    }
                ]
        },
            {
                'epoch': 14,
                'mappers': [
                    {
                        'name': 'crop',
                        'range': [10, 20, 1],
                    }
                ]
        },
            {
                'epoch':15,
                'mappers': [
                    {
                        'name': 'crop',
                        'range': [20, 30, 1],
                    }
                ]
        },
            {
                'epoch': 18,
                'mappers': [
                    {
                        'name': 'gamma',
                        'range': [0, 10, 1],
                    }
                ]
        },
            {
                'epoch': 20,
                'mappers': [
                    {
                        'name': 'gamma',
                        'range': [10, 20, 1],
                    }
                ]
        },
            {
                'epoch': 23,
                'mappers': [
                    {
                        'name': 'salt_and_pepper',
                        'range': [0.98],
                    }
                ]
        },

            {
                'epoch': 24,
                'mappers': [
                    {
                        'name': 'salt_and_pepper',
                        'range': [0.98],
                    }
                ]
            },
            {
                'epoch':26,
                'mappers': [
                    {
                        'name': 'randomBrightnessContrast',
                        'range': [0],
                    }
                ]
            },
    {
                'epoch': 28,
                'mappers': [
                    {
                        'name': 'randomBrightnessContrast',
                        'range': [1],
                    }
                ]
            },

            {
                'epoch':30,
                'mappers': [
                    {
                        'name': 'sharpen',
                        'range': [0],
                    }
                ]
            },

            {
                'epoch': 32,
                'mappers': [
                    {
                        'name': 'sharpen',
                        'range': [1],
                    }
                ]
            },

            {
                'epoch': 34,
                'mappers': [
                    {
                        'name': 'sharpen',
                        'range': [2],
                    }
                ]
            },

            {
                'epoch': 36,
                'mappers': [
                    {
                        'name': 'motionblur',
                        'range': [0],
                    }
                ]
            },
            {
                'epoch': 38,
                'mappers': [
                    {
                        'name': 'motionblur',
                        'range': [1],
                    }
                ]
            },

            {
                'epoch': 40,
                'mappers': [
                    {
                        'name': 'motionblur',
                        'range': [2],
                    }
                ]
            },

            {
                'epoch': 42,
                'mappers': [
                    {
                        'name': 'GridDistortion',
                        'range': [0],
                    }
                ]
            },
            {
                'epoch': 44,
                'mappers': [
                    {
                        'name': 'GridDistortion',
                        'range': [1],
                    }
                ]
            },
            {
                'epoch': 46,
                'mappers': [
                    {
                        'name': 'GridDistortion',
                        'range': [2],
                    }
                ]
            },
            {
                'epoch': 48,
                'mappers': [
                    {
                        'name': 'OpticalDistortion',
                        'range': [0],
                    }
                ]
            },

            {
                'epoch': 50,
                'mappers': [
                    {
                        'name': 'OpticalDistortion',
                        'range': [1],
                    }
                ]
            },

            {
                'epoch': 52,
                'mappers': [
                    {
                        'name': 'OpticalDistortion',
                        'range': [2],
                    }
                ]
            }
        ]


    return augmenatation_mapper


def dump_augmentation_mappers(augmenatation_mapper, save_path='augmenatation_mapper.json'):
    if os.path.isdir(save_path):
        save_path = os.path.join(save_path,"augmenatation_mapper.json")

    with open(save_path, 'w') as f:
        json.dump(augmenatation_mapper, f, indent=4)


