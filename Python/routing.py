import math
import random
import itertools
import resource
import sys
import networkx as nx
import multiprocessing as mp

class Route:
    def __init__(self):
        None

    def closest(self, v, edges, visited):
        min_dist = math.inf
        min_node = None
    
        for node in edges:
            dist = node.euclidean_distance(v)
            if node not in visited and dist < min_dist:
                min_node = node
                min_dist = dist
        return min_node
            
    def route(self, u, v):
        prev = curr = u
        hops = stretch = 0
        visited = {u}

        while curr != v:
            prev = curr
            curr = self.closest(v, prev.routing_edges, visited)

            visited.add(curr)
            stretch += prev.euclidean_distance(curr)
            hops += 1
        return hops, stretch, visited


    def dijkstra_shortest_path(self):
        routes = 0
        avg_hops = 0
        max_hops = 0
        avg_stretch = 0
        max_stretch = 0
        worse_hop_route = []
        worse_hop_nodes = ()
        worse_stretch_route = []
        worse_stretch_nodes = ()
        for u, v in itertools.permutations(self.G.nodes(), 2):
            shortest_path = nx.shortest_path(G = self.G, source = u, target = v, weight = 'weight', method='dijkstra')
            routes += 1
            hops    = len(shortest_path) - 1
            stretch = sum(shortest_path[i].euclidean_distance(shortest_path[i+1]) for i in range(0, hops))

            avg_hops += hops
            if hops > max_hops:
                max_hops = hops
                worse_hop_nodes = (u, v)
                worse_hop_route = shortest_path
            avg_stretch += stretch
            if stretch > max_stretch:
                max_stretch = stretch
                worse_stretch_nodes = (u, v)
                worse_stretch_route = shortest_path
            
        return([routes,
                avg_hops / routes,
                max_hops,
                avg_stretch / routes,
                max_stretch])

    def greedy_routing(self):
        routes = 0
        avg_hops = 0
        max_hops = 0
        avg_stretch = 0
        max_stretch = 0
        worse_hop_route = []
        worse_hop_nodes = ()
        worse_stretch_route = []
        worse_stretch_nodes = ()
        nd = 0
        for u, v in itertools.permutations(self.G.nodes(), 2):
            hops, stretch, shortest_path = self.route(u, v)
            if (hops > 20) :
                nd += 1

            routes += 1
            avg_hops += hops
            if hops > max_hops:
                max_hops = hops
                worse_hop_nodes = (u, v)
                worse_hop_route = shortest_path
            avg_stretch += stretch
            if stretch > max_stretch:
                max_stretch = stretch
                worse_stretch_nodes = (u, v)
                worse_stretch_route = shortest_path
        print(nd)
        return([routes,
                avg_hops / routes, 
                max_hops,
                avg_stretch / routes, 
                max_stretch])

    def greedy_routing_thread(self, i, t, q):
        start = math.ceil((self.n / t) * i)
        end = math.ceil((self.n / t) * (i + 1))
        end = end if end <= self.n else self.n

        print("Thread " + str(i) + " processing nodes between " + str(start) + " - " + str(end))

        routes = 0
        avg_hops = 0
        max_hops = 0
        avg_stretch = 0
        max_stretch = 0
        worse_hop_route = []
        worse_hop_nodes = ()
        worse_stretch_route = []
        worse_stretch_nodes = ()
        for u in list(self.G.nodes())[start:end]:
            for v in self.G.nodes():
                if u != v:
                    hops, stretch, shortest_path = self.route(u, v)

                    routes += 1
                    avg_hops += hops
                    if hops > max_hops:
                        max_hops = hops
                        worse_hop_nodes  = (u, v)
                        worse_hop_route = shortest_path
                    avg_stretch += stretch
                    if stretch > max_stretch:
                        max_stretch = stretch
                        worse_stretch_nodes  = (u, v)
                        worse_stretch_route = shortest_path

        print("Thread " + str(i) + " done.")
        q.put([routes, 
                avg_hops, 
                max_hops,
                avg_stretch, 
                max_stretch,
                worse_hop_nodes, 
                worse_hop_route,
                worse_stretch_nodes, 
                worse_stretch_route])

    def multi_greedy_routing(self, nthreads):
        max_rec = 0x100000
        resource.setrlimit(resource.RLIMIT_STACK, [0x100 * max_rec, resource.RLIM_INFINITY])
        sys.setrecursionlimit(max_rec)

        print("Available number of processors: " + str(mp.cpu_count()))
        nthreads = min(nthreads, mp.cpu_count())
        print("Using: " + str(nthreads))

        results = []
        processes = []
        q = mp.SimpleQueue()
        for i in range(0, nthreads):
            p = mp.Process(target = self.greedy_routing_thread, args = (i, nthreads, q))
            processes.append(p)
            p.start()

        for p in processes:
            result = q.get()
            results.append(result)
            
        for p in processes:
            p.join()

        routes = 0
        avg_hops = 0
        max_hops = 0
        avg_stretch = 0
        max_stretch = 0
        worse_hop_route = []
        worse_hop_nodes = ()
        worse_stretch_route = []
        worse_stretch_nodes = ()
        for i in range(0, len(results)):
            routes += results[i][0]
            avg_hops += results[i][1]
            if (results[i][2] > max_hops):
                max_hops = results[i][2]
                worse_hop_nodes = results[i][5]
                worse_hop_route = results[i][6]
            avg_stretch += results[i][3]
            if (results[i][4] > max_stretch):
                max_stretch = results[i][4]
                worse_stretch_nodes = results[i][7]
                worse_stretch_route = results[i][8]
    
        return [routes, 
                avg_hops / routes, 
                max_hops,
                avg_stretch / routes, 
                max_stretch]
                # worse_hop_nodes, 
                # worse_hop_route,
                # worse_stretch_nodes, 
                # worse_stretch_route]
