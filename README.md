
# Girvan–Newman Algorithm implemented using Boost and C++ for files of type graphml


The following C++ program was created in order to detect communities within a network. First, we find the shortest paths of the network. 
Next, using the paths we calculate a betweenness value for each Edge on the graph. We remove the Edge with the largest betweenness value.
We then continue calculating betweenness and removing Edges until there are none left. 
The program does not calculate modularity nor does it have a heurestic to determine when the communities are maximized. Alternatively, we output the graph several times over the course of the edge removal.

We wanted to utilize more heurstics and optimize our run time further, but ran into many issues regarding property maps and visitors in Boost's library and found this to be the best we could do.
##Program Input:
The program requires two command line arguments to be run.
1. The path to graphml file you would like to run the algorithm on (extension required)
2. The path/name you would like your files to be output with (no extension)
##Program Output:
The program will output several .dot files that represent the graph at various points of the run time of the algorithm
One of these files will have a begin indicator, this is your original .graphml file
The rest of the files will be labeled in order
Here is an example of the output of our program when called with the following command line from CLion(these files have been converted from .dot to .png using a script found in the repo):
```
/Users/yeet/Documents/GitHub/21f-pa03-how-bad-can-it-be/cmake-build-debug/21f-pa03-how-bad-can-it-be ../graphs/football.graphml ../output/footbball_output
```
> The graphml file the program calls is included in the graphs file of the repo

### This is the map as is.
![footbball_output_begin](https://user-images.githubusercontent.com/71847764/141670104-a6d0debd-1efa-41db-8085-937b6974ecfa.png)
### Here is the map after >20% of edges are removed
![footbball_output1](https://user-images.githubusercontent.com/71847764/141668978-1801fa29-459d-402a-96f2-a561964d9391.png)
### Here is the map after >40% of edges are removed
![footbball_output2](https://user-images.githubusercontent.com/71847764/141668979-828e8f23-b3bb-4002-b8f3-1710a7ccdb8d.png)
### Here is the map after >60% of edges are removed
![footbball_output3](https://user-images.githubusercontent.com/71847764/141668980-3152dca7-3faf-44cf-847f-87a34c2e23bd.png)

Currently our program only supports .dot output for a graphml file with a property type of label attatched. 


