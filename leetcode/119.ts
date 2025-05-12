//note: solution is exactly like 70.rs
function getRow(rowIndex: number): number[] {
    rowIndex += 1;
	const row = new Uint32Array(rowIndex);
    let prev = 0, curr = 1;
	for(let i = 0; i <= rowIndex; i++){
		for(let j = 0; j < i; j++){
            
            prev = curr;
            curr = row[j];
            row[j] += prev;
            
		}
	}
	return row as unknown as number[];
};
