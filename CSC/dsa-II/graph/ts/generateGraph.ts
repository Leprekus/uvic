import Dijkstra from "./dijkstra";
import { GraphItems } from "./graph";
import Kruskal from "./kruskal";
import fs from 'fs';
import UnionFind from "./unionFind"

/*
 * @params:
 * V: number of vertices
 * eRange: range of the number of edges the graph can have
 * wRange: range of weight values an edge can have
 * */

//a < b, returns a random integer in the range a (inclusive) b (exclusive)
const range = (a: number, b: number) => Math.floor(Math.random() * (b - a) + a);

const generateGraph = (V: number, eRange: [number, number], wRange: [number, number]): GraphItems<number> => {
    if(eRange[1] < V-1) throw Error('Graph must have at least V-1 edges');
    const [eMin, eMax] = eRange;
    const [wMin, wMax] = wRange;
    const numEdges =  range(eMin, eMax);
    const vertices = new Array(V).fill(0).map((_, u) => u);
    const edges: GraphItems<number> = new Array(numEdges)
        .fill(0)
        .flatMap((_, i) => {
            const u = i < V ? i : range(0, V);
            const v =  range(0, V);
            // if we're creating a cycle on the last vertex
            // create an edge to a vertex v, v < V - 1
            if(u === v && u >= V - 1){
                const v = range(0, V - 1);
                const w = range(wMin, wMax);
                return [[u,  v,  w], [v, u, w]];
            }
            // if we're creating a cycle on the first vertex
            // create an edge to a vertex v, 0 < v < V
            if(u === v && u === 0){
                const v = range(1, V);
                const w = range(wMin, wMax);
               return [[u,  v, w], [v, u, w]];

            }
            
            // if we're creating a cycle
            // assign v to be randomly more 0 < v < u, or u < v < V
            if(u === v) {
                const v = Math.random() < 0.5 ? range(0, u): range(u + 1, V);
                const w =  range(wMin, wMax);
                return [[u, v, w], [v, u, w]];
            }
            
            const w = range(wMin, wMax);
            return [[u, v, w], [v, u, w]];
        });

    return edges;
}

const runKruskal = (g: GraphItems<number>, ret: number[]) => {
    const k = new Kruskal(g, 0);
    const t = k.mst();
    const c: number = t.map(e => e.weight()).reduce((a, b) => a + b);
    console.log(`Kruskal: Cost ${c}`);
    ret.push(c);

};

const runDijkstra = (g: GraphItems<number>, ret: number[]) => {
    const d = new Dijkstra(g, 0);
    const t = d.mcp();
    const c: number = Array.from(t.entries()).map(([k, v]) => v).reduce((a, b) => a + b);
    console.log(`Dijkstra: Cost ${c}`);
    ret.push(c);
};
const runAlgorithms = () => {
    const [ kruskalRet, dijkstraRet ]: number[][] = [[], []];
    const graphs: Array<Array<[number, number, number]>> = [];
    new Array(30).fill(0)
    .map(_ => {
        const V = 5;
        const eRange: [number, number] = [V, V * V];
        const wRange: [number, number] = [0, V * V];
        const g = generateGraph(V, eRange, wRange);
        graphs.push(g);
        runKruskal(g, kruskalRet);
        runDijkstra(g, dijkstraRet);
    });

    const rows = kruskalRet.map((v, i) => `${v},${dijkstraRet[i]}`).join('\n');
    const content = graphs.map(graph =>
        graph.map(triple => triple.join(' ')).join('\n')
    ).join('\n\n'); // separate graphs with blank line

   fs.writeFileSync('results.csv', 'kruskal,dijkstra\n' + rows);
   fs.writeFileSync('graphs.txt', content);
}
runAlgorithms();
