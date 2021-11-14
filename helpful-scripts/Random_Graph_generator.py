import networkx as nx
from networkx.generators.random_graphs import erdos_renyi_graph
n = 120
p = 0.1
g = erdos_renyi_graph(n, p)
for node_id in g.nodes:
    g.nodes[node_id]['node_id'] = node_id
nx.write_graphml(g, 'graphs/random2.graphml', named_key_ids=True)


#Ripped off code from https://blog.finxter.com/how-to-generate-random-graphs-with-python/