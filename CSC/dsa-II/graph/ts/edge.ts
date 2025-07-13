/*
 * u is the source node
 * v is the node adjacent to u
 * v 
 */
export default class Edge<T>{
    private u: T;
    private v: T;
    private w: number;
    constructor(u: T, v: T, w: number){
        this.u = u;
        this.v = v;
        this.w = w;
    }
    public from(): T {
        return this.u;
    }
    public to(): T {
        return this.v;
    }

    public weight(): number {
        return this.w;
    }

    /*
     * compares edge a (caller) and edge b (argument)
     * returns: 
     * a < b: -1
     * a > b:  1
     * a == b: 0
     * */
    public compare(E: Edge<T>): number {
        return this.w < E.weight() ? -1 :
               this.w > E.weight() ? 1 :
               0
    }
}

export type Edges<T> = Edge<T>[];

