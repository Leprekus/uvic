import UnionFind from "./unionFind"

/*
 * @params:
 * V: number of vertices
 * eRange: range of the number of edges the graph can have
 * wRange: range of weight values an edge can have
 * */


//a < b, returns a random integer in the range a (inclusive) b (exclusive)
const range = (a: number, b: number) => Math.floor(Math.random() * (b - a) + a);

const generateGraph = (V: number, eRange: [number, number], wRange: [number, number]): number[][] => {
    if(eRange[1] < V-1) throw Error('Graph must have at least V-1 edges');
    const [eMin, eMax] = eRange;
    const [wMin, wMax] = wRange;
    const numEdges =  range(eMin, eMax);
    const vertices = new Array(V).fill(0).map((_, u) => u);
    const edges = new Array(numEdges)
        .fill(0)
        .map((_, i) => {
            const u = i < V ? i : range(0, V);
            const v =  range(0, V);
            // if we're creating a cycle on the last vertex
            // create an edge to a vertex v, v < V - 1
            if(u === v && u >= V){

               return [u,  range(0, V - 1), range(wMin, wMax) ]
            }
            // if we're creating a cycle on the first vertex
            // create an edge to a vertex v, 0 < v < V
            if(u === v && u === 0){

               return [u,  range(1, V), range(wMin, wMax) ];
            }
            
            // if we're creating a cycle
            // assign v to be randomly more 0 < v < u, or u < v < V
            if(u === v) {
                const diff = Math.random() < 0.5 ? -range(0, u): range(v + 1, V);
                return [u, v + diff, range(wMin, wMax) ];
            }
            
            return [u, v, range(wMin, wMax) ];
        });

    return edges;
}

console.log(generateGraph(5, [5, range(0, 5 * 5)], [0,53])); 
