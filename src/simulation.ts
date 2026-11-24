export type Vec3 = readonly [number, number, number];
export type Triangle = readonly [number, number, number];

// Match the tolerances used by the float-based C++ implementation.
const FLOAT_EPSILON = 2 ** -23;
const MIN_DISTANCE = 10 * FLOAT_EPSILON;
const FACE_TOLERANCE = 5 * FLOAT_EPSILON;

function subtract(a: Vec3, b: Vec3): Vec3 {
    return [a[0] - b[0], a[1] - b[1], a[2] - b[2]];
}

function dot(a: Vec3, b: Vec3): number {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

function cross(a: Vec3, b: Vec3): Vec3 {
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    ];
}

function normalize(
    vector: Vec3,
    length = Math.sqrt(dot(vector, vector)),
): Vec3 {
    if (length === 0) {
        return [0, 0, 0];
    }
    return [vector[0] / length, vector[1] / length, vector[2] / length];
}

export function createVertices(count: number): Vec3[] {
    const vertices: Vec3[] = [];
    while (vertices.length < count) {
        const theta = Math.random() * Math.PI;
        const phi = Math.random() * 2 * Math.PI;
        const vertex: Vec3 = [
            Math.sin(theta) * Math.cos(phi),
            Math.sin(theta) * Math.sin(phi),
            Math.cos(theta),
        ];
        const overlaps = vertices.some((other) => {
            const difference = subtract(vertex, other);
            return dot(difference, difference) <= MIN_DISTANCE ** 2;
        });
        if (!overlaps) {
            vertices.push(vertex);
        }
    }
    return vertices;
}

export function stepSimulation(
    vertices: readonly Vec3[],
    strength: number,
    tolerance: number,
): { vertices: Vec3[]; stable: boolean } {
    // Read every force from the same snapshot before using the new positions.
    const next = vertices.map((vertex, index): Vec3 => {
        let forceX = 0;
        let forceY = 0;
        let forceZ = 0;

        for (let other = 0; other < vertices.length; other++) {
            if (index === other) {
                continue;
            }
            const difference = subtract(vertex, vertices[other]);
            const distance = Math.sqrt(dot(difference, difference));
            const direction = normalize(difference, distance);
            const magnitude = strength / Math.max(distance, MIN_DISTANCE) ** 2;
            forceX += magnitude * direction[0];
            forceY += magnitude * direction[1];
            forceZ += magnitude * direction[2];
        }

        return normalize([
            vertex[0] + forceX,
            vertex[1] + forceY,
            vertex[2] + forceZ,
        ]);
    });

    const threshold = Math.max(tolerance, MIN_DISTANCE);
    const stable = next.every((vertex, index) =>
        vertex.every(
            (value, axis) =>
                Math.abs(value - vertices[index][axis]) <= threshold,
        ),
    );
    return { vertices: next, stable };
}

export function generateFaces(vertices: readonly Vec3[]): Triangle[] {
    const faces: Triangle[] = [];
    for (let a = 0; a < vertices.length - 2; a++) {
        for (let b = a + 1; b < vertices.length - 1; b++) {
            for (let c = b + 1; c < vertices.length; c++) {
                const origin = vertices[a];
                const normal = normalize(
                    cross(
                        subtract(vertices[b], origin),
                        subtract(vertices[c], origin),
                    ),
                );
                if (dot(normal, normal) === 0) {
                    continue;
                }

                // An outer face has all other vertices on one side of its plane.
                let front = false;
                let back = false;
                for (let index = 0; index < vertices.length; index++) {
                    if (index === a || index === b || index === c) {
                        continue;
                    }
                    const direction = normalize(
                        subtract(vertices[index], origin),
                    );
                    const side = dot(normal, direction);
                    front ||= side > FACE_TOLERANCE;
                    back ||= side < -FACE_TOLERANCE;
                    if (front && back) {
                        break;
                    }
                }

                if (!(front && back)) {
                    // Orient toward the empty half-space for front-face rendering.
                    faces.push(front ? [a, c, b] : [a, b, c]);
                }
            }
        }
    }
    return faces;
}
