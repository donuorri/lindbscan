# ${1:Flood-Fill Density Based Clustering (FFDBC)}

This project is a the reference implementation of the Flood Fill Density Based Clustering algorithm,
a fast density based clustering algorithm with O(n) time complexity.
DBSCAN (Density-Based Spatial Clustering of Applications with Noise) is a milestone in the field
of clustering techniques that gave origin to a prolific research trend
thanks to its innovative theoretical clustering model based on the concept of density.
The major break-through deriving from the application of this technique, consists in
the detection of arbitrarily-shaped clusters, combined with the contextual discard of
outliers, regarded as noise.
Nevertheless, the price for these remarkable features is the O(n^2) computational
complexity, where n equals the cardinality of the input dataset.
FFDBC was designed to overcome such limitation, providing a density based technique which has a linear
computational complexity.

## Description of the algorithm

The high computational cost of DBSCAN is essentially due to the distance measurements that the algorithm
performs during the scan phase. Optimizations based on indexing trees (R*, KD) can lower the time complexity
to O(n log n) in the average case.
FFDBC reduces the execution time by avoiding distance measurements. The input space is partitioned in cells
using a regular grid. Each cell contains 0 or more input points. To optimize memory usage, only non empty cells
(cells with one or more points) are stored in memory. FFDBC then proceeds by merging adjacent non empty cells.
Therefore, the density model used in FFDBC is an approximation of the one introduced with DBSCAN.
Nevertheless, experimental results have proven the soundness of this approach.


## Dependencies and Compilation

This reference implementation was developed with the Qt Framework 5.2. To compile the source code
simply install the Qt Framework 5.2 (or above) and its dependencies, then use the qmake tool, or the
IDE Qt Creator to build the sources listed in the project file FFDBC_Demo.pro.
This project was developed and tested on Ubuntu Linux 14.04, but it should compile just fine on the other
platforms supported by the Qt Framework.

## Usage

Once you have built the sources, copy the directory "datasets" to the path where the executable file is located.
The program will look for a directory named "datasets" in its current working directory to load the datasets.

To operate the software, simply select the dataset, set the parameters for clustering and then click on "Run".
Execution times, as well as other details, are reported to the standard output.

## Notes on performance

To ensure maximum readability of the code, this reference implementation is not fully optimized and it is single threaded.
Nevertheless, it shows how fast FFDBC is when compared to optimized DBSCAN implementations.
We believe that parallel implementations (on CPU or GPU) can produce significantly higher throughputs.

## Credits

FFDBC was developed at the Computer Human Interaction Laboratory (http://chilab.dicgim.unipa.it) of the
University of Palermo (Italy) as part of a post-grad research project.

## License

This software is released under the terms of the GPL 3.0 license.
