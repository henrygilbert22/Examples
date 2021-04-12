import numpy as np
import pandas as pd
import csv
import itertools 
from itertools import permutations 
import multiprocessing as mp
import os
from os import path
import time

HOST = 'localhost'
USERNAME = 'hgilbert'
PASS = '8kx5jY3x7dXG'
DB = 'DLLR'
SCHEMA = 'dbo'
SearchCol = ["REALP.FNAME", "REALP.CAT", "REALP.EXP", "REALP.INDTE", "REALP.EMAIL", "REALP.REG#", "TRADEADR.LINE2", "TRADEADR.LINE3", "TRADEADR.CITY", "TRADEADR.STATE", "TRADEADR.ZIP", "TRADEADR.COUNTY"]


TABLEA = "a"
TABLEB = ""

DataMap = {}

def Delegator(arrayOfElem, SearchCol, TABLEA, TABLEB, df, df2,found):                                                           # Function to create parallele analysis on batch of processes
    pool = mp.Pool(processes = os.cpu_count())                                                                                  #Creating processes based off number of system cpu's
    num = 0                                                                                                                     # Each process has a number associated with it for debugging                        
    remaining = mp.Manager().list()                                                                                             # Keeping list of proceeses that are still running for debugging
    for elm in arrayOfElem:                                                                                                     #Looping through batch and creatig process for each

         pool.apply_async(KeyValidator, args=(elm, SearchCol, TABLEA, TABLEB, df, df2,found, num,remaining,))
         remaining.append(str(elm))
   #      KeyValidator(elm, SearchCol, TABLEA, TABLEB, df, df2,found, num, remaining)
         print("		creating process for: " + str(elm) + ": " + str(num))
         num+=1
    pool.close()                                                                                                                #Closing and joining pool
    pool.join()

    if found:
        return str(found[0])                                                                                                    #Returning found element, otherwise return cont
    else:
        return "cont"



