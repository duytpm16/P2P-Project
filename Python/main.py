import argparse
import random
from graph import Graph


def main(file, d, r, seed):
    random.seed(12345)
    G = Graph(file, d, r)

    #print("Greedy Routing Results")
    #print(G.dijkstra_shortest_path())
    #print(G.greedy_routing())
    #print(G.multi_greedy_routing(5))
    
    print("Broadcast Results")
    print(G.flood_summary())
    print(G.broadcast_summary())

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--degree', type=int, default=8)
    parser.add_argument('-f', '--file', type=str)
    parser.add_argument('-r', '--r', type=float, default=0.25)
    parser.add_argument('-s', '--seed', type=int, default=12345)
    

    args = parser.parse_args()
    main(args.file, args.degree, args.r, args.seed)
