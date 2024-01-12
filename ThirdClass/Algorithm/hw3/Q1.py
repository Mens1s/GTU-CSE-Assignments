def highestDiscount(storeList, currIdx, currSet, maxx, result):
    
    if currIdx == len(storeList): # Base case: reached end of list
        current_discount = calc_discount(currSet) # Calculate discount for current set
        
        if current_discount > maxx[0]:
            result[0] = set(currSet)
            maxx[0] = current_discount  
    
    else:
        currSet.append(storeList[currIdx]) # append the current store
        highestDiscount(storeList, currIdx + 1, currSet, maxx, result)

        currSet.pop() # pop first element
        highestDiscount(storeList, currIdx + 1, currSet, maxx, result) 

# Example usage
storeList = ["Mens1sStore", "StoreOfMY", "DeathStore"]

maxx = [0]  
result = {} # Using a list/set/dict to pass by reference as C language

highestDiscount(storeList, 0, [], maxx, result)
print("Maximum discount:", maxx[0])
print("Stores to visit:", result[0])