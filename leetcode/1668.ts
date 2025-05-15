function maxRepeating(sequence: string, word: string): number {
	
    	let count = 0, w = word;
        while(sequence.includes(w)){
            w += word;
            count++;
        }
        return count;

};
