def findBrighetstLocation(grid):
    brightestLocations = []
    
    for y in range(1, len(grid)-1):
        for x in range(1,len(grid[0])-1):
            if grid[y][x] > grid[y-1][x] and grid[y][x] > grid[y+1][x] and grid[y][x] > grid[y][x-1] and grid[y][x] > grid[y][x+1]:
                brightestLocations.append((y,x))
    
    if len(brightestLocations) > 0:
        return brightestLocations
    # teaching assitant said that we should search if we cannot find brightest location in center of array, we have to search
    # for brightest location in the first and last row also in the first and last column
    # seach for brightest location in the first and last row also in the first and last column
    for x in range(1,len(grid[0])-1):
        if grid[0][x] > grid[0][x-1] and grid[0][x] > grid[0][x+1] and grid[0][x] > grid[1][x]:
            brightestLocations.append((0,x))
        if grid[len(grid)-1][x] > grid[len(grid)-1][x-1] and grid[len(grid)-1][x] > grid[len(grid)-1][x+1] and grid[len(grid)-1][x] > grid[len(grid)-2][x]:
            brightestLocations.append((len(grid)-1,x))
    for y in range(1, len(grid)-1):
        if grid[y][0] > grid[y-1][0] and grid[y][0] > grid[y+1][0] and grid[y][0] > grid[y][1]:
            brightestLocations.append((y,0))
        if grid[y][len(grid[0])-1] > grid[y-1][len(grid[0])-1] and grid[y][len(grid[0])-1] > grid[y+1][len(grid[0])-1] and grid[y][len(grid[0])-1] > grid[y][len(grid[0])-2]:
            brightestLocations.append((y,len(grid[0])-1))
            
    # search for brightest location in the corners sol ust
    if grid[0][0] > grid[0][1] and grid[0][0] > grid[1][0]:
        if grid[0][1] < grid[0][2]:
            brightestLocations.append((0,0))
        elif grid[0][1] < grid[0][2]:
            brightestLocations.append((0,0))

    # sag ust
    if grid[0][len(grid[0])-1] > grid[0][len(grid[0])-2] and grid[0][len(grid[0])-1] > grid[1][len(grid[0])-1]:
        if grid[0][len(grid[0])-2] < grid[0][len(grid[0])-3]:
            brightestLocations.append((0,len(grid[0])-1))
        elif grid[0][len(grid[1])-1] > grid[2][len(grid[0])-1]:
            brightestLocations.append((0,len(grid[0])-1))

    # sol alt
    if grid[len(grid)-1][0] > grid[len(grid)-1][1] and grid[len(grid)-1][0] > grid[len(grid)-2][0]:
        if grid[len(grid)-2][0] < grid[len(grid)-3][0]:
            brightestLocations.append((len(grid)-1,0))
        elif grid[len(grid)-1][1] < grid[len(grid)-1][2]:
            brightestLocations.append((len(grid)-1,0))
            
    # sag alt
    if grid[len(grid)-1][len(grid[0])-1] > grid[len(grid)-1][len(grid[0])-2] and grid[len(grid)-1][len(grid[0])-1] > grid[len(grid)-2][len(grid[0])-1]:
        if grid[len(grid)-2][len(grid[0])-1] < grid[len(grid)-3][len(grid[0])-1]:
            brightestLocations.append((len(grid)-1,len(grid[0])-1))
        elif grid[len(grid)-1][len(grid[0])-2] < grid[len(grid)-1][len(grid[0])-3]:
            brightestLocations.append((len(grid)-1,len(grid[0])-1))
    
    return brightestLocations

#test case
grid = [[1,2,3,4,5,6,7,8,9,10],
        [2,3,4,5,6,7,8,9,10,11],
        [3,4,5,6,7,8,9,10,11,12],
        [4,5,6,7,8,9,10,11,12,13],
        [5,6,7,8,9,10,11,12,13,14],
        [6,7,8,9,10,11,12,13,14,15],
        [7,8,9,10,11,12,13,14,15,16],
        [8,9,10,11,12,13,14,15,16,17],
        [9,10,11,12,13,14,15,16,17,18],
        [10,11,12,13,14,15,16,17,18,19]]
print(findBrighetstLocation(grid))

grid = [
        [17, 18, 17, 16, 15, 14, 13, 12, 11, 10],
        [18, 17, 16, 15, 14, 13, 12, 11, 10, 9],
        [17, 16, 15, 14, 13, 12, 11, 10, 9, 8],
        [16, 15, 14, 13, 12, 11, 10, 9, 8, 7],
        [15, 14, 13, 12, 11, 10, 9, 8, 7, 6],
        [14, 13, 12, 11, 10, 9, 8, 7, 6, 5],
        [13, 12, 11, 10, 9, 8, 7, 6, 5, 4],
        [12, 11, 10, 9, 8, 7, 6, 5, 4, 3],
        [11, 10, 9, 8, 7, 6, 5, 4, 3, 2],
        [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
    ]
for a in grid:
    print(a)
    
print(findBrighetstLocation(grid))
    
                
    