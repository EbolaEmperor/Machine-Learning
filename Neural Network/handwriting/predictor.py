import joblib
import numpy as np

def readFile(fileName):
    mat = np.zeros([1, 1024], int)
    img = open(fileName)
    lines = img.readlines()
    for i in range(0, 32):
        for j in range(0, 32):
            mat[0][32 * i + j] = lines[i][j]
    return mat

def turnValue(vector):
    digit = 0
    for i in range(0, 10):
        if ( abs(vector[0][i] - 1.0) < abs(vector[0][digit] - 1.0) ):
            digit = i
    if(abs(vector[0][digit] - 1.0) > 0.9):
        return -2
    return digit

classifier = joblib.load('model.joblib')
test_data = readFile('test.txt')
fp = open('result.txt', 'w')
fp.write( str(turnValue(classifier.predict(test_data))) )