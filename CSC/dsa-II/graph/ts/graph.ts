import Edge, { type Edges } from "./edge";

export type GraphItems<T> = Array<[T, T, number]>;
export default class Graph<T> {
    edges: Map<T, Edges<T>>;
    /*
     * accepts an array containing elements of the form [source, adjacent, weight]
     * */
    constructor(items: GraphItems<T>){
        this.edges = this.constructEdges(items); 
    }
    private constructEdges(items: GraphItems<T>): Map<T, Edges<T>>{

        const ret = new Map<T, Edges<T>>();
        for(const [u, v, w] of items){

            const edges = ret.get(u) || [];
            edges.push(new Edge<T>(u, v, w));
            ret.set(u, edges);

        }
        return ret;
    }
    public adjacent(u: T): Edges<T>{
        return this.edges.get(u) || [];
    }

    public edgeIterator(): MapIterator<[T, Edges<T>]> {
        return this.edges[Symbol.iterator]();
    }

    public getEdgeArray(): Edges<T> {
        return Array.from(this.edgeIterator())
                .flatMap(([_, v]) => v);
    }

    /**
     * Returns a list of unique vertices in the graph.
     *
     * Uses `filter` to ensure each vertex appears only once:
     * it keeps the vertex if its current index matches the index
     * of its first occurrence in the array.
     *
     * @returns An array of unique vertices.
     */
    public getVertices(): T[] {
        const v = Array.from(this.edges.keys()[Symbol.iterator]());
        return v.filter((v, i, A) => A.indexOf(v) === i);
    }
}
