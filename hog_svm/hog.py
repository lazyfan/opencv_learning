import cv2
import numpy as np
import random
from non_maximum import non_max_suppression_fast as nms
import os

def gamma_trans(img, gamma):
    gamma_table = [np.power(x / 255.0, gamma) * 255.0 for x in range(256)]
    gamma_table = np.round(np.array(gamma_table)).astype(np.uint8)
    return cv2.LUT(img, gamma_table)


def load_images(dirname):
    img_list = []
    for name in os.listdir(dirname):
        path = dirname + name
        img = cv2.imread(path)
        img_list.append(img)
    return img_list


def extract_images(path, img_list, size, wsize=(80, 80)):
    """roi img"""
    extract_img = []

    return extract_img


def extract_neg_img(dirname, extract_neg, wsize=(80, 80)):
    x = 10
    xmin, xmax, ymin, ymax = 0, 0, 0, 0
    for i in range(15):
        path = dirname + str(i + 1) + '.jpg'
        img = cv2.imread(path)
        path = dirname
        for j in range(x):
            xmin = random.randint(1, 288 - wsize[0])
            ymin = random.randint(1, 352 - wsize[1])
            xmax = xmin + wsize[0]
            ymax = ymin + wsize[1]
            roi = img[xmin:xmax, ymin:ymax]
            extract_neg.append(roi)
    return extract_neg


def computeHOGs(img_list, gradient_list, hog):

    for img in img_list:
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        gray = cv2.equalizeHist(gray)
        gray = gamma_trans(gray, 0.8)
        gradient_list.append(hog.compute(gray))
        # print(type(hog.compute(gray)))
        hog_feat = hog.compute(gray)
        # print(hog_feat)
        # print(hog_feat.shape)

    return gradient_list


def sliding_window(image, stepSize, windowSize):
    for y in range(0, image.shape[0]-windowSize[1], stepSize):
        for x in range(0, image.shape[1]-windowSize[0], stepSize):
            yield (x, y, image[y:y + windowSize[1], x:x + windowSize[0]])


def resize(img, scaleFactor):
    return cv2.resize(img, (int(img.shape[1] * (1 / scaleFactor)), int(img.shape[0] * (1 / scaleFactor))),
                      interpolation=cv2.INTER_AREA)

def get_svm_detector(svm):
    '''
    导出可以用于cv2.HOGDescriptor()的SVM检测器，实质上是训练好的SVM的支持向量和rho参数组成的列表
    :param svm: 训练好的SVM分类器
    :return: SVM的支持向量和rho参数组成的列表，可用作cv2.HOGDescriptor()的SVM检测器
    '''
    sv = svm.getSupportVectors()
    rho, _, _ = svm.getDecisionFunction(0)
    sv = np.transpose(sv)
    return np.append(sv, [[-rho]], 0)

gradient_list = []
labels = []
path1 = 'new_pos/'
path2 = 'new_neg/'

def train_svm(hog_args, detection=1):
    # 读取HOG特征
    pos_list = load_images(path1)
    # pos_list = extract_images(path_pos, pos_list, 333, wsize=(80, 80))
    neg_list = load_images(path2)
    # neg_list = extract_images(path_neg, neg_list, 193, wsize=(80, 80))
    # neg_list = extract_neg_img(path2, neg_list, wsize=(80, 80))
    # neg_list = extract_neg_img(path2,wsize=(80,80))

    winSize, blockSize, blockStride, cellNum, binNum = hog_args
    hog = cv2.HOGDescriptor(winSize, blockSize, blockStride, cellNum, binNum)

    print('####计算正样本特征#####')
    computeHOGs(pos_list, gradient_list, hog)
    for _ in range(len(pos_list)):
        labels.append(+1)
    print('####计算负样本特征#####')
    computeHOGs(neg_list, gradient_list, hog)
    for _ in range(len(neg_list)):
        labels.append(-1)
    # 训练svm
    print('####开始训练#####')
    svm = cv2.ml.SVM_create()
    # svm.setCoef0(0.0)
    # # criteria = (cv2.TERM_CRITERIA_MAX_ITER + cv2.TERM_CRITERIA_EPS, 1000, 1e-3)
    # # svm.setTermCriteria(criteria)
    # svm.setType(cv2.ml.SVM_C_SVC)
    # svm.setGamma(0.001)
    # svm.setC(30)
    # svm.setKernel(cv2.ml.SVM_RBF)
    svm.setCoef0(0.0)
    svm.setDegree(3)
    criteria = (cv2.TERM_CRITERIA_MAX_ITER + cv2.TERM_CRITERIA_EPS, 1000, 1e-3)
    svm.setTermCriteria(criteria)
    svm.setGamma(0)
    svm.setKernel(cv2.ml.SVM_LINEAR)
    svm.setNu(0.5)
    svm.setP(0.1)  # for EPSILON_SVR, epsilon in loss function?
    svm.setC(0.01)  # From paper, soft classifier
    svm.setType(cv2.ml.SVM_EPS_SVR)  # C_SVC # EPSILON_SVR # may be also NU_SVR # do regression task
    # train_data = np.array(gradient_list).astype('float32')
    svm.train(np.array(gradient_list), cv2.ml.ROW_SAMPLE, np.array(labels))

    # hog.setSVMDetector(svm)
    # hog.save('hog_svm.xml')
    svm.save("svm.xml")
    print('####训练完成#####')

    if detection:
        svm_detector = get_svm_detector(svm)
        hog.setSVMDetector(svm_detector)
        hog.save('hogDetector.xml')



