#def getListOfTasks(tasks):
#    result = []
#    while len(tasks) > 0:
#        minTask, maxTask = divideAndConquerResourceAllocation(tasks)
#        result.append([minTask, maxTask])
#        
#        if len(tasks) == 1:
#            result[-1] = [tasks[0], "None"]
#            tasks.remove(tasks[0])
#        else:    
#            tasks.remove(minTask)
#            tasks.remove(maxTask)
#        
#    return result
# if all task needed it will be done in O(n) time complexity so generally it will be O(n^2logn) time complexity
# output like [[-10, 10], [-9, 9], [-8, 8], [-7, 7], [-6, 6], [-5, 5], [-4, 4], [-3, 3], [-2, 2], [-1, 1]]
def getMinMaxUsingDivideAndConquer(tasks):
    if not tasks:
        return None
    
    if len(tasks) == 1:
        return tasks[0], tasks[0]
    
    mid = len(tasks) // 2
    
    lMin, lMax = getMinMaxUsingDivideAndConquer(tasks[:mid])   # for left side of array
    rMin, rMax = getMinMaxUsingDivideAndConquer(tasks[mid:]) # for rigth side of array
    
    minTask = min(lMin, rMin)  
    maxTask = max(lMax, rMax)  
    
    return minTask, maxTask

t = [1,2,3,4,5,6,7,8,9,10,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10]
print(getMinMaxUsingDivideAndConquer(t))
