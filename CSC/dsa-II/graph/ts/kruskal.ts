import Edge, { Edges } from "./edge";
import Graph, { type GraphItems } from "./graph";
import UnionFind from "./unionFind";

export default class Kruskal<T> {

    G: Graph<T>;
    uf: UnionFind<T>;
    constructor(items: GraphItems<T>, s: T) {
        this.G = new Graph<T>(items);
        this.uf = new UnionFind<T>();
    }

    public mst(): Edges<T> {

        const tree: Edges<T> = []; const vertices = this.G.getVertices().map(v => this.uf.add(v));
        const sortedEdges = this.G.getEdgeArray().sort((a, b) => a.compare(b));
        sortedEdges.map(e => {
            if(this.uf.find(e.from()).parent !== this.uf.find(e.to())){
                this.uf.union(e.from(), e.to());
                tree.push(e);
            }
        });
        return tree;
    }
}

