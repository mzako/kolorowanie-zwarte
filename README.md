# kolorowanie-zwarte (Interval edge coloring)

Based on [paper by Paweł Szołtysek](http://www.szoltysek.eu/works/bo_planowanie.pdf).

Interval edge coloring means that edges in every vertex need to be from a specific interval
of integers. So for example, given a vertex with edges colored {5, 6, 7, 8} is fine, but
a vertex with edges colored {5, 6, 8, 9} is not.

The algorithm is heuristic, so it may not find a solution, and if it does find one,
it does not necessarily mean that it's an optimal one (uses minimal number of colors).

> Note: In our implementation, we denote "no color" as 0.

Algorithm seems pretty OK at coloring simple graphs and simple bipartile graphs.

Below are some results (more results [here](./output))

## Successful attempts

### Bipartile graph 1 (from mentioned paper)

Uses optimal number of colors

![szoltysek2](./output/szoltysek2_out.png?raw=true)

### Bipartile graph 2 (from mentioned paper)

Suboptimal (vertex 9 could use numbers colors {6, 7, 8} instead of going "down")

![szoltysek](./output/szoltysek_out.png?raw=true)

### Bipartile graph 3 B5,10

5 - 10. Suboptimal - vertices on the right could easily go "up" in colors, instead of down.

![B_5_10](./output/B_5_10_out.png?raw=true)

### K4

![K4](./output/K4_out.png?raw=true)

### 2 loops with a bridge

![2loop2bridge](./output/2loop2bridge_out.png?raw=true)

## Failed attempts

Notice edges that the algorithm failed to color (denoted by label '0')

### Bipartile graph 1

15 - 20

![B_15_20_FAILED_out](./output/B_15_20_FAILED_out.png?raw=true)

### K6

![K6_FAILED](./output/K6_FAILED_out.png?raw=true)

## Running

Follow the instructions [here](./cpp/README.md).

The program will create two files: one in .dot format, and another in .txt.
.dot file can be used to generate a graph like so (requires graphviz to be installed):

```
dot -Tpng tree_out.dot -o tree_out.png
```

We have also implemented a simple graph generator in Python. It can generate
three types of graphs: complete, bipartile (with random connections) and
random. The script is easy to use and is contained [here](./graph_generator).
