class Line:
    size = 5
    def __init__(self, block_sections):
        self.d = block_sections
        self.s = "u" * Line.size
        self.solved = False

    def pureFix(self):
        return self.fix(self.size - 1, len(self.d) - 1)

    def fix(self, i, j):
        if i < 0:
            return j < 0
        else:
            return self.fix0(i, j) or self.fix1(i, j)
        
    def fix0(self, i, j):
        if self.s[i] != "1":
            return self.fix(i - 1, j)
        return False

    def fix1(self, i, j):

        leadingZero = (# The index of string need to greater than d[j] + 1, for example 'uuuu' and d[j] = 3, that mean i must be 3 or more bigger to make string become '0111' which has leading zero.
            j >= 0 and i >= self.d[j]
        ) and self.matchSigma(  
            i, j
        )

        leadingOne = (j == 0 and  (i + 1) - self.d[j] == 0)
        if leadingOne:
            for index in range(self.d[j]):
                if self.s[index] == '0':
                    leadingOne = False
                    break
            # print(i, j, leadingZero, leadingOne)

        if leadingOne:
            return True
        elif leadingZero:
            return self.fix(i - self.d[j] - 1, j - 1)
        return False

    def matchSigma(self, i, j):
        if self.s[i - self.d[j]] != "1":
            for index in range(self.d[j]):
                if self.s[i - self.d[j] + 1 + index] == "0":
                    return False
        else:
            return False
        return True

    def paint(self, i=None, j=-2):
        if i == None:
            i = self.size - 1
        if j == -2:
            j = len(self.d) - 1

        if i < 0:
            return ""

        if j < 0:
            return "0" * (i + 1)

        p0, p1 = self.fix0(i, j), self.fix1(i, j)
        # print(f'p0 = {p0} p1 = {p1} {i} {j}')

        # paint'
        if p0 and not p1:
            return self.paint0(i, j)
        elif not p0 and p1:
            return self.paint1(i, j)
        else:
            t0, t1 = self.paint0(i, j), self.paint1(i, j)
            return self.merge(t0, t1)

    def paint0(self, i, j):
        return self.paint(i - 1, j) + "0"

    def paint1(self, i, j):
        if self.d[j] == i + 1:
            return "1" * self.d[j]

        return self.paint(i - self.d[j] - 1, j - 1) + "0" + ("1" * self.d[j])

    def merge(self, s1, s2):
        re = ""
        for i in range(len(s2)):
            if s1[i] == s2[i]:
                re += s1[i]
            else:
                re += "u"
        return re

    def painted(self):
        self.s = self.paint()
        for char in self.s:
            if char == "u":
                self.solved = False
                return
        self.solved = True

    def diff(self, painted):
        diffs = []
        for char in range(self.size):
            if self.s[char] != painted[char]:
                diffs.append(char)
        return diffs

    def updatePixel(self, number, pixel):
        # print(self.s)
        self.s = self.s[:number] + pixel + self.s[number + 1:]
        # print(self.s)

    def toString(self):
        re = ""
        for i in range(len(self.s)):
            re += self.s[i]
            re += " "
        return re
