#!/usr/bin/env python
# coding: utf-8

import numpy as np
import pandas as pd
import pymssql 
import csv
from TransformFunctions import *
sys.path.append('./ForeignKey')
from keyParser import *
from pathlib import Path
import os
import multiprocessing as mp
import time
import json

HOST = 'localhost'
USERNAME = 'hgilbert'
PASS = '8kx5jY3x7dXG'
DB = 'DLLR'
SCHEMA = 'dbo'
DATA = "MRECDataMappingMap.csv"
PICKMAP = "PickTransform.csv"
ERROROUTPUT = open("ErrorOutput.txt", "w")
TOTALREAD = 0
TOTALOUT  = 0

ApiNameMapping = {}                                                                                                                                             #Declaring global dictionaries
TransMap = PickTransMaker(PICKMAP)
ApiLinkMap = {}
TableMapping = {}
Api2Dest = {}
Required = {}
SQLColToAPI = {}


conn = pymssql.connect(HOST, USERNAME, PASS, DB)  

def TableCreation(DestDict):
    global TransMap
    global ERROROUTPUT
    global TOTALREAD

    for name in DestDict:                                                                                                                                       #Looping through different table names
        print("WORKING ON TABLE " + name)
        
        column = []
        fromTable = []
        col = ""
        source = ""

        for apiName in DestDict[name]: 
            if apiName in TableMapping:
                col, source = TableMapping[apiName]
                column.append(col[0])
                fromTable.append(col[1])
                fromTable = list(dict.fromkeys(fromTable))
     
        
        if fromTable:
            finalStatement = StatementCreator(fromTable, column )
            if finalStatement:
                print(finalStatement)
                #try:   
                QATesting(finalStatement)
                exit()
                DestDict[name] = pd.read_sql_query(finalStatement,conn)
                DestDict[name] = DestDict[name].drop_duplicates()
                print("\n\n")

                #except:
                    #print("query failed")

                columnDict = columnDictionaryCreator(DestDict[name])
                DestDict[name] = DestDict[name].rename(columns = columnDict)
            

                
            for apiName in DestDict[name]:

                if apiName in TableMapping:                                                                                                                         #Checking we have a valid statement for that column
                    statement, SourceTrans = TableMapping[apiName]

                    reqData = ""
                    if apiName in Required:
                        reqData =  Required[apiName]
                    
                    if SourceTrans == "pick":                                                                                                                   #Checking if its a picklist transformation
                        DestDict[name][apiName] = TransformPicker(SourceTrans, DestDict[name][apiName], reqData, TransMap[apiName])                 #Setting the column of the table equal to the SQL query after it's been parsed and transformed 
                    else:
                        DestDict[name][apiName] = TransformPicker(SourceTrans, DestDict[name][apiName], reqData) 
                        

                elif apiName in Required:                                                                                                                             #Checking if we didn't have a statement for the data, but the data was requried regardless
                    if Required[apiName] == "PHONE":                                                                                                                  #Setting corresponding columns to appropraite garabge data
                        DestDict[name][apiName] =  "111-111-1111"
                    if Required[apiName] == "SSN":
                        DestDict[name][apiName] =  "111-11-1111"
                    if Required[apiName] == "TEXT":
                        DestDict[name][apiName] =  "Garbge name"
                    if Required[apiName] == "EMAIL":
                        DestDict[name][apiName] =  "Gargabge@email.com"
                    if Required[apiName] == "ZIP":
                        DestDict[name][apiName] =  "11111"
                    if Required[apiName] == "DATE":
                        DestDict[name][apiName] =  "11/11/1111"
                    if Required[apiName] == "PICK":
                        DestDict[name][apiName] =  "Transformation does not exists"
            
            
        
        ToCSV(DestDict)
            
    """                                                                                                                      #Looping through all the columns in the table sent over
        if apiName in TableMapping:                                                                                                                         #Checking we have a valid statement for that column
            statement, SourceTrans = TableMapping[apiName]                                                                                                  #Grabbing SQL statement and transformation for that apiname
            
            reqData = ""
            if apiName in Required:
                reqData = Required[apiName]
            
            try:                                                                                                                                           #Trying execution statement to cath SQL error
                DataFrameObj = pd.read_sql_query(statement,conn)                                                                                            #Creating new DataFrame object off sql query
                        
                if SourceTrans == "pick":                                                                                                                   #Checking if its a picklist transformation
                    DestDict[Api2Dest[apiName]][apiName] = TransformPicker(SourceTrans, DataFrameObj.iloc[:,0], reqData, TransMap[apiName])                 #Setting the column of the table equal to the SQL query after it's been parsed and transformed 
                else:
                    DestDict[Api2Dest[apiName]][apiName] = TransformPicker(SourceTrans, DataFrameObj.iloc[:,0], reqData)                                    #If apiname is not in the picklist mapping, call same command but do not pass in TransMap         

                print("     SUCCSES WITH STATEMENT: " + statement)          
            except:
                print("     ERROR WITH STATEMENT: " + statement)                                                                                            #Catching error and printing out message if execution failed
        
        
        elif apiName in Required:                                                                                                                             #Checking if we didn't have a statement for the data, but the data was requried regardless
            if Required[apiName] == "PHONE":                                                                                                                  #Setting corresponding columns to appropraite garabge data
                DestDict[Api2Dest[apiName]][apiName] =  "111-111-1111"
            if Required[apiName] == "SSN":
                DestDict[Api2Dest[apiName]][apiName] =  "111-11-1111"
            if Required[apiName] == "TEXT":
                DestDict[Api2Dest[apiName]][apiName] =  "Garbge name"
            if Required[apiName] == "EMAIL":
                DestDict[Api2Dest[apiName]][apiName] =  "Gargabge@email.com"
            if Required[apiName] == "ZIP":
                DestDict[Api2Dest[apiName]][apiName] =  "11111"
            if Required[apiName] == "DATE":
                DestDict[Api2Dest[apiName]][apiName] =  "11/11/1111"
            if Required[apiName] == "PICK":
                DestDict[Api2Dest[apiName]][apiName] =  "Transformation does not exists"
                
     """
    
        
