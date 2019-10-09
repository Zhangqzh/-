

from __future__ import division
import time
import sys

sys.setrecursionlimit(1000000)
symbols = [1, 2, 3, 4, 5, 6, 7, 8, 9]


def isValid(spuzzle, cellIndex, symIndex):
    if symIndex >= limit:
        return False
    row = cellIndex // limit
    col = cellIndex % limit
    box = row // boxsize * boxsize * limit + col // boxsize * boxsize

    for i in range(limit):
        if spuzzle[row * limit + i] == symbols[symIndex]:
            return False
        if spuzzle[col + i * limit] == symbols[symIndex]:
            return False
        if spuzzle[
            box + (i // boxsize * limit + i % boxsize)
        ] == symbols[symIndex]:
            return False
    return True


def solve(puzzle):
    cellIndex = 0
    while cellIndex < len(puzzle) - 1 and puzzle[cellIndex] != 0:
        cellIndex += 1
    symIndex = 0
    spuzzle = list(puzzle)
    count = 0

    while cellIndex < len(puzzle):
        count += 1
        if isValid(spuzzle, cellIndex, symIndex):
            spuzzle[cellIndex] = symbols[symIndex]
            cellIndex += 1
            while cellIndex < len(puzzle) - 1 and puzzle[cellIndex] != 0:
                cellIndex += 1
            symIndex = 0
        else:
            symIndex += 1
        if symIndex >= limit:
            spuzzle[cellIndex] = 0
            cellIndex -= 1
            while puzzle[cellIndex] != 0:
                cellIndex -= 1
            symIndex = symbols.index(spuzzle[cellIndex]) + 1

    return (spuzzle , count)

class Space:
    def __init__(self,index):
        self.index = index
        self.row = index // limit
        self.col = index % limit
        self.box = self.row // boxsize * boxsize * limit +self.col // boxsize * boxsize
        self.constraints = set()

    def set_constraints(self,constrains):
         self.constraints = constrains

    def __lt__(self,other):
        if len(self.constraints) < len(other.constraints):
            return True


def create_sapces(puzzle):
    spaces =[]
    for i in range(len(puzzle)):
        if puzzle[i] == 0:
            spaces.append(Space(i))
    return spaces




def back_tracking(puzzle,spaces,start,index,nodesb):
    nodesb+=1
    spuzzle = list(puzzle)
    if index < len(spaces):
        space = spaces[index]
        listr = [puzzle[j] for j in range(len(puzzle)) if j // limit == space.row]
        listc = [puzzle[j] for j in range(len(puzzle)) if j % limit == space.col]
        listb = [puzzle[j] for j in range(len(puzzle)) if (j // limit) // boxsize * boxsize * limit + (
                   j % limit) // boxsize * boxsize == space.box]

        constraint = set(symbols) - set(listr) - set(listc) - set(listb)
        space.set_constraints(constraint)

        tmpcon = list(space.constraints)

        if start<len(tmpcon):
            conIndex = start
            spuzzle[space.index] = tmpcon[conIndex]
            return back_tracking(spuzzle, spaces, 0, index + 1,nodesb)
        else:
            preIndex = spaces.index(space)-1
            precon = list(spaces[preIndex].constraints)
            preValueIndex =precon.index(spuzzle[spaces[preIndex].index])
            spuzzle[spaces[preIndex].index]=0
            return back_tracking(spuzzle,spaces,preValueIndex+1,preIndex,nodesb)
    else:
        return (spuzzle , nodesb)


def get_constraints(puzzle,space):
    listr = [puzzle[j] for j in range(len(puzzle)) if j // limit == space.row]
    listc = [puzzle[j] for j in range(len(puzzle)) if j % limit == space.col]
    listb = [puzzle[j] for j in range(len(puzzle)) if (j // limit) // boxsize * boxsize * limit + (
            j % limit) // boxsize * boxsize == space.box]

    constraint = set(symbols) - set(listr) - set(listc) - set(listb)
    space.set_constraints(constraint)



def MRV(puzzle,spaces,start,index,nodesm):
    nodesm+=1
    for i in spaces:
        get_constraints(puzzle,i)
    spaces.sort()
    spuzzle = list(puzzle)
    if index < len(spaces):
        space = spaces[index]

        tmpcon = list(space.constraints)

        if start < len(tmpcon):
            conIndex = start
            spuzzle[space.index] = tmpcon[conIndex]
            return MRV(spuzzle, spaces, 0, index + 1,nodesm)
        else:
            preIndex = spaces.index(space) - 1
            precon = list(spaces[preIndex].constraints)
            preValueIndex = precon.index(spuzzle[spaces[preIndex].index])
            spuzzle[spaces[preIndex].index] = 0
            return MRV(spuzzle, spaces, preValueIndex + 1, preIndex,nodesm)
    else:
        return (spuzzle ,nodesm)





















def main():
    data_list = []
    get_command = input("Please enter the name of your puzzle\n: ")
    getc=get_command.split()
    filename=getc[0]
    algorithm=getc[1]
    with open(filename) as f, open(filename + '_solved.txt', 'w') as s,open(filename + 'performance.txt','w') as p:
        t = time.process_time()
        for line in f:
            num = list(map(int, line.split()))
            data_list.extend(num)


        global limit, boxsize
        limit = int((len(data_list) + 1) ** (1 / 2))
        boxsize = int(limit ** (1 / 2))
        if algorithm == 'BF':
            (solved,nodes) = solve(data_list)
        elif algorithm == 'BT':
            spaces = create_sapces(data_list)
            (solved , nodes) = back_tracking(data_list,spaces,0,0,0)
        elif algorithm == 'FC-MRV':
            spaces = create_sapces(data_list)
            (solved ,nodes) = MRV(data_list, spaces, 0, 0,0)

        s.write("%s" % solved)
        p.write("Process time: %f Total nodes: %d\n" % (time.process_time() - t,nodes))
        print("%s" % solved)
        print("Process time: %f Total nodes: %d\n" % (time.process_time() - t,nodes))


if __name__ == '__main__':
    main()
