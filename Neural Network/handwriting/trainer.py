import numpy as np
from os import listdir
from sklearn.metrics import accuracy_score
from sklearn.neural_network import MLPClassifier
import joblib

def readFile(fileName):
    mat = np.zeros([1024], int)
    img = open(fileName)
    lines = img.readlines()
    for i in range(0, 32):
        for j in range(0, 32):
            mat[32 * i + j] = lines[i][j]
    return mat

def readData(folder):
    fileList = listdir(folder)
    fileNum = len(fileList)
    dataSet = np.zeros([fileNum, 1024], int)
    labelSet = np.zeros([fileNum, 10])
    for i in range(0, fileNum):
        fileName = fileList[i]
        dataSet[i] = readFile(folder + '/' + fileName)
        label = int(fileName.split('_')[0])
        labelSet[i][label] = 1.0
    return dataSet, labelSet

def turnValue(vector):
    digit = 0
    for i in range(0, 10):
        if ( abs(vector[i] - 1.0) < abs(vector[digit] - 1.0) ):
            digit = i
    if(abs(vector[digit] - 1.0) > 0.9):
        return -1
    return digit

def turnArray(arr):
    n = len(arr)
    result = np.zeros([n])
    for i in range(0, n):
        result[i] = turnValue(arr[i])
    return result

train_data, train_label = readData('trainingDigits')
classifier = MLPClassifier(hidden_layer_sizes=(100,), activation='logistic', solver='adam', learning_rate="constant", learning_rate_init=0.0001, max_iter=40000)
classifier.fit(train_data, train_label)

test_data, test_label = readData('testDigits')
train_predict = classifier.predict(train_data)
predicts = classifier.predict(test_data)
print('训练集准确率：', accuracy_score(turnArray(train_label), turnArray(train_predict)) )
print('测试集准确率：', accuracy_score(turnArray(test_label), turnArray(predicts)) )

file = 'model.joblib'
joblib.dump(classifier, file)