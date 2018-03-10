# -*- coding: utf-8 -*-
"""
Created on Sat Mar 10 09:52:19 2018

@author: cclee
"""

import cv2
import numpy as np
import sys, getopt        
    
def process_video(filename):
    cap = cv2.VideoCapture(video_filename)
    bOpenVideo = cap.isOpened()
    print('Open Video: {0} '.format(bOpenVideo))
    if bOpenVideo == False:
        return  
    
    fps = cap.get(cv2.CAP_PROP_FPS)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    while True:
        bVideoRead, frame = cap.read()  
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    
def main():
    print('len(sys.argv):', len(sys.argv))
    # fix random seed for reproducibility
#    np.random.seed(7)    
#    training()
#    positive_histogram()
#    video_clip()
#    fix_LR()
#    label_training_data(sigma = 1.5)
#    generate_feature()
#    
    try:
        opts, args = getopt.getopt(sys.argv[1:], "1234")
    except getopt.GetoptError as err:
        # print help information and exit:
        print( str(err))
        print('detect.py -1 or detect.py -2')             
        return 2

    for o, a in opts:
        if o == "-1":
            print('process one_lightvideo640-mkv.mkv ...')
            process_video('one_lightvideo640-mkv.mkv');
        elif o == '-2':
            print('process video640-mkv.mkv...')
            process_video('video640-mkv.mkv')
        else:
            return 0
        
if __name__ == "__main__":
    main()