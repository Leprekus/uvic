export class UnionFindNode<T>{
    element: T;
    parent: UnionFindNode<T>;
    size: number;
    constructor(element: T) {
        this.element = element;
        this.parent = this;
        this.size = 1;
    }
}

export default class UnionFind<T> {
    public nodes: Map<T, UnionFindNode<T>>;
    constructor () {
        this.nodes = new Map<T, UnionFindNode<T>>;
    }
    public union(a: T, b: T): void {
        const p1 = this.find(a);
        const p2 = this.find(b);
        if(p1 === p2) return;
        if(p1.size >= p2.size) {
            p2.parent = p1;
            p1.size += p2.size;
        } else {
            p1.parent = p2;
            p2.size += p1.size;
        }
    }
    private findHelper(a: UnionFindNode<T>): UnionFindNode<T> {
        if(a.parent !== a) return this.findHelper(a.parent);
        return a;
    }

    public find(k: T): UnionFindNode<T> {
        const key = this.nodes.get(k); 
        if(key === undefined) throw Error(`No node found with key ${k}`);
        return this.findHelper(key);
    }
    public add(k: T): UnionFindNode<T> {
        const v =  new UnionFindNode(k);
        this.nodes.set(k , v);
        return v;
    }
}
