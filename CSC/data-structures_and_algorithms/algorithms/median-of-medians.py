def median_of_medians(A, i, opt):
    i = 0 if i <= 0 else i - 1
    #divide A into sublists of len 5
    sublists = [A[j:j+5] for j in range(0, len(A), 5)]
    medians = [sorted(sublist)[len(sublist)//2] for sublist in sublists]
    if len(medians) <= 5:
        pivot = sorted(medians)[len(medians)//2]
    else:
        #the pivot is the median of the medians
        pivot = median_of_medians(medians, len(medians)/2, opt)

    #partitioning step
    low = [j for j in A if j < pivot]
    high = [j for j in A if j > pivot]

    k = len(low)

    if opt == 'sm':
        if i < k:
            return median_of_medians(low,i, opt)
        elif i > k:
            return median_of_medians(high,i-k-1, opt)
        else: #pivot = k
            return pivot

    if opt == 'lg':
        if i < k:
            return median_of_medians(high,i, opt)
        elif i > k:
            return median_of_medians(low,i-k-1, opt)
        else: #pivot = k
            return pivot

    raise Exception("opt parameter not provided")

A = [1,2,3,4,5,6,7,8,9]

def ith_smallest(A, i):
    return median_of_medians(A, i, 'sm')

def ith_largest(A, i):
    return median_of_medians(A, i, 'lg')

print(ith_smallest(A, 1))
print(ith_largest(A, 1))

