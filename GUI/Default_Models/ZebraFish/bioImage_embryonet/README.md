# EmbryoNet models 

To test our model, first please install <a href="https://github.com/bioimage-io/core-bioimage-io-python">bioimageIO toolkit</a>, and then run the following commands

1) ```cd``` to ```EmbryoNet/GUI/Default_Models/ZebraFish/bioImage_embryonet```
2) ```bioimageio validate rdf.yaml```
3) ```bioimageio test-model rdf.yaml```
4) ```bioimageio predict-image rdf.yaml --inputs input_test_image.npy --inputs input_test_time_for_conv.npy  --inputs input_test_time_for_fc.npy --outputs outputResutl.npy```

More information about models could be found at our website <a href="https://embryonet.de">embryonet.de</a>.<br>
Source code is available at https://github.com/mueller-lab/EmbryoNet.
