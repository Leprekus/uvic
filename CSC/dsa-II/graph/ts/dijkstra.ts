import { Edges } from "./edge";
import Graph, { GraphItems } from "./graph";
import Heap, { HeapComparator, HeapItems, HeapPriority } from "./heap";

type MinHeap<T> = Heap<T>;
export default class Dijkstra<T> {
    G: Graph<T>;
    s: T;
    comparator: HeapComparator<T>;
    constructor(items: GraphItems<T>, s: T) {
        this.G = new Graph(items);
        this.s = s;
    }
    private minHeap(D: HeapItems<[T, number]>): MinHeap<[T, number]>  {
        return new Heap(
            D, 
            ([_, v0]: [T, number], [__, v1]: [T, number]) => v0 < v1 ? HeapPriority.First : HeapPriority.Second
        );
    }
    private popMin(H: MinHeap<[T, number]>): [T, number] | null {
        return H.popPriorityItem();
    }

    private minHeapUpdate(H: MinHeap<[T, number]>, item: [T, number]): void {

        const [ k0, v0 ] = item;
        H.items = H.items.map(([k, v]) => k === k0 ? [k0, v0] : [k, v]); 
        H.buildPriorityHeap();
    }
    /*
     * @returns:
     * a Map<T, number> of vertices containing the minimum cost path
     * from source to all other vertices
     * */
    public mcp(): Map<T, number> {

        const D = new Map<T, number>();
        this.G.getVertices().map(v => D.set(v, v === this.s ? 0 : Infinity));        

        const Q = this.minHeap(Array.from(D[Symbol.iterator]()));

        while(Q.length) {
            const [u, w0 ] = this.popMin(Q)!;
            if(w0 === Infinity) {
                break; //exit once we find an unreachable vertex
            }
            this.G.adjacent(u).map(e => {
                if(D.get(u)! + e.weight() < D.get(e.to())!){
                    D.set(e.to()!, D.get(u)! + e.weight());
                    this.minHeapUpdate(Q, [e.to(), D.get(e.to())!]);
                }
            });
        }
        return D;
    }
}