def KeyValidator(elm, SearchCol, TABLEA, TABLEB, df, df2, found, num,remaining):                                                #Function to check if keys are valid solution

    if found:
        counter = 0
        while found[3] > 10000000:                                                                                              #Checkig if there are more than 10 millions lines being analyized, if so, just wait for memory issues and limits
            time.sleep(30)
            print("still waiting, this is the current size:: " +str(found[3]))
            counter += 1
            continue

    else:                                                                                                                       #If this the first element of the batch for tables then set up the shared memory list 
        found.append("cont")
        found.append(0)
        found.append(0)
        found.append(0)

    unq = 0
    KeyCol = []
    AKeys = []
    BKeys = []
    for subTup in elm:                                                                                                          # Seperating the indivudal keys into either table A keys or keys for table B
        if subTup[0].split(".")[0] == TABLEA:
            AKeys.append(subTup[0])
            BKeys.append(subTup[1])
        else:
            AKeys.append(subTup[1])
            BKeys.append(subTup[0])
        
        KeyCol.append(subTup[0])                                                                                                #Creating array of all keys
        KeyCol.append(subTup[1])


    for col in AKeys:                                                                                                           #Looping through A and B keys columns to fill any empty rows with "A" or "B", this precents poor match ups caused by null values matching
        df[col] = df[col].fillna("a")
        df[col] = df[col].replace(r'^\s*$', "a", regex=True)
    for col in BKeys:
        df2[col] = df2[col].fillna("b")
        df2[col] = df2[col].replace(r'^\s*$', "b", regex=True)    


    df2.set_index(BKeys)                                                                                                        #Setting the index of table a and table b to their respective keys
    df.set_index(AKeys)


    new = df.merge(df2,left_on=AKeys, right_on = BKeys,  how='inner')                                                           #Merging on the keys to create the joint table new

    length = len(new.index)                                                                                                     # Getting length of the new table

    if length == 0:                                                                                                             # If the length of the table is 0 then theres no correlation between columns 
        print(str(elm) + "	Failed: " + str(num))
        remaining.remove(str(elm))
        found[3] -= new.size
        return

    new = new.iloc[:10000, :100]                                                                                                #Limiting new's size due to mememory errors
    new = new.drop_duplicates()                                                                                                 #Getting rid of duplicates as their not needed for this comparison
    found[3] += new.size                                                                                                        # incrementing the total size of the batch process



    for col in AKeys:
        unq += new[col].nunique()                                                                                               
    for col in BKeys:
        unq += new[col].nunique()

    unq = unq / (len(AKeys) + len(BKeys))                                                                                       # Averaging number of unique values the key columns have


    DupMap = {}
    fucked = False

    SearchCol = []

    for col in df2:                                                                                                             # Creating array of all columns betwee the two tables
        SearchCol.append(col)
    for col in df:
        SearchCol.append(col)



    for index, row in new.iterrows():                                                                                           # looping through the rows of the table

        val = ""
        key = ""
        lineDict = {}

        for col in SearchCol:                                                                                                   # Creating the value for a key value pairing based off the key columns and non-key columns
            if col not in KeyCol:
                lineDict[col] = str(row[col]).strip()
        
        for col in KeyCol:
            key += str(row[col]).strip()
        
        if key not in DupMap:                                                                                                   # Adding key value pair to a dictionary
            DupMap[key] = lineDict

        else:
            for subLine in DupMap[key]:
                if DupMap[key] != lineDict:                                                                                     # If the key already exists and the values don't match up, we need to see how close they are
                    if not MapDiffChecker(lineDict, DupMap[key]):
                       print(str(elm) + " FAILED: " + str(num))                                                                 # if over a 40% discrepency, then it's too much variation and this fails 
                       found[3] -= new.size                                                                                     #Subtracting size from batch
                       remaining.remove(str(elm))
                       return
                    else:
                        DupMap[key] = lineDict                                                                                  # if its close enough replace the current with it (There should be a better system where I average it out )
    if found:
        if found[1] * found[2] < unq *  length:                                                                                 # if we made it through checking all the lines we check if the current solution has a higher product of table length and uniqueness that the old solution
            print(str(elm) + " SUCCSESS ( BETTER ): "  +str(num))
            found[0] = str(elm)
            found[1] =  unq
            found[2] =  length
            found[3] -= new.size
            remaining.remove(str(elm))
            return
        else:                                                                                                                   # if it doesnn't ust upate the batch size
            found[3] -= new.size
            print("Found solution but it wasn't beter: " + str(num))
            remaining.remove(str(elm))
            return
    else:                                                                                                                       # if the element is the first solution found
        found.append(str(elm))
        found.append(unq)
        found.append(length)
        found.append(new.size)
        print(str(elm) + " SUCCESS (FIRST ) - LEN: " + str(length) + " -  UNQ: " + str(unq) + " SIZE: "  + str(new.size)+ " =  len(" +str(len(new)) + ") * width(" + str(len(new.columns)) + "): " + str(num) )
        found[3] -= new.size
        remaining.remove(str(elm))
        return

def DataFrameCreator(TABLEA, TABLEB):                                                           # This creates the tables from csv files stored in a folder
    df = pd.DataFrame()
    df2 = pd.DataFrame()

    df = pd.read_csv("inputFiles/" + TABLEA + ".csv", dtype = object)
    df2 = pd.read_csv("inputFiles/" + TABLEB + ".csv", dtype = object)

    df = df.iloc[:10000, :50]
    df2 = df2.iloc[:10000, :50]                                                                 #Limiting size for efficeiny and emory reasons

    dict1 = {}                                                                                  # Setting column names to the tablleName.ColName, this is so they don't get confused when merging
    for col in df:
        dict1[col] = TABLEA + "." + col

    dict2 = {}
    for col in df2:
        dict2[col] = TABLEB + "." + col


    df = df.rename(columns = dict1)
    df2 = df2.rename(columns = dict2)

    return df, df2


def MapDiffChecker(map1, map2):                                                                 #Function for checking the line difference for key Validaot r
    length = len(map1)
    bad = 0

    
    for col in map1:
        if map1[col] and map2[col]:
            if not pd.isna(map1[col]) and not pd.isna(map2[col]) and (map1[col] != "nan" and map2[col] != "nan"):
                if map1[col] != map2[col]:                                                      # Coutnting how many columns don't match up
                    bad +=1

    if bad > 0.5*length:                                                                        # if its worse tyhan 50%, then they don't match so return false
        return False
    else:
        return True

