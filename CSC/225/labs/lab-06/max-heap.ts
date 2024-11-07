const A1 = [6,5,4,1,2,3]
const A2 = [99,19,9,7,11,3,3,2,2,1]

class Maxheap {
	heap: number[]
	height: number
	counter: number
	root: number
	//parent >= children
	//insertion: insert -> bubbleUp (if necessary)
	//removal:
	constructor(size: number) {
		this.heap = new Array(size)
		this.height = Math.ceil(Math.log(size) / Math.log(2))
		this.counter = 0
		this.root = this.heap[0]
	}
	get_left(i: number){
		return 2*i
	}
	get_right(i: number){
		return 2*i + 1
	}

	get_parent(i: number){ return Math.floor(i/2) }

	bubble_up(i: number): void{
		const parent_idx = this.get_parent(i)
		const x = this.heap[i]
		const parent = this.heap[ parent_idx ]
		if(i === 0 || parent <= x)
			return
		this.heap[ parent_idx ] = x
		this.heap[i] = parent
		this.bubble_up(parent_idx)
	}

	insert(x: number): number {
		this.heap[ this.counter++ ] = x
		this.bubble_up(this.counter-1)
		return x
	}
}

	
	
	
	
	
const m1 = new Maxheap(A1.length)
const m2 = new Maxheap(A2.length)
for(const x of A1)
	m1.insert(x)

for(const x of A2)
	m2.insert(x)
console.log(m1.heap, m2.heap)
// [ 1, 2, 3, 5, 6, 4 ] [
// 	1,  2, 2,  3, 3,
//    11, 19, 7, 99, 9
//  ]