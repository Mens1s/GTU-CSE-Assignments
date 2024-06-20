
def f(x):
    a = [2,1,2,-2,1,-1,1]
    return a[x]

def findMaxTotalArea(areaSize):
 
    result = -99999 - 1
    tempValufOfMax = 0
 
    for i in range(0, areaSize):
        tempValufOfMax = tempValufOfMax + f(i)
        
        result = max(result, tempValufOfMax)
 
        if tempValufOfMax < 0:
            tempValufOfMax = 0
    return result
 
 
print ("Maximum contiguous sum is", findMaxTotalArea(7))