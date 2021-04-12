import sys
import pandas as pd
import pymssql 
import multiprocessing as mp
import os
import re

HOST = 'localhost'
USERNAME = 'hgilbert'
PASS = '8kx5jY3x7dXG'
DB = 'DLLR'
SCHEMA = 'dbo'

# MUST ADDRESS FACT THAT WHEN GRABBING FOREIGN KEY REALTIONS WE MIGHT BE ADDING TABLES THAT ARENT IN THE WHERE. SO SEND LIST OF TABLES IN THE WHERE STATEMENT FROM KEYPARSER AND ADD TO STATEMENT IF NOT ALREADY IN
map = {}
refrenceArray = []
file = open("./ForeignKey/all.txt", "r")
newMaster = open("./ForeignKey/subAll.txt", "r")
done = False
BigStatement = ""


FinalPath = []

def TableGetter(statement):
    tables = []
    pairs = statement.split("and")
    for pair in pairs:
        individuals = pair.split("=")
        for individual in individuals:
            if not individual.split(".")[0].strip() in tables:
                tables.append(individual.split(".")[0].strip())
    
    return tables

def Request(TableA, TableB):
    global file
    global newMaster
    FileLoader()
    MapPrep()
    path = []


    
    if TableA not in map[TableB] and TableB not in map[TableA]:
        path = []
        statement = ""

        TransFinder(TableB, TableA, TableB, path)

        if not BigStatement:
            TransFinder(TableA, TableB, TableA, path)
        
        return BigStatement, TableGetter(BigStatement)

       
    else:
        if TableA in map[TableB]:
            statement = ConnectionFinder(TableB,TableA)
            statement = statement[:-4]
            return statement, TableGetter(statement)
        else:
            statement = ConnectionFinder(TableA,TableB)
            statement = statement[:-4]
            return statement, TableGetter(statement)



def main():
    FileLoader()
    MapPrep()

    
    
    if (len(sys.argv) == 2):
        TableConnectFinder(sys.argv[1])
        
    if(len(sys.argv) != 3):
        print("usage: python3 keyParser SOURCE-TABLE DESTINATION-TABLE")
        exit()
    elif sys.argv[1] not in map[sys.argv[2]] and sys.argv[2] not in map[sys.argv[1]]:
        path = []
        statement = ""

        TransFinder(sys.argv[2].strip(), sys.argv[1].strip(), sys.argv[2].strip(), path)

        if not BigStatement:
            TransFinder(sys.argv[1].strip(), sys.argv[2].strip(), sys.argv[1].strip(), path)
        
        print(BigStatement)

       
    else:
        if sys.argv[1] in map[sys.argv[2]]:
            statement = ConnectionFinder(sys.argv[2],sys.argv[1])
            statement = statement[:-4]
            print(statement)
        else:
            statement = ConnectionFinder(sys.argv[1],sys.argv[2])
            statement = statement[:-4]
            print(statement)

    
        

def TwoWayAdder(relations):
    for line in relations:
        part1 = line.split("=")[0].strip()
        part2 = line.split("=")[1].strip()
        reverse = part2 + " = " + part1
        if reverse not in relations:
            relations.append(reverse)

    



def MapPrep():
    global map
    lines = file.readlines()
    pattern = "'(.*?)'"


    relations = []

    for line in lines:
        
        substrings = re.findall(pattern, line)

        counter = 1
        string1 = ""
        string2 = ""
        if substrings:

            for sub in substrings:
                if counter == 3:
                    relations.append(string1 + " = " + string2)
                    counter = 1

                if counter < 3:
                    if counter == 1:
                        string1 = sub
                    else:
                        string2 = sub
                    
                    counter += 1
            
            relations.append(string1 + " = " + string2)
            
    
    TwoWayAdder(relations)
    MapCreator(relations)

        


            


        
        

def MapCreator(relations):
    global map

    for line in relations:
        if not line.isspace():
            part1 = line.split("=")[0].strip()

            sourceTable = part1.split(".")[0].strip()
            sourceCol = part1.split(".")[1].strip()

            part2 = line.split("=")[1].strip()
            DestTable = part2.split(".")[0].strip()
            DestCol = part2.split(".")[1].strip()

            if sourceTable not in map:
                subMap = {}
                colArray = []
                colArray.append(lineDuplicationChecker(sourceTable, sourceCol, DestTable, DestCol))
                subMap[DestTable.strip()] = colArray
                map[sourceTable.strip()] = subMap
            else:
                if DestTable not in map[sourceTable]:
                    colArray = []
                    colArray.append(lineDuplicationChecker(sourceTable, sourceCol, DestTable, DestCol))
                    map[sourceTable][DestTable.strip()] = colArray
                else:
                    map[sourceTable][DestTable].append(lineDuplicationChecker(sourceTable, sourceCol, DestTable, DestCol))


