import math
import random
import itertools

class Broadcast:
    def __init__(self):
        self.spatial_index = {}

    def get_intersection(self, left, right, bottom, top, nodes):
        intersection = self.spatial_index.get((left, right, bottom, top))
        if intersection is None:
            intersection = {
                v for v in nodes if v.x >= left and v.x <= right and v.y >= bottom and v.y <= top
            }
            if (len(intersection) == 0):
                return None
            self.spatial_index[(left, right, bottom, top)] = intersection
        return intersection

    def create_broadcast_graph(self):
        for u in self.G.nodes():
            self.B.add_node(u, pos=(u.x, u.y))
            xmid, ymid, xmin, xmax, ymin, ymax = 0, 0, 0, 1, 0, 1

            # Phase 1
            for i in range(0, 2):
                xmid   = (xmin + xmax) / 2
                ymid   = (ymin + ymax) / 2
                length = abs((xmax - xmin)) / 2

                left   = [xmid, xmid - length, xmid - length, xmid]
                right  = [xmid + length, xmid, xmid, xmid  + length]
                bottom = [ymid, ymid, ymid - length, ymid - length]
                top    = [ymid + length, ymid + length, ymid, ymid]

                for j in range(0, 4):
                    if u.x >= left[j] and u.x <= right[j] and u.y >= bottom[j] and u.y <= top[j]:
                        xmin = xmid if left[j]   == xmid else xmin
                        xmax = xmid if right[j]  == xmid else xmax
                        ymin = ymid if bottom[j] == ymid else ymin
                        ymax = ymid if top[j]    == ymid else ymax
                        
            intersection = {
                v for v in self.G.nodes() if v.x >= xmin and v.x <= xmax and v.y >= ymin+length and v.y <= ymax+length
            }
            u.north = random.sample(intersection, min(1, len(intersection)))[0] if len(intersection) > 0 else None
            if u.north is not None:
                u.create_broadcast_edge(u.north, 1, 0)

            intersection = {
                v for v in self.G.nodes() if v.x >= xmin and v.x <= xmax and v.y >= ymin-length and v.y <= ymax-length
            }
            u.south = random.sample(intersection, min(1, len(intersection)))[0] if len(intersection) > 0 else None
            if u.south is not None:
                u.create_broadcast_edge(u.south, 1, 0)

            intersection = {
                v for v in self.G.nodes() if v.x >= xmin-length and v.x <= xmax-length and v.y >= ymin and v.y <= ymax
            }
            u.west = random.sample(intersection, min(1, len(intersection)))[0] if len(intersection) > 0 else None
            if u.west is not None:
                u.create_broadcast_edge(u.west, 1, 0)

            intersection = {
                v for v in self.G.nodes() if v.x >= xmin+length and v.x <= xmax+length and v.y >= ymin and v.y <= ymax
            }
            u.east = random.sample(intersection, min(1, len(intersection)))[0] if len(intersection) > 0 else None
            if u.east is not None:
                u.create_broadcast_edge(u.east, 1, 0)

            # Phase 2
            for i in range(2, self.k + 1):
                xmid = (xmin + xmax) / 2
                ymid = (ymin + ymax) / 2
                length = abs((xmax - xmin)) / 2

                left   = [xmid, xmid - length, xmid - length, xmid]
                right  = [xmid + length, xmid, xmid, xmid  + length]
                bottom = [ymid, ymid, ymid - length, ymid - length]
                top    = [ymid + length, ymid + length, ymid, ymid]

                for j in range(0, 4):
                    if u.x >= left[j] and u.x <= right[j] and u.y >= bottom[j] and u.y <= top[j]:
                        xmin = xmid if left[j]   == xmid else xmin
                        xmax = xmid if right[j]  == xmid else xmax
                        ymin = ymid if bottom[j] == ymid else ymin
                        ymax = ymid if top[j]    == ymid else ymax 
                        continue

                    intersection = self.get_intersection(left[j], right[j], bottom[j], top[j], self.G.nodes)
                    if intersection is None:
                        continue
                    
                    intersection = {
                        v for v in intersection if u != v and v not in u.broadcast_edges
                    }
                    for v in random.sample(intersection, min(1, len(intersection))):
                        u.create_broadcast_edge(v, i, j)
                        self.B.add_edge(u, v, weight = u.euclidean_distance(v))
            
            # Phase 3
            xmid = (xmin + xmax) / 2
            ymid = (ymin + ymax) / 2
            length = abs((xmax - xmin)) / 2
            left   = [xmid, xmid - length, xmid - length, xmid]
            right  = [xmid + length, xmid, xmid, xmid  + length]
            bottom = [ymid, ymid, ymid - length, ymid - length]
            top    = [ymid + length, ymid + length, ymid, ymid]
            for j in range(0, 4):
                if u.x >= left[j] and u.x <= right[j] and u.y >= bottom[j] and u.y <= top[j]:
                    xmin = xmid if left[j]   == xmid else xmin
                    xmax = xmid if right[j]  == xmid else xmax
                    ymin = ymid if bottom[j] == ymid else ymin
                    ymax = ymid if top[j]    == ymid else ymax
                    intersection = {
                        v for v in self.G.nodes() if u != v and v not in u.broadcast_edges and v.x >= left[j] and v.x <= right[j] and v.y >= bottom[j] and v.y <= top[j]
                    }
                    for v in intersection:
                        u.create_broadcast_edge(v, self.k+1, j)
                        self.B.add_edge(u, v, weight = u.euclidean_distance(v))

            nearby = {
                v for v in self.G.nodes if u != v and v not in u.routing_edges and u.chebyshev_distance(v) < self.r**self.k
            }
            for v in nearby:
                u.create_broadcast_edge(v, self.k+2, j)
                self.B.add_edge(u, v, weight = u.euclidean_distance(v))

        return None


    def broadcast(self, u):
        n = 0
        dist  = 0
        seen  = {u}
        queue = []
        phase = []
        for k in range(2, self.k + 3):
            queue.append(u)
            phase.append(k)

        temp = u
        while temp.north is not None:
            seen.add(temp.north)
            for k in range(2, self.k + 3):
                queue.append(temp.north)
                phase.append(k)
            dist += temp.euclidean_distance(temp.north)
            n += 1
            temp = temp.north

            temp2 = temp
            while temp2.west is not None:
                seen.add(temp2.west)
                for k in range(2, self.k + 3):
                    queue.append(temp2.west)
                    phase.append(k)
                dist += temp2.euclidean_distance(temp2.west)
                n += 1
                temp2 = temp2.west

            temp2 = temp
            while temp2.east is not None:
                seen.add(temp2.east)
                for k in range(2, self.k + 3):
                    queue.append(temp2.east)
                    phase.append(k)
                dist += temp2.euclidean_distance(temp2.east)
                n += 1
                temp2 = temp2.east
        
        temp = u
        while temp.south is not None:
            seen.add(temp.south)
            for k in range(2, self.k + 3):
                queue.append(temp.south)
                phase.append(k)
            dist += temp.euclidean_distance(temp.south)
            n += 1
            temp = temp.south

            temp2 = temp
            while temp2.west is not None:
                seen.add(temp2.west)
                for k in range(2, self.k + 3):
                    queue.append(temp2.west)
                    phase.append(k)
                dist += temp2.euclidean_distance(temp2.west)
                n += 1
                temp2 = temp2.west

            temp2 = temp
            while temp2.east is not None:
                seen.add(temp2.east)
                for k in range(2, self.k + 3):
                    queue.append(temp2.east)
                    phase.append(k)
                dist += temp2.euclidean_distance(temp2.east)
                n += 1
                temp2 = temp2.east

        temp = u
        while temp.east is not None:
            seen.add(temp.east)
            for k in range(2, self.k + 3):
                queue.append(temp.east)
                phase.append(k)
            dist += temp.euclidean_distance(temp.east)
            n += 1
            temp = temp.east

        temp = u
        while temp.west is not None:
            seen.add(temp.west)
            for k in range(2, self.k + 3):
                queue.append(temp.west)
                phase.append(k)
            dist += temp.euclidean_distance(temp.west)
            n += 1
            temp = temp.west

        while len(queue) != 0:
            u = queue.pop(0)
            k = phase.pop(0)
            for i in range(0, 4):
                neighbors = u.broadcast_neighbors.get((k, i))
                if neighbors is None:
                    continue
                for v in neighbors:
                    if v not in seen:
                        n += 1
                        dist += u.euclidean_distance(v)
                        for p in range(k + 1, self.k + 1):
                            queue.append(v)
                            phase.append(p)
                        seen.add(v)
        return n, dist
    
    def broadcast_summary(self):
        complete = 0
        avg_dist = 0
        max_dist = 0
        min_p = math.inf
        for u in self.G.nodes():
            n, dist = self.broadcast(u)
            p = n / (self.n - 1)
            if p > 0.9:
                complete += 1
                avg_dist += dist
                if (p < min_p):
                    min_p = p
                if dist > max_dist:
                    max_dist = dist
                    node = u
        avg_dist /= complete
        mst_dist = 0
        for u,v in self.BMST.edges():
            mst_dist += self.BMST[u][v]['weight']
        return complete, min_p, avg_dist, max_dist, mst_dist, node

    def flood(self, u):
        n = 0
        dist  = 0
        seen  = {u}
        queue = [u]
        while len(queue) != 0:
            u = queue.pop(0)
            for v in self.G.neighbors(u):
                if v not in seen:
                    dist += u.euclidean_distance(v)
                    seen.add(v)
                    queue.append(v)
        return n, dist

    def flood_summary(self):
        complete = 0
        avg_dist = 0
        max_dist = 0
        for u in self.G.nodes():
            n, dist = self.flood(u)
            avg_dist += dist
            complete += 1
            if dist > max_dist:
                max_dist = dist
                node = u
        avg_dist /= complete

        mst_dist = 0
        for u,v in self.GMST.edges():
            mst_dist += self.GMST[u][v]['weight']
        return complete, avg_dist, max_dist, mst_dist, node