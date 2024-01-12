def findFlawedFuse(fuses, left, right, NORMAL_CURRENT = 4):
    if left == right:
        if fuses[left] != NORMAL_CURRENT:
            return left  # Flawed fuse found
        else:
            return None  # No flaw in this segment
        
    mid = (left + right) // 2
    
    if fuses[mid] != NORMAL_CURRENT:
        return findFlawedFuse(fuses, left, mid)  # Flaw is in the first half or including mid
    else:
        return findFlawedFuse(fuses, mid + 1, right)  # Flaw is in the second half

def findFlawedFuse(fuses, NORMAL_CURRENT = 4):
    length = len(fuses)
    i = length - 1
    
    while i >= 0:
        if fuses[i] == NORMAL_CURRENT:
            if i == length - 1:
                return None
            return i + 2  # Flawed fuse found
        i -= 1
    return 1 

l = [4,4,4,4,4,4,0,0,0,0,0,0,0]
print(findFlawedFuse(l))

l = [0,0,0,0,0,0,0,0,0,0,0,0,0]
print(findFlawedFuse(l))

l = [4,4,4,4,4,4,4,4,4,4,4,4,0]
print(findFlawedFuse(l))