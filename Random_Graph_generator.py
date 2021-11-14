from networkx.generators.random_graphs import erdos_renyi_graph
n = 120
p = 0.5
g = erdos_renyi_graph(n, p)
print(g.nodes)
print(g.edges)

#Ripped off code from https://blog.finxter.com/how-to-generate-random-graphs-with-python/