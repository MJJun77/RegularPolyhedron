# RegularPolyhedron

## Vertex / Edge / Face number relation
Pyramid has : (V, E, F) = (4, 6, 4)
Then, if 1 Vertex is added over the one of the face, 1 face becomes 3 - 2 faces are increased.
So when Vertex count V >= 4, # of triangle face is : 4 + (V - 4) * 2 = 2V - 4
And, if all the faces are triangle shape, edge count is `# of faces` * 3 / 2 = 3V - 6
So, for the V vertex's polyhedron's (V, E, F) is : (V, 3V - 6, 2V - 4) for V >= 4

