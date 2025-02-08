def longest_palindrome(s):
    length = len(s)
    mxlen = 1
    
    if (length == 0):
        return 0
    if (length == 1):
        return 1

    for i in range(0, length):
        for j in range(0, length - i):
            if (s[i:length - i - j + 1] == \
                s[i:length - i - j + 1][::-1] \
                and len(s[i:length - i - j + 1])>=2):
                print(s[i:length - i - j + 1], i, j, -i-j+1)
                mxlen = max(mxlen, length - j - i + 1)


    return mxlen
    


if (__name__ == "__main__"):
    while(True):
        s = input();
        print(longest_palindrome(s));
    
