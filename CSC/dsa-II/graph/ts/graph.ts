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
}
