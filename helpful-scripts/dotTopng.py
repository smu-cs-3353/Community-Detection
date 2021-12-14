import pydotplus

graph= pydotplus.graph_from_dot_file('/Users/yeet/Documents/GitHub/21f-pa03-how-bad-can-it-be/output/footbball_output_begin.dot')
graph.write_png('./output/footbball_output_begin.png')



