class UnionFindNode<T>{
    element: T;
    parent: UnionFindNode<T>;
    size: number;
    constructor(element: T) {
        this.element = element;
        this.parent = this;
        this.size = 1;
    }
}
class UnionFind<T> {
    public union(a: UnionFindNode<T>, b: UnionFindNode<T>) {
        const p1 = this.find(a);
        const p2 = this.find(b);
        if(p1 === p2) return;
        if(p1.size > p2.size) {
            p2.parent = p1;
            p1.size += p2.size;
        } else {
            p1.parent = p2;
            p2.size += p1.size;
        }
    }
    public find(a: UnionFindNode<T>): UnionFindNode<T> {
        if(a.parent !== a) return this.find(a.parent);
        return a;
    }
    public add(element: T): UnionFindNode<T> {
        return new UnionFindNode(element);
    }
}
