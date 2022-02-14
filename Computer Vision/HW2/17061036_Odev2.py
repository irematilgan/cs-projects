import cv2
import os
import matplotlib.pyplot as plt
import numpy as np
from skimage import io
from tqdm import tqdm
import pandas as pd

def calculateHist(img,color_format):
    img = np.array(img)
    rows = len(img)
    cols = len(img[0])
    if color_format == 'hsv':
        hsv = cv2.cvtColor(img,cv2.COLOR_RGB2HSV)
        hue_hist = np.zeros((256),dtype = np.float32)
        for i in range(rows):
            for j in range(cols):
                hue_hist[img[i,j,0]] += 1
        hue_hist /= (rows*cols)
        return hue_hist
    elif(color_format == 'rgb'):
        r_hist = np.zeros((256),dtype = np.float32)
        g_hist = np.zeros((256),dtype = np.float32)
        b_hist = np.zeros((256),dtype = np.float32)
        
        hue_hist = np.zeros((1,256),dtype = np.float32)
        for i in range(rows):
            for j in range(cols):
                r_hist[img[i,j,0]] += 1
                g_hist[img[i,j,1]] += 1
                b_hist[img[i,j,2]] += 1
        
        r_hist /= (rows*cols)
        g_hist /= (rows*cols)
        b_hist /= (rows*cols)
        return [r_hist,g_hist,b_hist]

def calculateDist(hist1,hist2,mode):
    dist = 0
    hist1 = np.array(hist1)
    hist2 = np.array(hist2)
    if mode == 'hsv':
        dist = sum(pow(pow((hist1-hist2),2),0.5))
    elif mode == 'rgb':
        dist = sum(pow(pow((hist1[0,:]-hist2[0,:]),2) + pow((hist1[1,:]-hist2[1,:]),2) + pow((hist1[2,:]-hist2[2,:]),2),0.5))
    
    return dist

def findSimilar(img_hist,histograms,color_mode):
    similars = []
    for index,hist in enumerate(histograms): 
        dist = calculateDist(hist,img_hist,color_mode)
        similars.append(dist)
            
    return np.argsort(similars)[:5]

def calculateSuccess(images,tags,color_format,categories_dict):
    histograms = []
    randoms = []
    for tag in np.unique(tags):
        cat_images = np.where(tags == tag)[0]
        random_indexes = [np.random.randint(cat_images[0],cat_images[-1]) for i in range(30)]
        for index in random_indexes:
            try:
                hist = calculateHist(images[index],color_format)
                randoms.append(index)
                histograms.append(hist)
            except:
                pass
        

    j = 0
    total_success = 0
    for tag in np.unique(tags):
        print("SUCCESS OF ",categories_dict[tag])
        print("==========")
        success = 0
        for i in range(30):
            temp = histograms[:j+i]
            temp.extend(histograms[j+i+1:])
            try:
                similar_indexes = findSimilar(histograms[j+i],temp,color_format)
                for similar in similar_indexes:
                    if(tags[randoms[similar]] == tag):
                        success+=1
                        break
            except:
                pass
        j+=30
        print("{}%\n".format(success/30*100))
        total_success += success
    print("TOTAL SUCCESS = {:.2f} %".format(total_success/180*100))

def showImages(img_ref,images,similars_index):
    w=10
    h=10
    fig=plt.figure(figsize=(20, 20))
    columns = 6
    rows = 1
    fig.add_subplot(rows, columns, 1)
    plt.xlabel('Reference Image')
    plt.imshow(img_ref)
    for i in range(2, columns*rows+1):
        fig.add_subplot(rows, columns, i)
        plt.xlabel('Image{}'.format(i))
        plt.imshow(images[similars_index[i-2]])

def readImages(target_folder,category_name):
    y = []
    arr = []
    path = target_folder + category_name + "/"
    files = os.listdir(path)
    for file in files:
        arr.append(io.imread(path+file))
        y.append(file.split("_")[0])
        
    y = np.array(y)
    return arr,y

def testImages(images,color_format):
    histograms = []
    random_indexes = np.array([np.random.randint(0,len(images)) for i in range(50)])
    for index in random_indexes:
        try:
            hist = calculateHist(images[index],color_format)
            histograms.append(hist)
        except:
            pass
                
    for i in range(5):
        similars_index = findSimilar(histograms[i],histograms[5:],color_format)
        showImages(images[random_indexes[i]],images,random_indexes[similars_index])
    plt.show()

image_folder = os.getcwd()
image_folder = image_folder.replace("\\","/") + "/images/"
files = os.listdir(image_folder)
categories = dict()
for file in files:
    num,name = file.split(".")
    categories.update({
        num : name
    })

images = []
tags = []
for file in files:
    img,tag = readImages(image_folder,file)
    images.extend(img)
    tags.extend(tag)
    
images = np.array(images)
tags = np.array(tags)
print("====TESTING STARTED====")
testImages(images,'hsv')

print("\n====SUCCESS REPORT==== ")
calculateSuccess(images,tags,'hsv',categories)