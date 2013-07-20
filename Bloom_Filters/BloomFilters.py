""" Implemented by Rahul Manghwani . Key is choosing optimal values for no of hash functions and arraySize
    Probability of False Positive is (1 - e^(-kn/m))^k
    Memory Usage is O(cn) where n is the no of elements
    Time is O(cm) where m is how many bits long each element is

    No of Hash Functions(k) = ln(2) * c

    Function to get the optimal value for k = m/n * ln(2) .. Choose m , n . Cal K. Find the error rate, if acceptable go ahead, else choose diff values """
 
""" Import Inbuilt Hash Function"""
from zlib import crc32

""" Returns a output of applying k hash functions to the key"""
def hashFunctions(key, noofHashFunctions, arraySize):
    return [abs(crc32(key,hashfuncNo) % arraySize) for hashfuncNo in range(noofHashFunctions)]


class BloomFilters(object):
    def __init__(self,arraySize, noofHashFunctions):
        self.BloomArray = [0] * arraySize
        self.k = noofHashFunctions

    def add(self, key):
        for i in hashFunctions(key, self.k, len(self.BloomArray)):
            self.BloomArray[i] = 1
    
    def __contains__(self,key):
        for i in hashFunctions(key,self.k, len(self.BloomArray)):
            if self.BloomArray[i] == 0 :
                return False
        return True


bf = BloomFilters(50,7)

bf.add("hello")
bf.add("californication")
bf.add("hallucination")
bf.add("emulated")
bf.add("master")
bf.add("father")
bf.add("brother")
bf.add("sister")

for j in ['californication', 'lets us Bloomers', 'daugther', 'master', 'brother']:
    print j , j in bf


            
    
