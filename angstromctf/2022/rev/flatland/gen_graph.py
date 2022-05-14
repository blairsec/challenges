import random

flag = b"actf{Fl4TmAn_rouNdw0R1D}"
deet = list(flag)
random.shuffle(deet)
# edges = [[deet.index(flag[flag.index(c) + 1])] if c != ord("}") else [] for c in deet]
sflag = b"NfTRcD1ontrw}4{mFl_Ad0ua"
edges = [20, 14, 15, 6, 9, 12, 5, 22, 18, 1, 7, 21, 10, 2, 16, 19, 17, 13, 10, 8, 11, 3, 0, 4]
edges2 = [17, 10, 0, 22, 14, 4, 5, 10, 15, 15, 18, 7, 16, 14, 19, 2, 9, 0, 8, 17, 22, 12, 2, 19]

# A class to represent a graph object
class Graph:
 
    # Constructor
    def __init__(self, edges, n):
 
        # A list of lists to represent an adjacency list
        self.adjList = [[] for _ in range(n)]
 
        # add edges to the undirected graph
        for (src, dest) in edges:
            if src == -1 or dest == -1:
                continue
            self.adjList[src].append(dest)
            self.adjList[dest].append(src)
 
 
def hamiltonianPaths(graph, v, visited, path, n):
 
    # if all the vertices are visited, then the Hamiltonian path exists
    if len(path) == n:
        if bytes(sflag[x] for x in path[:5]) != b"actf{" or sflag[path[-1]] != ord("}"):
            # return
            pass
        # print the Hamiltonian path
        x = bytes(sflag[x] for x in path)
        # if x != flag:
        if True:
            print(x)
        return
 
    # Check if every edge starting from vertex `v` leads to a solution or not
    for w in graph.adjList[v]:
 
        # process only unvisited vertices as the Hamiltonian
        # path visit each vertex exactly once
        if not visited[w]:
            visited[w] = True
            path.append(w)
 
            # check if adding vertex `w` to the path leads to the solution or not
            hamiltonianPaths(graph, w, visited, path, n)
 
            # backtrack
            visited[w] = False
            path.pop()
 
 
def findHamiltonianPaths(graph, n):
 
    # start with every node
    for start in range(n):
        if sflag[start] != ord("a"):
            continue
 
        # add starting node to the path
        path = [start]
    
        # mark the start node as visited
        visited = [False] * n
        visited[start] = True
    
        hamiltonianPaths(graph, start, visited, path, n)

g = Graph([(i, edges[i]) for (i, c) in enumerate(sflag)] + [(i, edges2[i]) for (i, c) in enumerate(sflag)], len(sflag))
findHamiltonianPaths(g, len(sflag))
