type Items<T> = Array<T>
class Dijkstra<Items> {
    items: Items;
    heap: Items;
    constructor(items: Items) {
        this.items = items;
        this.heap = [] as Items;
    }

    private heapify() {}
}
