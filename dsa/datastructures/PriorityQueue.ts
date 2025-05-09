class PriorityQueue<T>{
	heap: T[];
	comparator: (a: T, b: T) => boolean;

	constructor(items: T[], comparator: (a: T, b: T) => boolean){
		this.comparator = comparator;
		this.heap = []; for(const t of items) this.push(t);
		
	}

	parent(i: number){ return (i-1)/2|0 }
	left(i: number){ return 2*i + 1; }
	right(i: number){ return 2*i + 2; }

	heapifyBottomUp(item: T, i: number){
		if(i === 0 || item == null) return;

		const pIdx = this.parent(i);
		const P = this.heap[ pIdx ];
		
		if(this.comparator(item, P)){
			this.heap[ pIdx ] = item;
			this.heap[i] = P;
			this.heapifyBottomUp(item, pIdx);
		}
	}

	

	heapifyTopDown(i: number){
		let  P = i;
		const L = this.left(i);
		const R = this.right(i);
		const n = this.heap.length;

		if(L < n && this.comparator(this.heap[L], this.heap[P]))
			P = L;
		if(R < n && this.comparator(this.heap[R], this.heap[P]))
			P = R;

		if(P !== i){
			const temp = this.heap[i];
			this.heap[i] = this.heap[P];
			this.heap[P] = temp;
			this.heapifyTopDown(P);
		}
		
	}

	push(item: T) {
		this.heap.push(item);
		this.heapifyBottomUp(item, this.heap.length - 1);
	}

	pop(): T | null {

		if(this.heap.length <= 0) return null;

		const item = this.heap[0]!;
		this.heap[0] = this.heap[ this.heap.length  - 1 ];
		this.heap.pop();
		this.heapifyTopDown(0);

		return item;
	}
}

const q = new PriorityQueue([1,2,3,4,5,6], (a, b) => a > b);
console.log('asc');
while(q.heap.length > 0) console.log(q.pop());

console.log('\ndesc');
const r = new PriorityQueue([6,5,4,3,2,1], (a, b) => a < b);
while(r.heap.length > 0) console.log(r.pop());
