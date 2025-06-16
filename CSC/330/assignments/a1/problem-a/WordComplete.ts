const inputs = [
`2
COOKIEMONSTER
COO
COOKIEEATER
COURAGEOUS
COOKIEMONSTUR
FOUNTAINPEN
MORDOR
BEER
FEEBLE
ANDROID`,

]

const solution = () => {
    for(const str of inputs){
        let tokens = str.split('\n');
        tokens.slice(1, tokens.length); //omit input
        for(const token of tokens){
            // we are parsing num of inputs
            if( Number.isNaN(token) === false ){
            
            }
    
        }
    }
}