def ToCSV(DestDict):

    for name in DestDict:
        DestDict[name].to_csv("OutputFolder/" + name + ".csv")

        
def StatementCreator(fromTable, column ):
    
    finalStatement = ""
    partOne = ""
    partTwo = ""

    for col in column:
        if "+" in col:
            partOne += col + " AS '" + col.split("+")[0] + "', "
        else:
            partOne += col + " AS '" + col + "', "
    partOne = partOne[:-2]
    
    for col in fromTable:
        partTwo += col + ", "
    partTwo = partTwo[:-2]

        

    finalStatement += " SELECT " + partOne + " FROM " + partTwo
    for i in range(len(fromTable)):
        fromTable[i] = fromTable[i].split(".")[1]
    
    statement = ""
    tables = []

    if len(fromTable) == 2:
        statement, tables = Request(fromTable[0], fromTable[1])
        if statement:
            finalStatement += " where " + statement
        else:
            statement, tables = Request(fromTable[1], fromTable[0])
            finalStatement += " where " + statement
    
    elif len(fromTable) > 2:

        for i in range(len(fromTable)):
            connect = "" 
            counter = 0
            for k in range(len(fromTable)):
                
                if fromTable[i] != fromTable[k]:
                    if Request(fromTable[i], fromTable[k]):
                        statement, miniTables = Request(fromTable[i], fromTable[k])
                        tables = tables + miniTables
                        tables = list(dict.fromkeys(tables)) 
                        connect += statement + " and "
                        counter += 1
        
            

            if counter == len(fromTable) - 1:
                break
        finalStatement += " where " + connect[:-4]

    return TableAdder(finalStatement, tables)

def TableAdder(finalStatement, tables):
    for table in tables:
        afterFrom = finalStatement.split("FROM")[1]
        afterFrom = afterFrom.split("where")[0]
        if table not in afterFrom:
            section1 = finalStatement.split("where")[0]
            section2 = finalStatement.split("where")[1]
            finalStatement = section1 + ", " + table + " where " + section2
    
    return finalStatement

def columnDictionaryCreator(model):
    newDict = {}
    col = ""
    for col in model:
        newDict[col] = SQLColToAPI[col]
    
    return newDict


    

                

                

def Printer(DestDict):
        for col in DestDict:
            if col in TransMap:
                print("HAS A PICK LIST")
            print(col)
            print("      " + DestDict[col])

            



def InputGetter():                                                                                                                                              #Fucntion to get possible new inputs
    global HOST                                                                                                                                                 #Declarig the globals
    global USERNAME
    global PASS
    global DB
    global PICKMAP
    global SCHEMA
    predefined = input("Would you like to use preset data for MREC (Y/N)")                                                                                      #prompting if they would like to use the MREC values (for us)

    if(predefined == "Y"):                                                                                                                                      #if yes, don't change anything
        return
    else:
        HOST = input("Please enter DB Host name: ")                                                                                                             #Otherwise, get new input values
        USERNAME = input("Please enter Username: ")
        PASS = input("Please enter Password: ")
        DB = input("Please enter DB name: ")
        SCHEMA = input("Please enter DB name: ")
        DATA = input("Please enter filename for mappings: ")
        PICKMAP = input("Please enter filename for picklist mapping: ")


