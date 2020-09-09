# Travelling-Salesmann-Problem-Using-Genetic-Algorithm-for-Parallel-and-Distributed-Computing

Travelling Salesperson Problem being a classic combinatorial optimization problem is an interesting but a challenging problem to be solved. It falls under the class of NP hard problem and becomes non-solvable for large data set by traditional methods like integer linear programming and branch and bound method, being the earlier popular approaches. Given the list of cities and distances between them, the problem is to find the shortest tour possible which visits all the cities in list exactly once and ends in the city where it starts. Despite the Traveling Salesman Problem is NP-Hard, a lot of methods and solutions are proposed to the problem. One of them is Genetic Algorithm (GA). GA is a simple but an efficient heuristic method that can be used to solve Traveling Salesman Problem. Genetic algorithm is powerful technique to discover optimal for traveling salesman problem (TSP). Genetic Algorithm based solutions emerged as the most popular tool to solve this which is a heuristic mechanism to find the closest approximate solution to the problem. In this project, we approach Travelling Salesman Problem using GA approach, a natural selection technique which repeatedly modifies a population of individual solutions, with the added power of modern computing systems. Here I come up with a parallel version of GA for both multicore and many core architectures in order to make some of the challenging problems like Vehicle Routing Problem for google maps, DNA sequencing and many more optimization problems involving a good amount of complex computation and data handling. The project presents a comparative analysis of the results for various degree and structure of graphs depicting close approximation to the accuracy in terms of most optimal path for traversal with highly reduced execution time.

Genetic parallel algorithm for approximating the Travelling Salesman Problem

### Contributing

* Install lib `mpich2`

```bash
$ sudo apt-get install mpich2
```

* Compile

```bash
$ make
```

* Run

```bash
$ mpiexec -n 2 ./tspgen


If you have any query please feel free to contact me on : kakshak@gmail.com
