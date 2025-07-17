import Graph, { type GraphItems } from "./graph";
import UnionFind from "./unionFind";

export default class Kruskal<T> {

    G: Graph<T>;
    uf: UnionFind<T>;
    constructor(items: GraphItems<T>, s: T) {
        this.G = new Graph<T>(items);
        this.uf = new UnionFind<T>();
    }

    public mst(): T[] {
        const sortedEdges = this.G.getEdgeArray();
        console.log(sortedEdges);
        return [];
    }
}

gen f ff
