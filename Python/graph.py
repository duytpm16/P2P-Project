import sys
import math
import networkx as nx
import random
import itertools
from scipy.spatial import distance
from routing import Route
from broadcast import Broadcast

class Node:
    def __init__(self, node_id, *coordinates):
        self.coordinates = coordinates
        self.id = node_id
        self.eucl_dist = {}
        self.cheb_dist = {}
        self.routing_edges = set()
        self.routing_neighbors = {}
        self.broadcast_edges = set()
        self.broadcast_neighbors = {}
        self.north = None
        self.south = None
        self.west = None
        self.east = None
        self.x = coordinates[0]
        self.y = coordinates[1]

    def euclidean_distance(self, v):
        if v not in self.eucl_dist:
            self.eucl_dist[v] = v.eucl_dist[self] = distance.euclidean(
                self.coordinates, v.coordinates
            )
        return self.eucl_dist[v]

    def chebyshev_distance(self, v):
        if v not in self.cheb_dist:
            self.cheb_dist[v] = v.cheb_dist[self] = distance.chebyshev(
                self.coordinates, v.coordinates
            )
        return self.cheb_dist[v]

    def create_edge(self, v, i):
        self.routing_edges.add(v)
        self.routing_neighbors[i] += 1
        v.routing_edges.add(self)
        v.routing_neighbors[i] += 1
        return

    def create_broadcast_edge(self, v, i, j):
        if ((i, j) not in self.broadcast_neighbors):
            self.broadcast_neighbors[(i, j)] = []
        if ((i, j) not in v.broadcast_neighbors):
            v.broadcast_neighbors[(i, j)] = []

        self.broadcast_edges.add(v)
        self.broadcast_neighbors[(i, j)].append(v)
        v.broadcast_edges.add(self)
        v.broadcast_neighbors[(i, j)].append(self)
        return
        
    def __repr__(self):
        return f"{self.id}:{self.coordinates}"
      

class Graph(Route, Broadcast):
    def __init__(self, file_name, d, r):
        self.d = d
        self.r = r
        labels = {}
        self.n = 0
        Route.__init__(self)
        Broadcast.__init__(self)
        
        with open(file_name, "r") as f:
            for i, line in enumerate(f):
                labels[i] = Node(i, *(float(x) for x in line.split()))
                self.n += 1
        self.k = math.floor(math.log(self.n, 2))

        # G
        print("Creating graph G...")
        self.G = nx.random_regular_graph(d, self.n, seed = 12345)
        nx.relabel_nodes(self.G, labels, copy=False)
        for u, v in self.G.edges:
            self.G[u][v]["weight"] = u.euclidean_distance(v)


        # G MST
        self.GMST = nx.minimum_spanning_tree(self.G)

        # G*
        print("Creating graph G*...")
        for u, i in itertools.product(self.G.nodes(), range(0, self.k)):
            u.routing_neighbors[i] = 0
        self.subgraphs(self.G.nodes())

        # Broadcast graph
        print("Create broadcast graph G*...")
        for u in self.G.nodes():
            for i, j in itertools.product(range(0, self.k), range(0, 4)):
                u.broadcast_neighbors[(i,j)] = []
        self.B = nx.empty_graph(self.n)
        self.create_broadcast_graph()
        self.BMST = nx.minimum_spanning_tree(self.B)

    def euclidean_distance(self, u, v):
        return u.euclidean_distance(v)

    def chebyshev_distance(self, u, v):
        return u.chebyshev_distance(v)

    def subgraphs(self, nodes):
        for u, i in itertools.product(nodes, range(0, self.k)):
            nearby = {
                v for v in nodes if u != v and v not in u.routing_edges and v.routing_neighbors[i] < self.k and u.chebyshev_distance(v) < self.r**i
            }
            for v in random.sample(nearby, min(self.k - u.routing_neighbors[i], len(nearby))):
                u.create_edge(v, i)