def ApiCaller(DestModelArray, DestDict):                                                                                                                        #Fucntion to write input output discrepency to file
    global TOTALOUT                                                                                                                                             #Declaring globals
    global ERROROUTPUT
    global TOTALREAD


    for model in ApiLinkMap:
        DestDict[model] = DestDict[model].assign(record_holder='hcgilbet@gmail.com')
        Printer(DestDict[model])
                                                                                                                                                                  #Looping through distinct model names
        if model in ApiLinkMap:
            print("TRYING TO DO: " + model)
            listOfCodes = DataFrame_to_record(DestDict[model])                                                                                                      #Converting data frame model to JSON object
            listOfOutputs = []                                                                                                                                      #Initializing array and varaibles
            outputsCount = 0

            for item in listOfCodes:                                                                                                                                #Looping through rows of data
                if outputsCount < 100:                                                                                                                              #If there are less than 100 records in the arary
                    listOfOutputs.append(item)                                                                                                                      #Adding row of data to array
                    outputsCount += 1

                else:                                                                                                                                               #Once we reach 100 
                    api_to_formability(listOfOutputs, ApiLinkMap[model])                                                                                            #Call API pusher
                    outputsCount = 0                                                                                                                                #Reset varaibles
                    listOfOutputs = []

            api_to_formability(listOfOutputs, ApiLinkMap[model])                                                                                                    # Final request for remaining records 
     
def MappingCreator(Master, DestDict):
    global ApiNameMapping
    global TableMapping
    global Api2Dest
    global Required

  

    for apiName in Master["DESTINATION API REFNAME"]:                                                                                                           #Looping through all the api names
        if(pd.isnull(apiName)==False):                                                                                                                          #Checking it's not a null value
            row = Master.loc[Master["DESTINATION API REFNAME"] == apiName]                                                                                      #Created mapping between every API name and its source column and source table
            ApiNameMapping[apiName] = row[['SOURCE TABLE', 'SOURCE COLUMN', 'TRANSFORM', 'REQUIRED']]                                                                       #Created mapping between every API name and its source column and source table and possible transformation                              
            for subName in row["DESTINATION MODEL"]:                                                                                                            #Creating a mapping between api names and the destination model
                if(pd.isnull(subName) == False):                                                                                                                #Checking the desintation model assoicated with the api name isn't null
                    Api2Dest[apiName] = subName                                                                                                                 #Creating the mapping between the apiname and its destination model

            for itName in ApiNameMapping[apiName]["SOURCE TABLE"]:                                                                                              #Looping to grab the value for source table
                SourceTable = itName                                                                                                                            #Grabbing value of source table
            for itColumn in ApiNameMapping[apiName]["SOURCE COLUMN"]:                                                                                           #Looping to grab the value for source column
                SourceColmn = itColumn                                                                                                                          #Grabbing value of source column
            for itTrans in ApiNameMapping[apiName]["TRANSFORM"]:                                                                                                #Looping to grab the value for transform
                SourceTrans = itTrans                                                                                                                           #Grabbing value of transform
            for itReq in ApiNameMapping[apiName]["REQUIRED"]:                                                                                                   #Looping to grab the value for required
                SourceReq = itReq
            
            if (pd.isnull(SourceReq)==False):                                                                                                       
                Required[apiName] = SourceReq                                                                                                                   #Getting value of required regardless of whether other fields are set
        
            if(pd.isnull(SourceTable)==False and pd.isnull(SourceColmn)==False):                                                                                #Checking that both the source table and source column are not false
                sources = SourceColmn.split(",")
                actualSourceCol = ""

                if len(sources) > 1:
                    for col in sources:
                        actualSourceCol += str(SourceTable) + "." + col.strip() + " + "
                    
                    actualSourceCol = actualSourceCol[:-2]
                else:
                    actualSourceCol = str(SourceTable) + "." + str(SourceColmn)

                statement = ((actualSourceCol , SCHEMA + '.' + str(SourceTable) ))
                
    
                if apiName in TransMap:                                                                                                                         #Checking if the apiname is a picklist transformation
                    SourceTrans = "pick"                                                                                                                        #Setting varaible so we know its a picklist transformation       

                TableMapping[apiName] = ((statement, SourceTrans))                                                                                              #if so storing the statement, and new source trans       
                SQLColToAPI[actualSourceCol.split("+")[0]]  = apiName      

            else:
                ERROROUTPUT.write("ERROR WITH LINE: " + "SELECT: " + str(SourceColmn) + " FROM: " + str(SourceTable) + "\n")                                    #If the api name was there, but either the source or destination was null, output error to file

            
    return TableMapping, Api2Dest, DestDict

def WSRemover(array):
    newArray = []
    for elm in array:
        newElm = elm.rstrip()
        newElm = newElm.lstrip()
        newArray.append(newElm)
    return newArray

    
