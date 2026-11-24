import {
    AmbientLight,
    BufferGeometry,
    DirectionalLight,
    Float32BufferAttribute,
    LineBasicMaterial,
    LineSegments,
    Mesh,
    MeshStandardMaterial,
    PerspectiveCamera,
    Points,
    PointsMaterial,
    Scene,
    WebGLRenderer,
} from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import type { Triangle, Vec3 } from "./simulation";

export function createView(container: HTMLElement) {
    const scene = new Scene();
    const camera = new PerspectiveCamera(40, 1, 0.1, 100);
    camera.position.set(2.5, 1.7, 3.4);

    const renderer = new WebGLRenderer({ antialias: true, alpha: true });
    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    container.append(renderer.domElement);

    const controls = new OrbitControls(camera, renderer.domElement);
    controls.enableDamping = true;
    controls.minDistance = 1.6;
    controls.maxDistance = 12;

    const light = new DirectionalLight(0xffffff, 2.2);
    light.position.set(3, 4, 5);
    scene.add(light, new AmbientLight(0xffffff, 1.4));

    const surface = new Mesh(
        new BufferGeometry(),
        new MeshStandardMaterial({
            color: 0x236fd6,
            flatShading: true,
            polygonOffset: true,
            polygonOffsetFactor: 1,
            polygonOffsetUnits: 1,
        }),
    );
    const edges = new LineSegments(
        new BufferGeometry(),
        new LineBasicMaterial({ color: 0x90dcff }),
    );
    const points = new Points(
        new BufferGeometry(),
        new PointsMaterial({ color: 0xf1fbff, size: 0.045 }),
    );
    scene.add(surface, edges, points);

    new ResizeObserver(() => {
        const { clientWidth, clientHeight } = container;
        if (clientWidth && clientHeight) {
            camera.aspect = clientWidth / clientHeight;
            camera.updateProjectionMatrix();
            renderer.setSize(clientWidth, clientHeight, false);
        }
    }).observe(container);

    renderer.setAnimationLoop(() => {
        controls.update();
        renderer.render(scene, camera);
    });

    return {
        update(vertices: readonly Vec3[], triangles: readonly Triangle[]) {
            const positions = new Float32BufferAttribute(vertices.flat(), 3);
            const surfaceGeometry = new BufferGeometry();
            surfaceGeometry.setAttribute("position", positions);
            surfaceGeometry.setIndex(triangles.flat());
            surfaceGeometry.computeVertexNormals();

            const edgesByStart = vertices.map(() => new Set<number>());
            for (const [a, b, c] of triangles) {
                edgesByStart[Math.min(a, b)].add(Math.max(a, b));
                edgesByStart[Math.min(b, c)].add(Math.max(b, c));
                edgesByStart[Math.min(c, a)].add(Math.max(c, a));
            }
            const indices = edgesByStart.flatMap((ends, start) =>
                Array.from(ends, (end) => [start, end]).flat(),
            );
            const edgeGeometry = new BufferGeometry();
            edgeGeometry.setAttribute("position", positions.clone());
            edgeGeometry.setIndex(indices);

            const pointGeometry = new BufferGeometry();
            pointGeometry.setAttribute("position", positions.clone());

            surface.geometry.dispose();
            edges.geometry.dispose();
            points.geometry.dispose();
            surface.geometry = surfaceGeometry;
            edges.geometry = edgeGeometry;
            points.geometry = pointGeometry;

            return indices.length / 2;
        },
    };
}