def ReplaceInArray(sourceTable, sourceCol, DestTable, array, DestCol, line):
    
    for i in range(len(array)):
        if sourceTable in array[i] and sourceCol in array[i] and DestTable in array[i]:
            array[i] = line
            return

def lineDuplicationChecker(sourceTable, sourceCol, DestTable, DestCol):
    for line in refrenceArray:
        if sourceTable in line and sourceCol in line and DestTable in line:
            return line
    
    return sourceTable + "." + sourceCol + " = " + DestTable + "." +DestCol

    
def ThreeSubChecker(sourceTable, sourceCol, DestTable, array, DestCol ):
    for element in array:
        if sourceTable in element and sourceCol in element and DestTable in element:
            
            for line in refrenceArray:
                if sourceTable in line and sourceCol in line and DestTable in line and DestCol in line:
                    return True

            for line in refrenceArray:
                if sourceTable in line and sourceCol in line and DestTable in line: 
                    ReplaceInArray(sourceTable, sourceCol, DestTable, array, DestCol, line)
                    return True

           
        
            return True

    return False






def ConnectionFinder(Source, Dest):

    ConcatRelations = ""
    for relations in map[Source][Dest]:
        #print(relations)
        ConcatRelations += relations + " and "
    
    return ConcatRelations



def ParalleAnalysis(source, map):
    currentBigFileName = ""
    currentSmallFileName = ""

    newMaster = open("NEWOutputFolder/" + source + ".txt", "a")


    for dest in map[source]:
        
            subMap = {}
            for relations in map[source][dest]:
                subSource = relations.split("=")[0]
                
                if subSource not in subMap:
                    subMap[subSource] = []
                    subMap[subSource].append(relations.split("=")[1])
                else:
                    subMap[subSource].append(relations.split("=")[1])
            map[source][dest] = []
            smallestDif = 0 
            smallestDifNumber = 0

            
            if currentBigFileName != source:
                csvFile = pd.read_csv("inputFiles/" + source + ".csv", dtype = object)
                currentBigFileName = source
                
            for uniqueSource in subMap:
                if len(subMap[uniqueSource]) > 1:

                    print(uniqueSource + " has mutiple potential connections with: ")
                    print(subMap[uniqueSource])
                    
                    arrayToComp = csvFile[uniqueSource.split(".")[1].strip()].tolist()

                    smallestDif = 0 

                    for subDest in subMap[uniqueSource]:
                        if currentSmallFileName != subDest:
                            csvSubFile = pd.read_csv("inputFiles/" + subDest.split(".")[0].strip() + ".csv", dtype = object)
                            currentBigFileName = source

                        array1 = csvSubFile[subDest.split(".")[1].strip()].tolist()

                        avg = (len(set(arrayToComp) - set(array1)) + len(set(array1) - set(arrayToComp)))/2
                        
                        if smallestDif == 0:
                            smallestDif = subDest
                            smallestDifNumber = avg
                        elif avg < smallestDifNumber:
                            smallestDif = subDest
                            smallestDifNumber = avg
                    print("     chose: "  +smallestDif)
                    newMaster.write(uniqueSource + "=" + smallestDif + "\n")
                    subMap[uniqueSource] = smallestDif
            




def DuplicationHandler():
    pool = mp.Pool(processes=os.cpu_count()) 
    
    if not os.path.exists("NEWOutputFolder"):                                                                                                                      #Creating output folder if it doesnt already exist 
        os.makedirs("NEWOutputFolder")

    for source in map:
        pool.apply_async(ParalleAnalysis, args=(source,map,)) 
        #ParalleAnalysis(source)
    
    pool.close()
    pool.join()


def FileLoader():
    global refrenceArray

    for line in newMaster:
        refrenceArray.append(line)



def TableConnectFinder(Source):
    for tables in map[Source]:
        print("     " + tables)

            





def TransFinder(Source, Dest, ogSource, path):
    global done
    global BigStatement

    for subDest in map[Source]:    
        

        if subDest == Dest:
            path.append(subDest)
            statement = ConnectionFinder(ogSource, path[0])

            for i in range(len(path)):
                if((i + 1) < len(path)):
                    statement += ConnectionFinder(path[i], path[i+1])
            
            
            tables = ogSource + ", "
            for table in path:
                tables += table + ", "
            tables = tables[:-2]
            statement = statement[:-5]
            done = True

            BigStatement = statement
            return
    
   
    for subDest in map[Source]:
        if(done == False):

            if subDest in map and subDest not in path:
                path.append(subDest)
                TransFinder(subDest, Dest, ogSource, path) 
                
                top = path.pop()
                while top != subDest:
                    top = path.pop()
        else:
            return



if __name__ == '__main__':
  main()