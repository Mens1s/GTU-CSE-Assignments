# Graph is represented as a dictionary of lists / geekforgeeks.org
from collections import defaultdict

class Graph:
    def __init__(self):
        self.graph = defaultdict(list)

    def addEdge(self, u, v, latency):
        self.graph[u].append((v, latency))
# End of Graph class

def exhaustive_search(graph, source, dest):
    # DFS search    
    def DFSUtil(currNode, currWayLen, currPath, visited, minWayGraph, minWay = 999999):
        
        if currNode == dest: # base case for recursion
            if currWayLen < minWay:
                minWay = currWayLen
                minWayGraph[:] = currPath[:] # copy currPath to minWayGraph (list) because minWayGraph is reference type and we collect it on dfs function
            return
        
        visited.add(currNode)

        for neighbor, expr in graph.graph[currNode]:
            if neighbor not in visited:
                DFSUtil(neighbor, currWayLen + expr, currPath + [neighbor], visited, minWayGraph, minWay)
                
        visited.remove(currNode)
        
    def dfs(currNode, currWayLen, currPath):
        minWayGraph = []
        visited = set()
        DFSUtil(currNode, currWayLen, currPath, visited, minWayGraph)
        return minWayGraph
        

    minWayGraph = dfs(source, 0, [source])
    
    return minWayGraph


testData = Graph()
testData.addEdge('A', 'B', 5)
testData.addEdge('A', 'C', 3)
testData.addEdge('B', 'D', 6)
testData.addEdge('C', 'D', 7)
testData.addEdge('C', 'E', 8)
testData.addEdge('D', 'E', 9)

source_node = 'A'
destination_node = 'E'

result = exhaustive_search(testData, source_node, destination_node)
if result:
    print(result)
else:
    print(f"No path found between {source_node} and {destination_node}")