def QATesting(statement):

    select = statement.split("where")[0]
    select = select.replace("SELECT", "")
    where = statement.split("where")[1]
    
    whereArray = WSRemover(where.split("and"))
    fromArray = WSRemover(select.split("FROM")[1].split(","))
    columnsArray = WSRemover(select.split("FROM")[0].split(","))


    for touple in whereArray:
        for elm in touple.split("="):
            elm = elm.lstrip()
            elm = elm.rstrip()
        
            if (elm + " AS '" + elm + "'") not in columnsArray:
                columnsArray.append(elm + " AS '" + elm + "'")

    finalStatement = "SELECT "
    for col in columnsArray:
        finalStatement += col + ", "
   
    finalStatement = finalStatement[:-2] + " FROM "

    for col in fromArray:
        finalStatement += col + ", "
    
    finalStatement = finalStatement[:-2] + " WHERE " + where

    print("new final statement: " + finalStatement)

    testingDf = pd.read_sql_query(finalStatement,conn)
    testingDf = testingDf.drop_duplicates()

    testingDf = TestingTableCreator(testingDf)
    print(testingDf)
    tableVerifier(testingDf, finalStatement, whereArray)


def tableVerifier(df, finalStatement, whereArray):
    keyCol = []
    for tup in whereArray:
        keyCol.append(tup.split("=")[0].lstrip().rstrip())
        keyCol.append(tup.split("=")[1].lstrip().rstrip())

    counter = 1
    for index, row in df.iterrows(): 
        newWhereCond = ""
        for col in df:
            if col.split("AS")[0].lstrip().rstrip() in keyCol:
                newWhereCond += col.split("AS")[0].lstrip().rstrip() + " = " + str(row[col]).lstrip().rstrip() + " and "
        
        newWhereCond = newWhereCond[:-4]
        newStatement = (finalStatement + " and " +  newWhereCond).replace(".0", "")

        testingLine = pd.read_sql_query(newStatement,conn)
        passed = False
        for col in df:
            for subRow in testingLine[col]:
                if row[col] == subRow:
                    passed = True

        if passed:
            print(" ALL GOOD: (" + str(counter) + "/" + str(len(df.index)) + ")")
        else:
            print("fucked up")
            exit()
        counter += 1


        #print(newStatement + "\n\n\n")
            





def TestingTableCreator(currentDF):
    
    size = len(currentDF.index)

    copyColumns = list(currentDF.columns)

    copyDF = pd.DataFrame(columns = copyColumns)
    subDataFrames = []
    
    tenPer = int(0.1 * size)
    QuaterPer = int(tenPer/40)


    constTen = tenPer
    previousStart = 0
    if tenPer != 0:
        for i in range(10):

            newDF = currentDF.iloc[previousStart:tenPer]
            subDataFrames.append(newDF.head(QuaterPer))
            subDataFrames.append(newDF.tail(QuaterPer))

            previousStart = tenPer 
            tenPer += constTen

        newDF = pd.concat(subDataFrames)
        return newDF
        



def Main():
    global TableMapping
    global Api2Dest
    global ApiLinkMap

    DestDict = {}
    Master = pd.read_csv(DATA)                                                                                                                                  #Reading data from CSV file
    SubMaster = pd.read_csv(PICKMAP)
    DestModelArray = []  
                                                      

      
    if not os.path.exists("OutputFolder"):                                                                                                                      #Creating output folder if it doesnt already exist 
        os.makedirs("OutputFolder")      

    for row in Master["DESTINATION MODEL"]:                                                                                                                     #Putting all the desination model names into an array
        if(pd.isnull(row) == False):
            DestModelArray.append(row)                                                                                                                          #Capuring all names of destination models

    DestModelArray = list(dict.fromkeys(DestModelArray))                                                                                                        #Deleteing duplicates so I have array of unique destination model names
   
    for name in DestModelArray:                                                                                                                                 #Creating mapping between destination model names and a new DataFrame object
        d = Master.loc[Master["DESTINATION MODEL"] == name]
        DestDict[name] = pd.DataFrame(columns = d["DESTINATION API REFNAME"])
        
        rows = SubMaster.loc[SubMaster["DEST MODEL"] == name]                                                                                                   #Creating mapping between dest model and api link
        for row in rows["API LINK"]:
            if(pd.isnull(row) == False):
                ApiLinkMap[name] = row  


    TableMapping, Api2Dest, DestDict = MappingCreator(Master, DestDict)                                                                                                                           

    TableCreation(DestDict)
    
    #ApiCaller(DestModelArray, DestDict)                                                                                                                         #Wrtiting to error Fole
    conn.close() 





    
Main()