def Paralizer(TABLEA, TABLEB, DataMap):                                                         # Creating batches of possible combinations to paralize
    newFile = open("noCol.txt", 'r')

    df = pd.DataFrame()
    df2 = pd.DataFrame()

    cont = "cont"
    df,df2 = DataFrameCreator(TABLEA, TABLEB)
    noArray = []
     
    for line in newFile:
        noArray.append(line.strip())                                                            # Creating array of columsn deemed not good enoughg because they didn't containn over 5 unique values
    
    newFile.close()
    finalA = []
    finalB = []
    
    for col in df:
        
        if (col) not in noArray:
            finalA.append(col)
    
    
    for col in df2:
        if (col) not in noArray:
            finalB.append(col)



    c = list(itertools.product(finalA, finalB))                                                 # Creating set product of columns

    yes = True
    StatementsToTest = []
    midStep = ""

    manager = mp.Manager()
    found = manager.list()
    # create empty list to store the 
    for L in range(0, len(c)+1):
        
        if  L < 4:
        
            print("Finding all combos of length: " + str(L))
            for subset in itertools.combinations(c, L):                                     # Finding all combinations of all possible lenghts of combinations up to length of 3

                if  subset:
                    for elm in subset:
                        if DataMap[elm[0]] != DataMap[elm[1]] or ( "date" in DataMap[elm[0]]) or ("date" in DataMap[elm[1]]):       # Not using columns that are dates and that data types don't match up
                            yes = False
                    
                    if(yes):
                        if len(StatementsToTest) < 500:                                     # Creating batch of 500
                            if subset not in StatementsToTest:
                                print("adding: " + str(subset))
                                StatementsToTest.append(subset)
                        else:
                            midStep = Delegator(StatementsToTest, SearchCol, TABLEA, TABLEB, df, df2,found)
                            if midStep != "cont":
                                cont = midStep

                            StatementsToTest = []
                            
                    else:
                        yes = True

    if StatementsToTest:
        midStep = Delegator(StatementsToTest, SearchCol, TABLEA, TABLEB, df, df2, found)    # Calling remaingi statements
        if midStep != "cont":
            cont = midStep

        StatementsToTest = []                

    print("		FINAL ANSW: " + str(cont))
    if cont != "cont":                                                                      # If something other than cont was returned, this is the best soltuion and we'll write it to a file 
        fileToWrite = open("NEWTABLEOUT/" + TABLEA + ":" + TABLEB + ".txt", 'w')
        fileToWrite.write(str(cont) + "\n")
        print("WRITING: " + str(cont))
        fileToWrite.close()
    else:
        fileToWrite = open("NEWTABLEOUT/" + TABLEA + ":" + TABLEB + ".txt", 'w')
        fileToWrite.write(" ")
        fileToWrite.close()
        print("NO RELATIONS FOUND")

   





def main():
    global TABLEA
    global TABLEB
    global df
    global df2
    global DataMap

    
    
    if not os.path.exists("NEWTABLEOUT"):                                                                                                                      #Creating output folder if it doesnt already exist 
        os.makedirs("NEWTABLEOUT")
    
    
    dataTypes = open("DataTypes.txt", 'r')


    for line in dataTypes:
        TableAndColumn = line.split("=")[0].strip()
        dType = line.split("=")[1].strip()
        DataMap[TableAndColumn] = dType
  
    dataTypes.close()

    ComparedTables  =[]

    for filename in os.listdir("inputFiles"):                                                                                                               # looping through all comparisions between tables 
        for subFileName in os.listdir("inputFiles"):
            if subFileName != filename and (str(subFileName + filename) not in ComparedTables) and (str(filename + subFileName) not in ComparedTables):     # To avoid duplicate comparisons
                    
                    ComparedTables.append(str(filename + subFileName))
                    TABLEA = filename.split(".")[0]
                    TABLEB = subFileName.split(".")[0]
                    if not path.exists("NEWTABLEOUT/" + TABLEA + ":" + TABLEB + ".txt"):
                        print("Creating pocess for: " + TABLEA + " -> " + TABLEB)   
                        Paralizer(TABLEA, TABLEB, DataMap)                                                                                                  # Calling paralizer on the two tables 

                    else:
                        print("skipping: " + TABLEA + " -> " + TABLEB)                





if __name__ == '__main__':
  main()


