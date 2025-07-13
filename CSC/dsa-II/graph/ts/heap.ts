type HeapItems<T> = Array<T>;

/*
 * comparator returns:
 * compare(a,b) == 1 if a has a higher priority
 * compare(a, b) == -1 if b has a equal or greater higher priority than a
 * */
type HeapComparator<T> = (a: T, b: T) => 1 | 0 | -1;
class Heap<T> {
    items: HeapItems<T>;
    comparator: HeapComparator<T>;
    constructor(items: HeapItems<T>, comparator: HeapComparator<T>){

        this.items = items; this.heapify(items, items.length - 1);
        this.comparator = comparator
    }
    private parentIdx(i: number): number { return i / 2 | 0; }
    private leftIdx(i: number): number { return 2 * i; }
    private rightIdx(i: number): number { return 2 * i + 1; }
    private heapify(items: HeapItems<T>, i: number){
        const L = this.leftIdx(i);
        const R = this.rightIdx(i);
        let priority: number;
        if( 
            L <= this.items.length - 1 && 
            this.comparator(this.items[L], this.items[i]) === 1
        ){
            priority = L;
        } else {
            priority = i;
        }

        if( 
            R <= this.items.length - 1 && 
            this.comparator(this.items[R], this.items[i]) === 1
        ){
            priority = R;
        }

        if(priority !== i){
            const temp = this.items[i];
            this.items[i] = this.items[priority]; 
            this.items[priority] = temp;
            this.heapify(items, i);
        }
    }
    public popPriorityItem(): T | null{
        const item = this.items.pop() || null;
        this.heapify(this.items, this.items.length - 1);
        return item;
    }
}
