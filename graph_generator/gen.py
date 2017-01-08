#!/usr/bin/python3

import random
from datetime import datetime
from collections import defaultdict

graph_type = int(input("Graph type: 2-bipartile, 1-random, 0-complete: "))

nodes = defaultdict(list)
filename = ""

if graph_type == 2:
    # bipartile
    num_group_1 = int(input("Number of vertices in group 1: "))
    num_group_2 = int(input("Number of vertices in group 2: "))
    num_edges = int(input("Number of edges: "))
    seed = input("Random seed (empty = time-based): ")
    if seed == "":
        seed = datetime.now()
    random.seed(seed)

    group_2_start = num_group_1 + 1

    edge_idx = 0
    while edge_idx < num_edges:
        current_node_idx = random.randint(0, num_group_1)
        random_node = current_node_idx
        while random_node == current_node_idx:
            random_node = random.randint(group_2_start, 
                                         group_2_start + num_group_2)
        if random_node not in nodes[current_node_idx]:
            nodes[current_node_idx].append(random_node)
            nodes[random_node].append(current_node_idx)
            edge_idx += 1

    filename = "B_{}_{}_{}_{}".format(num_group_1, num_group_2, num_edges, seed)


elif graph_type == 1:
    # random
    num_nodes = int(input("Number of nodes: "))
    num_edges = int(input("Number of edges: "))
    seed = input("Random seed (empty = time-based): ")
    if seed == "":
        seed = datetime.now()
    random.seed(seed)

    edge_idx = 0
    while edge_idx < num_edges:
        current_node_idx = random.randint(0, num_nodes)
        random_node = current_node_idx
        while random_node == current_node_idx:
            random_node = random.randint(0, num_nodes)
        if random_node not in nodes[current_node_idx]:
            nodes[current_node_idx].append(random_node)
            nodes[random_node].append(current_node_idx)
            edge_idx += 1

    filename = "{}_{}_{}".format(num_nodes, num_edges, seed)

else:
    # complete
    num_nodes = int(input("Number of nodes: "))
    for i in range(num_nodes):
        others = list(range(num_nodes))
        others.remove(i)
        nodes[i] = others
    filename = "K{}".format(num_nodes)


filepath = "../test_data/{}".format(filename)

with open(filepath, "w") as f:
    for k, v in nodes.items():
        f.write(str(k) + " " + " ".join(map(str, v)) + "\n")
print("saved results to {}".format(filepath))