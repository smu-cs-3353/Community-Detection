import pydotplus

graph= pydotplus.graph_from_dot_file('/Users/yeet/Documents/GitHub/21f-pa03-how-bad-can-it-be/football/football_starting_example.dot')
graph.write_png('/Users/yeet/Documents/GitHub/21f-pa03-how-bad-can-it-be/football/football_starting_example.png')



