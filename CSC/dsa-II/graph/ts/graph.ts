import Edge, { type Edges } from "./edge";

type Items<T> = Array<[T, T, number]>;
export default class Graph<T> {
    edges: Map<T, Edges<T>>;
    constructor(items: Items<T>){
        this.edges = this.constructEdges(items); 
    }
    private constructEdges(items: Items<T>): Map<T, Edges<T>>{
        const ret = new Map<T, Edges<T>>();
        for(const [u, v, w] of items){
            const edges = ret.get(u) || [];
            edges.push(new Edge<T>(u, v, w));
            ret.set(u, edges);

        }
        return ret;
    }
}