import time

def test_detection2(img, hog_args, my_svm):
    img = cv2.resize(img, (320, 180))
    rectangles = []
    counter = 0
    scale = 1

    winSize, blockSize, blockStride, cellNum, binNum = hog_args
    hog = cv2.HOGDescriptor(winSize, blockSize, blockStride, cellNum, binNum)
    w, h = winSize
    print('####开始检测#####')
    start = time.time()
    for (x, y, roi) in sliding_window(img, 5, winSize):  # 对得到的图进行滑动窗口，(100, 40)为窗口大小，本文应取(64, 64)
        if roi.shape[1] != w or roi.shape[0] != h:  # 判断是否超纲
            continue
        gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
        gray = cv2.equalizeHist(gray)
        gray = gamma_trans(gray, 0.8)

        test_gradient = hog.compute(gray)
        _, result = my_svm.predict(np.array([test_gradient]))
        a, res = my_svm.predict(np.array([test_gradient]), flags=cv2.ml.STAT_MODEL_RAW_OUTPUT)
        score = res[0][0]
        # print('score: %f,label: %d'%(score,result[0][0]))
        if int(result[0][0]) == 1:
            if score > 1.0:
                # print(score)
                rx, ry, rx2, ry2 = int(x * scale), int(y * scale), int((x + w) * scale), int((y + h) * scale)
                rectangles.append([rx, ry, rx2, ry2, score])
        counter += 1
    print('time use: %.4f'%(time.time()-start))
    # print(counter)
    boxes = np.array(rectangles)
    # print(windows.shape)
    boxes = nms(boxes, 0.5)
    for (x, y, x2, y2, score) in boxes:
        cv2.rectangle(img, (int(x), int(y)), (int(x2), int(y2)), (0, 255, 0), 2)
        cv2.putText(img, "%f" % score, (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1, (0, 255, 0))
    cv2.imshow("img", img)
    cv2.waitKey()


def test_detection1(img, hog):
    img = cv2.resize(img, (320, 180))
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gray = cv2.equalizeHist(gray)
    gray = gamma_trans(gray, 0.8)
    t1 = time.time()
    rects, foundWeights = hog.detectMultiScale(gray, winStride=(5, 5), padding=(10, 10), scale=1.)
    print('time use: %.4f' % (time.time() - t1))
    for b, c in zip(rects,foundWeights):
        cv2.rectangle(img,(b[0], b[1]),(b[0]+b[2],b[1]+b[3]),(0,0,255),2)
        cv2.putText(img, "%f" % c, (int(b[0]), int(b[1])), cv2.FONT_HERSHEY_PLAIN, 1, (0, 0, 255))

    cv2.imshow('img2', img)
    cv2.waitKey()


def test_classfication(img, hog_args, my_svm):

    winSize, blockSize, blockStride, cellNum, binNum = hog_args
    img = cv2.resize(img, winSize)

    hog = cv2.HOGDescriptor(winSize, blockSize, blockStride, cellNum, binNum)
    w, h = winSize

    if img.shape[1] != w or img.shape[0] != h:  # 判断是否超纲
        print('')
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gray = cv2.equalizeHist(gray)
    gray = gamma_trans(gray, 0.8)
    test_gradient = hog.compute(gray)
    _, result = my_svm.predict(np.array([test_gradient]))
    a, res = my_svm.predict(np.array([test_gradient]), flags=cv2.ml.STAT_MODEL_RAW_OUTPUT)
    score = res[0][0]
    print(score)
    if result[0][0] > 0.5:
        print('pos')
    elif result[0][0] < -0.5:
        print('neg')


if __name__ == '__main__':

    detection = 1

    """(450,350),(50,50),(10,10),(10,10),9"""
    """(216,168), (24,24), (8,8), (8,8), 9"""
    """(64,128), (16,16), (8,8), (8,8), 9"""
    hog_args = [(216, 168), (24, 24), (8, 8), (8, 8), 9]  # 官方默认

    train, test = [1, 1]
    if train:
        train_svm(hog_args, detection)


    if test:
        print('####读取图片#####')
        img = cv2.imread('org_img/img-00234.jpg')
        if detection:
            hog = cv2.HOGDescriptor().load('hogDetector.xml')
            test_detection1(img, hog)
        else:
            my_svm = cv2.ml.SVM_load("svm.xml")
            test_classfication(img, hog_args, my_svm)
