import tetris
import traceback
import math
import statistics
import operator
from multiprocessing import Pool
from pympler.tracker import SummaryTracker
tracker = SummaryTracker()
from mem_top import mem_top
import gc
import objgraph
import pickle
import time

class Point():
    def __init__(self,args:list, result):
        self.args = args
        self.result = result
        self.finished = False
        self.earlyExit = False
     
    def __str__(self):
        x = [str(s) for s in self.args]
        return f"({', '.join(x)}, {self.result})"

def newVal(lowerVal,higherVal,m, iterations, gradientMagnitude = 0.5):
    assert(higherVal != lowerVal)
    if m == 0:
        return (higherVal+lowerVal)/2
    adjustment = math.atan(m/(iterations/10))*gradientMagnitude*(abs(higherVal-lowerVal)**(1/3))
    return lowerVal - adjustment

def checkDeltas(p1:Point,p2:Point, delta):
    for x1,x2 in zip(p1.args,p2.args):
        if abs(x2-x1) > delta:
            return False
    return True

def gradient(x1,x2,y1,y2):
    return (y2-y1)/(x2-x1)


def searchLocalMin(p1:Point,p2:Point,function ,delta = 10e-6, max_iter = 10000):
    iter = 1
    deltas = []
    while iter < max_iter and not checkDeltas(p1,p2,delta):
        if (p1.result if p1.result < p2.result else p2.result) > -iter//50:
            p1.earlyExit, p2.earlyExit = True, True
            break
        if p1.result == p2.result:
            p2.args = [(x1+x2)/2 for x1,x2 in zip(p1.args,p2.args)]
        if p1.result < p2.result:
            p2.args = [newVal(x1,x2,gradient(x1,x2,p1.result,p2.result),iter) for x1,x2 in zip(p1.args,p2.args)]
            p2.result = function(p2.args)
        else:
            p1.args = [newVal(x2,x1,gradient(x1,x2,p1.result,p2.result),iter) for x1,x2 in zip(p1.args,p2.args)]
            p1.result = function(p1.args)
        time.sleep(0.001)
        iter += 1
    print(f"{p1 if p1.result < p2.result else p2}")
    p1.finished, p2.finished = True, True
    return p1 if p1.result < p2.result else p2

def calc(pairs):
    p1,p2 = pairs
    p1.result, p2.result = tetris.eval(p1.args), tetris.eval(p2.args)
    return searchLocalMin(p1,p2,tetris.eval)

def getGlobal(function, lowerBound, upperBound, points = 5, delta = 10e-6, multiPorcessing = True):
    assert(len(lowerBound) == len(upperBound))
    pointsPairs = []
    bestPoints = []
    batch = 50
    if isinstance(points,int):
        try:
            with open(f"saves/{function.__module__}_{function.__name__}_points",'rb') as file:
                pointsPairs = pickle.load(file)
            with open(f"saves/{function.__module__}_{function.__name__}_bestPoints",'rb') as file:
                bestPoints = pickle.load(file)
        except:
            pointsPairs = createPoints([points]*len(upperBound), lowerBound, upperBound)
        if multiPorcessing:
            for x in range(len(pointsPairs)//batch):
                print(f"Points left: {len(pointsPairs)}")
                with Pool() as pool:
                    _ = pointsPairs[0:batch]
                    pointsPairs = pointsPairs[batch:]
                    bestPoints += pool.map(calc, _)
                with open("tetris.log",'a') as file:
                    for y in range(batch):
                        file.write(f"{bestPoints[batch*x+y]};\n")
                time.sleep(1)
                gc.collect()
                with open(f"saves/{function.__module__}_{function.__name__}_points",'wb') as file:
                    pickle.dump(pointsPairs, file)
                with open(f"saves/{function.__module__}_{function.__name__}_bestPoints",'wb') as file:
                    pickle.dump(bestPoints, file)
            with Pool() as pool:
                bestPoints += pool.map(calc, pointsPairs)
            with open("tetris.log",'a') as file:
                for x in bestPoints[(len(pointsPairs)//batch)*batch:]:
                    file.write(f"{x};\n")
            with open(f"saves/{function.__module__}_{function.__name__}_points",'wb') as file:
                pickle.dump(pointsPairs, file)
            with open(f"saves/{function.__module__}_{function.__name__}_bestPoints",'wb') as file:
                pickle.dump(bestPoints, file)
        else:
            for p1, p2 in pointsPairs:
                p1.result = function(p1.args)
                p2.result = function(p2.args)
            [searchLocalMin(p1,p2,function,delta) for p1,p2 in pointsPairs]
    bestPoints.sort(key = operator.attrgetter('result'))
    return bestPoints
    for p in bestPoints: print(p)


def createPoints(points:list, lowerBound:list, upperBound:list, pointPairs = [(Point([],None), Point([],None))]):
    if len(points) == 0:
        return pointPairs
    l,u,p  = lowerBound.pop(0), upperBound.pop(0), points.pop(0)
    newPointPairs = []
    step = (u - l)/(p - 1)
    for s in range(p):
        for p1,p2 in pointPairs:
            arg1 = list(p1.args)
            arg1.append(l + step*s)
            arg2 = list(p2.args)
            arg2.append(l + step*(s+1))
            newPointPairs.append((Point(arg1,None),Point(arg2,None)))
    return createPoints(points, lowerBound, upperBound, newPointPairs)

def testFunction(optimizer, function,lowerBound,upperBound,delta = 10e-6):
    function = function()
    lowerBound, upperBound = function.domain()
    print(f"{function.__class__.__name__}: {optimizer(function.eval, lowerBound, upperBound,delta = delta)[0]}")

def plot3D(function,lowerBound,upperBound):
    from mpl_toolkits import mplot3d
    import numpy as np
    import matplotlib.pyplot as plt
    print(function.__name__)
    ax = plt.axes(projection='3d')
    x = np.linspace(lowerBound[0], upperBound[0])
    y = np.linspace(lowerBound[1], upperBound[1])
    z = []
    for x1 in x:
        t = []
        for y1 in y:
            t.append(function([x1,y1]))
        z.append(t)
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.contour3D(x, y, z, 50, cmap='binary')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    fig.show()
    input()

def f(p):
    x,y = p
    #return (1.5-x+x*y)**2 + (2.25-x+x*(y**2))**2 + (2.625 - x + x*(y**3))**2
    return x**2 + y**2

if __name__ == '__main__':
    #print(tetris.eval([3.4181268101392694,-3.3855972247263626,-7.899265427351652,0,-4.500158825082766,-3.2178882868487753,-9.348695305445199]))
    #exit()
    x = getGlobal(tetris.eval,[-10,-10,-10,-10,-10,-10,-10],[10,10,10,10,10,10,10])
    print(x[0])