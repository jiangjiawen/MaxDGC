# MaxDGC

This is a graph clustering method.

It use the Eigen library for the matrix compute.

I call thie method Max seeD Graph Clustering based on random walk with restart.

Yes, its core is RWR, but It is a seed finding and expansion method.

I use RWR result as a base.

The algrithm is as below:

<img src="https://github.com/jiangjiawen/MaxDGC/blob/master/maxdalg.png" width="500" height="600" title="alg">

Okay，The original name is CASRW. The Max seeD is that i choose seed clusters from biggest to the smallest, and delete some seeds which are much like the seeds have been selected，according to step 11-19. The final choosen ones are the max cliques around them.

Surely, my method seems a little stupid. But in the experiment, it behaves well if i use good parameters.

Nowadays, Graph convolution network is hot, traditional graph clustering can learn from it. Maybe, they can merge.

files content:
file  entry: main.cpp. readnet.h readnet.cpp: read net. rwrgetseedcluster.h rwrgetseedcluster.cpp: get seed clusters. clusterprocess.h clusterprocess.cpp expansionandmerge.cpp expansionandmerge.h: add nodes and merge process.

readnet, input: file, file format is abc, output: nodes name vector: rowcolname, a map the format is ```-node1-node2，c``` : mat_map.

rwrgetseedcluster: input: rowcolname, mat_map, rwr threshold δ. output: seed cluster.

ClusteringProcess: assign last nodes into these seeds.
expansionandmerge：add linking nodes and merge process.

