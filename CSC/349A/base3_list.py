
def base3_list_to_base10(base3_list):
    base10_result = 0.0 
    base3_list.reverse()
    for i, n in enumerate(base3_list):
        base10_result += n * 3 ** i

    # your code goes here and should calculate the base10 floating point value appropriately  
    return base10_result 

    
     



