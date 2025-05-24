//const porno = [ 2, 3, 4, 5, 7, 11, 12, 13, 27, 41, 69];
//const porno = [ 3, 4, 5, 12 ];
//const porno = [5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 83];
const porno = [21,90,50,68,42,63,67,39,37,25,44,5,27,26,18,81,16,9,75,95,100,66,29,49,36,76,28,48,1,32, 31];
const len = porno.length;
const M: {[k: number]: number[] } = {};
const powersets = (A: number[], n: number, dir: string) => {
	//console.log(dir, n, A);
	if(n === len) return;
	//console.log({
	//	dir,
	//	level: n,
	//	items: A,
	//	reduction: A.reduce((a, b) => a + b)
	//});
	const reduction = A.reduce((a, b) => a + b);

	if(reduction in M){
		const S1 = M[reduction];
		if( S1.concat(A).every((v, i) => v === porno[i]) ){
			console.log( 'solved!\n', {
				S1,
				A,
			});
			return;
		}
	} else {
		M[reduction] = A;
	}


	n++;
	powersets(A.slice(0, A.length - 1), n, 'L');
	powersets(A.slice(1, A.length), n, 'R');
};
//powersets(porno, 0, 'root');
function canPartition(nums: number[]): boolean {
    nums = [...nums];
    if(nums.length <= 1) return false;

    nums.sort((a, b) => a - b);
    const reduction = nums.slice(0, nums.length - 1)
        .reduce((a, b) => a + b); //omit max elements
    
    //if max > sum(nums) - max there is no solution
    if(nums[nums.length - 1] > reduction) return false;
    const sum = reduction + nums[nums.length - 1];
    //if sum has a remainder there is no solution
    if(sum % 2) return false;
    //TODO: 
    //i)get k s.t. 2k = sum
    const k = sum / 2;
    let satisfied =  false;
    const stack: number[] = [];
    const M: { [key: number]: number[] } = {}; nums.forEach(key => (M[key] = M[key] || []).push(key));
    let w = nums[nums.length - 1];
    M[w].pop(); //remove max from available elements
    let dfs: number[] = [w]; //include max in our traversal
    //ii) build a tree to find k out of remaining elements
    // initialize with sum being set to the maximum element
    const findK = (A: number[], k: number) => {
	//initialize with all elements except max
	if(A.length === nums.length) throw Error("omit max");
	if(satisfied) return;
        if(A.length === 0) return;
	const s = A.reduce((a,b)=>a+b);
	w += s;
	if(w > k) {
	    //stack.forEach(k => M[k].push(stack.pop()!));
	    w -= s;
	    return;
	};
	if(w === k) {
	    satisfied = true;
	    console.log('satisfied', sum, dfs);
	    dfs = [...dfs, ...A];
	    return;
	}
	console.log(sum, 'sublist ', A);
	findK(A.slice(1, A.length), k, );
	findK(A.slice(0, A.length - 1), k);

    };
    console.log('calling fn()');
    findK(nums.slice(0, nums.length - 1), k);
    dfs = dfs.length ? dfs : [0];
    nums = (Object.values(M) as unknown as number[][]).flat();
    console.log({ dfs, nums, satisfied: dfs.reduce((a,b) => (a||0)+(b||0)) === nums.reduce((a,b) => (a||0)+(b||0))});
    //iii) see if remaining elements add up to k
    return (sum % 2) === 0;
    
};
canPartition(porno);

function canPartition(nums: number[]): boolean {
    nums = [...nums];
    if(nums.length <= 1) return false;

    nums.sort((a, b) => a - b);
    const reduction = nums.slice(0, nums.length - 1)
        .reduce((a, b) => a + b); //omit max elements
    
    //if max > sum(nums) - max there is no solution
    if(nums[nums.length - 1] > reduction) return false;
    const sum = reduction + nums[nums.length - 1];
    //if sum has a remainder there is no solution
    if(sum % 2) return false;
    //TODO: 
    //i)get k s.t. 2k = sum
    const k = sum / 2;
    let satisfied =  false;
    const stack: number[] = [];
    const M: { [key: number]: number[] } = {}; nums.forEach(key => (M[key] = M[key] || []).push(key));
    M[nums[nums.length - 1]].pop(); //remove max from available elements
    let dfs: number[] = []; //include max in our traversal
    //ii) build a tree to find k out of remaining elements
    // initialize with sum being set to the maximum element
	//initialize with all elements except max an
    const findK = (A: number[], k: number) => {
    
	//if(A.length === nums.length ) throw Error("omit max");
	if(satisfied) return;
    if(A.length === 0) return;
	const s = A.reduce((a,b)=>a+b);

	if(nums[nums.length - 1] === k || nums[nums.length - 1] + s === k) {
        
	    satisfied = true;
	    console.log('satisfied', nums[nums.length - 1] + s, dfs, A);
	    dfs = [nums[nums.length - 1]];
        if(nums[nums.length - 1] === k) return;
        dfs = [...dfs, ...A];
        A.forEach(k => M[k].pop());
	    return;
	}
    if(nums[nums.length - 1] + s !== k) {
	    //stack.forEach(k => M[k].push(stack.pop()!));
        
        console.log(nums[nums.length - 1] + s, 'sublist ', A, { k, m: nums[nums.length - 1] });
	    findK(A.slice(1, A.length), k);
	    findK(A.slice(0, A.length - 1), k);
        return;

	};

    };
    findK(nums.slice(0, nums.length - 1), k);
    dfs = dfs.length ? dfs : [0];
    
    nums = (Object.values(M) as unknown as number[][]).flat();
    console.log({ dfs, nums, satisfied: dfs.reduce((a,b) => (a||0)+(b||0)) === nums.reduce((a,b) => (a||0)+(b||0))});
    //iii) see if remaining elements add up to k
    return dfs.reduce((a,b) => (a||0)+(b||0)) === nums.reduce((a,b) => (a||0)+(b||0));
    
}
