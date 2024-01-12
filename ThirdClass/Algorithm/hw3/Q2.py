def job_schdule(userList, processList, processorList, currCost, minCost, assignment): # minCost has to be inf and ssigment has to be empty set

  if len(assignment) == len(userList):  # All assignments complete
    return min(minCost, currCost)

  for processor in processorList:
    for user in userList:
      for process in processList:
        
        if (user, process) not in assignment:  # Check if user-process pair is unassigned
          updated_assignment = set(assignment)
          updated_assignment.add((user, process, processor)) # Assign user-process pair to processor
          
          updated_cost = currCost + cost_of_assignment(user, process, processor)  # Update cost
          
          minCost = job_schdule(userList, processList, processorList, updated_cost, minCost, updated_assignment)  # Recurse with updated state
          
          assignment.remove((user, process, processor))  # Unassign for after exploring in here

  return minCost, assignment  # Return minimum cost and final assignment

# Usage example
sset = set()
floatInf = 999999999.99
schedule, cost = job_schdule(userList, processList, processorList,0, floatInf, sset)
print("Optimal schedule:", schedule)
print("Minimum cost:", cost)
