import { GraphItems } from "./graph";
import Kruskal from "./kruskal";
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
        .map((_, i) => {
            const u = i < V ? i : range(0, V);
            const v =  range(0, V);
            // if we're creating a cycle on the last vertex
            // create an edge to a vertex v, v < V - 1
            if(u === v && u >= V - 1){

               return [u,  range(0, V - 2), range(wMin, wMax) ]
            }
            // if we're creating a cycle on the first vertex
            // create an edge to a vertex v, 0 < v < V
            if(u === v && u === 0){

               return [u,  range(1, V), range(wMin, wMax) ];
            }
            
            // if we're creating a cycle
            // assign v to be randomly more 0 < v < u, or u < v < V
            if(u === v) {
                const vNew = Math.random() < 0.5 ? range(0, u): range(u + 1, V);
                return [u, vNew, range(wMin, wMax) ];
            }
            
            return [u, v, range(wMin, wMax) ];
        });

    return edges;
}

const V = 5;
const eRange: [number, number] = [V, V * V];
const wRange: [number, number] = [0, 53];
const g = generateGraph(V, eRange, wRange);
console.log(g);
const k = new Kruskal(g, 0);
console.log(k.mst());
k
