"""Author: Rahul Manghwani. rrm333@nyu.edu.   NYU Courant
   Boyer Moore : Single Pattern Matching Algorithm
   Preprocessing Time and Space: O(m)
   Searching: Worst Case O(m+n)
              Best Case O(n/m)  """


"""Calculate Bad Char Heuristics"""
def generateBadCharShift(pattern):
    rightMostOccur = {}
    for i in range(0,len(pattern)-1):
        rightMostOccur[pattern[i]] = len(pattern)-1-i
    return rightMostOccur


"""Calculate Good Char Suffix Shifts"""
def generateGoodSuffixShift(pattern):
    """ Preprocess 1 """
    f = [0] * (len(pattern)+1)
    s = [0] * (len(pattern)+1)
    i = len(pattern)
    j = len(pattern) + 1
    f[i] = j
    while i>=0:
        while ( j<= len(pattern) and pattern[i-1] != pattern[j-1]):
            if ( s[j] == 0):
                s[j] = j -i
            j = f[j]
        i = i - 1
        j = j - 1
        f[i] = j
    """Now we need to handle Case 2 when part of the matching suffix occurs at the beginning """
    """Longest Jump"""
    lj = f[0]
    for i in range(0,len(pattern)):
        if(s[i]==0):
            s[i] = lj
        """If suffix of pattern becomes shorter than f[0] then next wider border is f[j"""
        if(i==lj):
            lj = f[i]
    return s


"""Actual Search"""
def bmsearch(text, pattern):
    bs = generateBadCharShift(pattern)
    gs = generateGoodSuffixShift(pattern)
    matches = []
    for i in range(0, len(text) - len(pattern)+1):
        j = len(pattern)-1
        """Compare from right to left"""
        while(j>=0 and pattern[j] == text[i+j]):
            j = j - 1
        if(j<0):
            """Match Found"""
            matches.append(i)
            i = i + gs[0]
        else :
            bsShift = bs.get(text[i+j])
            gss = gs[j+1]
            i = i + max(bsShift,gss)
    return matches
