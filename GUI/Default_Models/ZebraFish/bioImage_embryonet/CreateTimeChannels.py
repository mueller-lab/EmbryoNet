import cv2
import numpy  as np 
def main():


  
    img = cv2.imread("Boom_LO350.jpg", cv2.IMREAD_COLOR)
    assert img.shape[2] == 3, img.shape[2]

    
    n = 581.0/1200  
    
    time_for_conv = np.ones(shape=(1, 1, 224, 224), dtype=np.float32) * n


    time_for_fc = np.ones(shape=(1,1), dtype=np.float32) * n 
    image = cv2.resize(img, (224, 224))
    image = np.transpose(image, axes=(2,0,1))
    image = np.float32(image)
    image.shape = (1,) + image.shape

    np.save("input_test_time_for_conv.npy", time_for_conv)
    np.save("input_test_time_for_fc.npy", time_for_fc)
    np.save("input_test_image.npy", image )
    
    
    
    
if (__name__=="__main__"): 
   main()
   