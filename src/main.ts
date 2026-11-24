import "./style.css";
import { createVertices, generateFaces, stepSimulation } from "./simulation";
import { createView } from "./view";

function element<T extends HTMLElement>(id: string): T {
    const result = document.getElementById(id);
    if (!result) {
        throw new Error(`Missing element: ${id}`);
    }
    return result as T;
}

function start() {
    const view = createView(element("viewport"));
    const vertexCount = element<HTMLInputElement>("vertex-count");
    const decreaseVertices = element<HTMLButtonElement>("vertex-decrease");
    const increaseVertices = element<HTMLButtonElement>("vertex-increase");
    const strength = element<HTMLInputElement>("strength");
    const tolerance = element<HTMLInputElement>("tolerance");
    const maxSteps = element<HTMLInputElement>("max-steps");
    const interval = element<HTMLInputElement>("interval");
    const play = element<HTMLButtonElement>("play");
    const step = element<HTMLButtonElement>("step");
    const reset = element<HTMLButtonElement>("reset");
    const status = element("status");
    const stats = element("stats");

    let vertices = createVertices(vertexCount.valueAsNumber);
    let stepCount = 0;
    let stable = false;
    let running = false;
    let lastStepAt = 0;

    function stepLimit(): number {
        return Math.max(1, Math.min(5000, maxSteps.valueAsNumber || 5000));
    }

    function updateControls() {
        const finished = stable || stepCount >= stepLimit();
        if (finished) {
            running = false;
        }
        play.textContent = running ? "Pause" : "Play";
        play.disabled = finished;
        step.disabled = running || finished;
        status.textContent = stable
            ? "Converged"
            : finished
              ? "Step limit reached"
              : running
                ? "Running"
                : "Paused";
        element("step-count").textContent = `${stepCount} / ${stepLimit()}`;
    }

    function updateGeometry() {
        const faces = generateFaces(vertices);
        const edgeCount = view.update(vertices, faces);
        stats.textContent = `${vertices.length} vertices · ${edgeCount} edges · ${faces.length} faces`;
    }

    function advance() {
        const result = stepSimulation(
            vertices,
            strength.valueAsNumber,
            tolerance.valueAsNumber,
        );
        vertices = result.vertices;
        stable = result.stable;
        stepCount++;
        updateGeometry();
        updateControls();
    }

    function resetSimulation() {
        running = false;
        vertices = createVertices(vertexCount.valueAsNumber);
        stable = false;
        stepCount = 0;
        updateGeometry();
        updateControls();
    }

    play.addEventListener("click", () => {
        running = !running;
        lastStepAt = performance.now();
        updateControls();
    });
    step.addEventListener("click", advance);
    reset.addEventListener("click", resetSimulation);
    vertexCount.addEventListener("change", resetSimulation);
    maxSteps.addEventListener("input", updateControls);

    function updateVertexButtons() {
        decreaseVertices.disabled =
            vertexCount.valueAsNumber <= Number(vertexCount.min);
        increaseVertices.disabled =
            vertexCount.valueAsNumber >= Number(vertexCount.max);
    }

    function adjustVertexCount(delta: number) {
        vertexCount.stepUp(delta);
        vertexCount.dispatchEvent(new Event("input", { bubbles: true }));
        vertexCount.dispatchEvent(new Event("change", { bubbles: true }));
    }

    decreaseVertices.addEventListener("click", () => adjustVertexCount(-1));
    increaseVertices.addEventListener("click", () => adjustVertexCount(1));
    vertexCount.addEventListener("input", updateVertexButtons);
    updateVertexButtons();

    for (const input of [vertexCount, strength, tolerance, interval]) {
        const output = element(`${input.id}-value`);
        const update = () => {
            output.textContent =
                input.value + (input === interval ? " ms" : "");
        };
        input.addEventListener("input", update);
        update();
    }

    function tick(now: number) {
        if (running && now - lastStepAt >= interval.valueAsNumber) {
            lastStepAt = now;
            advance();
        }
        requestAnimationFrame(tick);
    }

    updateGeometry();
    updateControls();
    requestAnimationFrame(tick);
}

try {
    start();
} catch (error) {
    console.error(error);
    element("error").hidden = false;
    element("controls").setAttribute("disabled", "");
}
