function generate(numRows: number): number[][] {
    const ret: number[][] = [];
    for(let i = 0; i < numRows; i++){
        const row: number[] = [];
        for(let j = 0; j < i + 1; j++){
            if(j < 1 || j === i){
                row.push(1);
            } else {
                row.push(ret[i - 1][j - 1] + ret[i - 1][j]);
            }
        }
        ret.push(row);
    }
    return ret;
};
