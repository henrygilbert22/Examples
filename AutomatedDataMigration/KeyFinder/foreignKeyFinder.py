import numpy as np
import pandas as pd
import csv
from pathlib import Path
import os
import multiprocessing as mp



HOST = 'localhost'
USERNAME = 'hgilbert'
PASS = '8kx5jY3x7dXG'
DB = 'DLLR'
SCHEMA = 'dbo'





def uniqueFinder(BigDic):

  TableToUniqueCol = {}
  counter = 0

  for table in BigDic:
    
    print("SEARCHING: " + str(counter) + " - " + table)
    counter += 1

    uniqueCol = []
    for col in BigDic[table]:
      print(BigDic[table][col])
      if BigDic[table][col].is_unique and BigDic[table][col].empty == False :
        uniqueCol.append(col)
    
    if uniqueCol:
      TableToUniqueCol[table] = uniqueCol

  
  for table in TableToUniqueCol:
    print(table + ": " + str(TableToUniqueCol[table]))
  MatchFinder(TableToUniqueCol, BigDic)


def MatchFinder(TableToUniqueCol, BigDic):
  for table in TableToUniqueCol:
    for col in TableToUniqueCol[table]:
      array1 = BigDic[table][col].tolist()

      for subTable in TableToUniqueCol:
        if table != subTable:
          for subCol in TableToUniqueCol[subTable]:    
            array2 = BigDic[subTable][subCol].tolist()
            if((all(elem in array1  for elem in array2)) or (all(elem in array2  for elem in array1))):
              print("THERE IS A LINK BETWEEN " + table + " and " + subTable + " THOUGH: " + col + " in comparison with: " + subCol)
              #print(BigDic[table][col])
              #print(BigDic[subTable][subCol])



def DepthTableSearch(array1, tableName, colName):

  print("SEARCHING: " + tableName.split(".")[0])
  
  TableOut = open('OutputFolder/' + tableName.split(".")[0] + "." + colName + ".txt", 'a')
    
  for filename in os.listdir('inputFiles'):
    
    if filename != tableName:        
      innerDF = pd.read_csv('inputFiles/' + filename, dtype=object)

      for subCol in innerDF:
        print("     COMPARING: " + tableName.split(".")[0] + "." + colName + " to " + filename.split(".")[0]  + "." + subCol)     
        array2 = innerDF[subCol].tolist()
        
        if array1 and array2:
          if((all(elem in array1  for elem in array2)) or (all(elem in array2  for elem in array1))):
              TableOut.write("       " + tableName + "." + colName + " = " + filename.split(".")[0] + "." + subCol + "\n\n\n")

  print("   SEARCH OF " + tableName + "COMPLETED: ")
  
 


def multiProc():
 
  pool = mp.Pool(processes=os.cpu_count()) 
  count = 0
  for tableName in os.listdir('inputFiles'):
    
    outerDF = pd.read_csv('inputFiles/' + tableName, dtype=object)
    for col in outerDF:
      
      pool.apply_async(DepthTableSearch, args=(outerDF[col].tolist(), str(tableName), str(col), )) 
      DepthTableSearch(outerDF[col].tolist(), str(tableName), str(col))
      print("CREATING PROCESS FOR: "  + tableName + "." + col + "   " + str(count))
      count += 1
    
  
  pool.close()
  pool.join()

  

     



def main():

  """

  tableNames = []

  if not os.path.exists("ForOutputFolder"):                                                                                                                      #Creating output folder if it doesnt already exist 
        os.makedirs("ForOutputFolder") 

  conn = pymssql.connect(HOST, USERNAME, PASS, DB)  
  

  DataFrameObj = pd.read_sql_query("SELECT * FROM information_schema.tables",conn)


  for tableName in DataFrameObj["TABLE_NAME"]:
    if tableName not in tableNames:
      print("UPLOADING TABLE: " + tableName)

      query = pd.read_sql_query("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '" + tableName + "'", conn)
      
      colArray = []
      for col in query["COLUMN_NAME"]:
        colArray.append(col)
      
      tableNames.append(tableName)
      df = pd.DataFrame(columns = colArray)
      df = pd.read_sql_query("SELECT DISTINCT * FROM " + SCHEMA + "." + tableName, conn)
      
      with open('ForOutputFolder/' + tableName + '.csv', 'wb') as csvfile:
        df.to_csv(csvfile, encoding='utf-8', index=False)

"""
  if not os.path.exists("OutputFolder"):                                                                                                                      #Creating output folder if it doesnt already exist 
    os.makedirs("OutputFolder")
  
  multiProc()
  #conn.close() 


if __name__ == '__main__':
  main()