type Items<T> = Array<T>
type Edges = Edge[];
interface Vertex {};
class Edge {
}

class Graph {
    constructor(E: Edges){
    }
}
class Dijkstra<Items> {
    G: Graph;
    s: Vertex;
    constructor(G: Graph, s: Vertex) {
        this.G = G;
        this.s = s;
    }
    private heapify() {}
    public popMin() {}
}

const g: Graph = {};
const s: Vertex = {};
const d = new Dijkstra(g, s);

