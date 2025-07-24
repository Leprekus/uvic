export type HeapItems<T> = Array<T>;
export enum HeapPriority {
    First,
    Second,
}
/*
 * comparator returns:
 * compare(a,b) == 1 if a has a higher priority
 * compare(a, b) == -1 if b has a equal or greater higher priority than a
 * */
export type HeapComparator<T> = (a: T, b: T) => HeapPriority;
export default class Heap<T> {

    items: HeapItems<T>;
    comparator: HeapComparator<T>;

    constructor(items: HeapItems<T>, comparator: HeapComparator<T>){
        this.comparator = comparator
        this.items = items; this.buildPriorityHeap();
    }
    /*
     * since our heap is implemented in an array, the priority element is popped,
     * meaning that our left and right indexes must be computed from right to left
     * */
    private leftIdx(i: number): number { 
        if(i === 0) return 1;
        return 2 * i; 
    }
    private rightIdx(i: number): number { 
        if(i === 0) return 2;
        return 2 * i + 1; 
    }
    public buildPriorityHeap(){
        for(let i = this.items.length / 2 | 0; i >= 0; i--){
            this.heapify(this.items, i);
        }
    }
    /*
     * maintains the priority heap property
     * assumes that left(i) and right(i) are priority
     * heaps, but the i might be smaller than its children
     * */
    private heapify(items: HeapItems<T>, i: number){
        const L = this.leftIdx(i);
        const R = this.rightIdx(i);
        let priority: number = i;
        if( 
            L <= this.items.length - 1 &&
            this.comparator(this.items[L], this.items[i]) === HeapPriority.First
        ){

            priority = L;

        }
        if( 
            R <= this.items.length - 1 &&
            this.comparator(this.items[R], this.items[priority]) === HeapPriority.First
        ){
            priority = R;
        }

        if(priority !== i){
            const temp = this.items[i];
            this.items[i] = this.items[priority]; 
            this.items[priority] = temp;
            this.heapify(items, priority);
        }
    }
    public popPriorityItem(): T | null{
        if(this.items.length < 1) throw Error("Heap underflow");

        const item = this.items[0];
        this.items[0] = this.items[this.items.length - 1];
        this.items.pop();
        this.heapify(this.items,  0);
        return item;
    }

    public insertPriorityItem(item: T): void {
        this.items.push(item);
    }
    public peekPriorityItem(): T | null {
        return this.items[0];
    }
    
    get length(): number {
        return this.items.length;
    }
}

