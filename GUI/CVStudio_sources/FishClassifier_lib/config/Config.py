import json

config = {}

config['Source'] = {}
config['Algorithm'] = {}
config['ResultRepresenter'] = []

config['classes'] = []
config['classes'].append(
{
    "id":0,
    "name":"BMP",
    "color": [255,0,0],
})
config['classes'].append(
{
    "id":1,
    "name":"NODAL",
    "color": [0,0,255],
})
config['classes'].append(
{
    "id":2,
    "name":"NORMAL",
    "color": [0,255,0],
})
config['classes'].append(
{
    "id":3,
    "name":"UNKNOWN",
    "color": [0,0,0],
})
config['classes'].append(
{
    "id":4,
    "name":"CUT",
    "color": [255,255,255],
})
config['classes'].append(
{
    "id":5,
    "name":"BOOM",
    "color": [255,0,255],
})

source_config = config['Source']
algorithm_config = config['Algorithm']

source_config['type'] = 'Path'
source_config['folder'] = 'F:/Test/imagesimages/'

#config['ResultRepresenter'].append(
#{
#    'type': 'json',
#    'folder': 'C:/test/image/json/'
#}

config['ResultRepresenter'].append(
{
    'type': 'labeler',
    'experiment_folder': 'F:/Test',
    'classes': config['classes'],
}

)

#config['ResultRepresenter'].append(
#{
    #'type': 'imwrite',
    #'prefix': 'output_',
    #'folder': 'C:/test/image/image/',
#}
#)

config['ResultRepresenter'].append(
{
    'type': 'imshow',
    'name': 'result',
    'width': 1024,
    'height': 1024,
    'waitKey': 10
}
)


algorithm_config['type'] = 'EmbryoClassifer'

algorithm_config['classifier'] = {}
classifier_config = algorithm_config['classifier']
classifier_config["path_to_net"] = "F:/Configs/EmbryoClassifier_v_1_GPU.onnx"
classifier_config["name"] = "Fish"
classifier_config["type"] = "OpenCV"
classifier_config["engine"] = "ONNX"
classifier_config["run"] = False
classifier_config["outputClasses"] = 5
classifier_config["height"] = 224
classifier_config["width"] = 224
classifier_config["minus"] = [0.485, 0.456, 0.406]
classifier_config["divide"] = [0.229, 0.224, 0.225]

algorithm_config['tracker'] = {}
tracker_config = algorithm_config['tracker']
tracker_config['detector'] = {}
detector_config = tracker_config['detector']
detector_config['name'] = 'FishDetector'
detector_config['engine'] = 'CPU'
detector_config['fx'] = 0.25
detector_config['fy'] = 0.25

detector_config['hough'] = {}
hough_config = detector_config['hough']
hough_config['method'] = "HOUGH_GRADIENT"
hough_config['dp'] = 1
hough_config['minDist'] = 20
hough_config['param1'] = 50
hough_config['param2'] = 30
hough_config['minRadius'] = 40
hough_config['maxRadius'] = 55

algorithm_config['logic'] = {}
logic_config = algorithm_config['logic']
logic_config['transactions'] = []
logic_transactions = logic_config['transactions']

UNKNOWN = 'UNKNOWN'
BMP = "BMP"
NODAL = "NODAL"
NORMAL ="NORMAL"
BOOM = "BOOM"
logic_transactions.append([UNKNOWN, BMP, BOOM])
logic_transactions.append([UNKNOWN, NODAL,BOOM]);
logic_transactions.append([UNKNOWN, BOOM]);
logic_transactions.append([UNKNOWN, NORMAL]);



with open('fish_config.json', 'w') as f:
    f.write(json.dumps(config, indent = 4))
