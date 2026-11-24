# Regular Polyhedron

This browser simulation arranges vertices on a unit sphere using pairwise repulsion and displays the outer triangular faces.

## Run locally

With Node.js 22.12 or newer installed, run these commands from the repository root.

```powershell
npm ci
npm run dev
```

Open the local URL printed by Vite. Use **Play**, **Step**, and **Reset** to inspect the simulation. Changing the vertex count starts a new random arrangement. Drag to rotate the view and scroll to zoom.

In VS Code on Windows, **Debug RegularPolyhedron (Web)** is the first configuration in **Run and Debug**. After installing dependencies, press **F5** to start Vite and open Microsoft Edge with the debugger attached. If VS Code still shows a previously selected configuration, select the web configuration once. The debug task uses port `5173`, so stop any manually started Vite server on that port before pressing **F5**.

Run `npm run build` to type-check the code and generate a static build in `dist/`.

## Source

- [src/simulation.ts](src/simulation.ts) contains the math. `createVertices` initializes points on the unit sphere. `stepSimulation` calculates inverse-square repulsion, applies the forces simultaneously, projects back to the sphere, and checks convergence. `generateFaces` checks each candidate triangle for points on both sides and keeps the outside faces.
- [src/view.ts](src/view.ts) turns those vertices and triangles into Three.js geometry and handles the orbit camera.
- [src/main.ts](src/main.ts) connects the inputs, playback, and status to the simulation.

The [legacy OpenGL project](legacy%20-%20based%20OpenGL/README.md) remains available with its own build instructions.
