def linearProbing(k: int) -> int:
   pass 

def doubleHashing(k: int) -> int:
    pass

'''
    TODO:
    [] IMPLEMENT CHOOSE HASHING PROCEDURE (LINEAR OR DOUBLE HASHING)
    [] IMPLEMENT HASHING METHOD (DIRECT ACCESS OR CHAINING)
    [] IMPLEMENT ALPHA FACTOR
'''
class HashTable:
    def __init__(self, size: int):
        self.ht = [None] * size
        self.size = size
        self.capacity = 0
        self.alpha = 
