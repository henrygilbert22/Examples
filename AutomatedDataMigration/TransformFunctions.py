import numpy as np
import pandas as pd
import json
import sys
import requests
JSONWRITEOUT = open("JsonOutput.json", "w")
VALIDOBJ = open("validObj.txt", "a")
INVALIDOBJ = open("invalidObj.txt", "a")




def PickTransMaker(filename):                                                                                                       #Creating nested mapping for pick list transformations
	Master = pd.read_csv(filename)                                                                                                  #Reading from picklist master file
	UniqueApiNames = []                                                                                                             #Initializing array for unique api names

	for apiName in Master["API REF NAME"]:
		UniqueApiNames.append(apiName)                                                                                              #Pushing all api names into array


	UniqueApiNames = list(dict.fromkeys(UniqueApiNames))                                                                            #Deleting duplicates
	MainMap = {}                                                                                                                    #Initialzing outer map. Api name will be mapped to a mapping of source and Dest

	for uniqueApiName in UniqueApiNames:                                                                                            #Looping through Api names
		rows = Master.loc[Master["API REF NAME"] == uniqueApiName]                                                                  #Grabbing all the rows that correspond with the api name
		MiniMap = {}                                                                                                                #Initialzing the mini map to store mapping between source and dest
		source = rows["SOURCE"]                                                                                                     #Grabbing all the columns that are associated with the api name

		for row in source:                                                                                                          #Looping through the sources
			if(pd.isnull(row) == False):                                                                                            #Making sure there are actually values associated with it

				dest = rows.loc[rows['SOURCE'] == row]["DESTINATION"]                                                               #Getting the corresponding Dest for the Source
				NewRow = ""                                                                                                         #Initialzing variables to store source and Dest in
				NewDest = ""
				for subRow in row:                                                                                                  #Nesseary to get actual value because row and dest are still both ROW objects
					NewRow += subRow
				for subDest in dest:
					NewDest += subDest

				MiniMap[NewRow] = NewDest                                                                                           #Creating mapping between the source and Dest


		MainMap[str(uniqueApiName)] = MiniMap                                                                                       #Creating mapping between api name and sub mapping of all its source and dest


	return MainMap


def DataFrame_to_record(inputFrame):
																																	# load the CSV, convert it to json, then build a python dictionary from the json
	df = inputFrame
	convertToJson = df.to_json(orient='records')                                                                                    #build the json record-wise rather than column-wise
	convertToDict = json.loads(convertToJson)
																																	# go through each record and replace each value with the object {'value': XYZ} just like Formability's API expects
	for record in convertToDict:
		for column in record:
			value = record[column]
			record[column] = {'value' : value}
																																	# convert back to json string for saving to a file
	return convertToDict




def api_to_formability(listOfRecords, urlSegment, count = 0):
	
	url = 'https://maryland.formability-staging.enovationallabs.com/api/v2/object_models/' + str(urlSegment) + '/records/'			#Creating URL based on object model id

	jsonRecords = '{ "records": ' + json.dumps(listOfRecords) + "}"																	#Creating JSON object

	headers = {																														#Creating header
		'Authorization': 'Bearer 433d6315912c5bed2ba4b2279d11c436 +JAV3JX9AvqMfiJSa8Zl0F5/QElyVPRQ1aTKMiGoImo=',
		'Content-Type': 'application/json',
		'current-user-id': '5d372903dd901e00f8e9818b'
		}

	response = requests.request("POST", url, headers=headers, data=jsonRecords)														#Calling API and capturing response

	print("----------------------")

	print("Response: ")
	print(response.text + "\n\n\n")																									#Printing response

	try:
		data = json.loads(response.text)																							#Loading response into JSON object

		if "valid" in data:
			print("VALID: ")																										#Pritning valid object id's
			for validObj in data["valid"]:
				print("     " + validObj["id"])
				VALIDOBJ.write(validObj["id"] + "\n")

		if "invalid" in data:
			print("\n\n INVALID: ")																									#Printing invalid object id's
			for invalidObj in data["invalid"]:
				print("     " + invalidObj["id"])
				INVALIDOBJ.write(invalidObj["id"] + "\n")
	except:
		
		if count < 10:
			print("STRING WAS NOT JSON - RUNNING SAME DATA AGAIN")
			count += 1
			api_to_formability(listOfRecords, urlSegment, count)
		else:
			print("TRIED 10 TIMES BUT DATA NOT TAKEN, OUTPUTTING TO FILE")
			INVALIDOBJ.write(str(listOfRecords))
			INVALIDOBJ.write("RESPONSE: \n")
			INVALIDOBJ.write(response.txt)




def TransformPicker(trans, data, reqData, miniMap={}):                           														#This is the 'case switch' for each different transformation
	
	if (trans == "SSNTrans"):                                     																		#Value is chosen based off what the row passed in and the function returns the corresponding transformed data table
		return SsnPrepadding(data, reqData)
	elif (trans == "pick"):
		return  PickListTransform(data, miniMap, reqData)
	elif (trans == "DateTrans"):
		return DateTrans(data, reqData)
	elif (trans == "PhoneTrans"):
		return  PhoneTrans(data, reqData)
	elif (trans == "ZipTrans"):
		return ZipTrans(data,reqData)
	elif (trans == "EmailTrans"):
		return EmailTrans(data, reqData)
	else:
		return StripData(data,reqData)                                                     														#Returns orgional dataet if transformation is blank


def SsnPrepadding(data, reqData):                                                														#Padding SSN numbers that are less than 9 char with leading 0's
	CleanedSsn = []                                                     																#Creating new array to hold cleaned numbers
	for ssnNumber in data:                                              																#Looping through given dataset
		
		if((pd.isnull(ssnNumber)==True or str(ssnNumber).isspace()== True) and reqData == "SSN"):
			CleanedSsn.append("111-11-1111")
		
		elif (pd.isnull(ssnNumber)==False and str(ssnNumber).isspace()== False):
			
			if(len(str(int(ssnNumber))) != 9):                              															#Checking if it the length doesn't = 9
				PassedSSN = str(int(ssnNumber)).zfill(9).strip()          																#Filling it with 0's up until a length of 9 and put it into our array
			else:
				PassedSSN = str(int(ssnNumber)).strip()                     															#If the lenght was 9, do nothing and put into our array
			
			sec1 = str(PassedSSN)[0:3]                                        															#Grabbing year month and date based off predefined index
			sec2 = str(PassedSSN)[3:5]
			sec3 = str(PassedSSN)[5:9]
				
			CleanedSsn.append(sec1 + "-" + sec2 + "-" + sec3)
		else:
			CleanedSsn.append("")

	return pd.Series(CleanedSsn)



def StripData(data,reqData):                                                																#Padding SSN numbers that are less than 9 char with leading 0's
	CleanedData = []                                                     															#Creating new array to hold cleaned numbers
	
	for element in data:                                              																#Looping through given dataset
		if (pd.isnull(element) == False and str(element).isspace() == False):
			CleanedData.append(str(element).strip())
		elif reqData == "TEXT":
			CleanedData.append("Required Field Left Blank")
		else:
			CleanedData.append(str(element))

	return pd.Series(CleanedData)


def PickListTransform(data, miniMap, reqData):                                   													#Function to transfrom value to correct pick list value
	CleanedSsn = []                                                     															#Initialzing array for cleaned data
	for subData in data:                                                															#Looping through all the data
		if subData in miniMap and pd.isnull(subData) == False:  	                                       															#Checking if we have a mapping for the input
			print(subData)
			CleanedSsn.append(str(miniMap[subData]).strip())                    													#if so, put correct mapping into the array
		else:
			CleanedSsn.append("Transform does not exist")          																	#If we do not have the mapping, instead write this

	return pd.Series(CleanedSsn)

def DateTrans(data, reqData):                                                    													#Date transformation function
	CleanedDates = []                                                     															#Initializing array to hold cleaned data in it
	for subDate in data:                                                															#Iterating through the given dates

		if ((pd.isnull(subDate) == True or str(subDate).isspace() == True) and reqData == "DATE"):
			CleanedDates.append("11/11/1111")
		
		elif(pd.isnull(subDate) == False and str(subDate).isspace() == False):
			actDate = str(subDate)                                          														#Capturing string version of date
			actDate = actDate.replace("-","")                               														#if '-' exists, delete it

			year = str(actDate)[0:4]                                        														#Grabbing year month and date based off predefined index
			month = str(actDate)[4:6]
			day = str(actDate)[6:8]

			date = day+"/"+month+"/"+year
			CleanedDates.append(date.strip())                       																#Creating new string to hold create format
		else:
			CleanedDates.append(str(subDate))
	
	return pd.Series(CleanedDates)                                        															#Returning array as a pd series

def ZipTrans(data, reqData):
	CleanedData = []

	for element in data:		
		if (pd.isnull(element) == False and str(element).isspace() == False):
			trunc = ""
			if (len(str(element)) == 9):
				part1 = str(element)[0:5]
				part2 = str(element)[5:9]
				trunc = part1 + "-" + part2
			else:
				trunc = str(element)[0:5]
		elif reqData == "ZIP":
			trunc = "11111"
		
		CleanedData.append(trunc)
	return pd.Series(CleanedData)


def PhoneTrans(data, reqData):                                                  													#Place holder function for other transformation
	CleanedData = []                                                     															#Creating new array to hold cleaned numbers
	for PhoneNum in data:                                              																#Looping through given dataset
		
		if ((pd.isnull(PhoneNum) == True or str(PhoneNum).isspace() == True) and reqData == "PHONE"):
			CleanedData.append("111-111-1111")
		elif (pd.isnull(PhoneNum) == False and str(PhoneNum).isspace() == False):
			sec1 = str(PhoneNum)[0:3]                                        														#Grabbing year month and date based off predefined index
			sec2 = str(PhoneNum)[3:6]
			sec3 = str(PhoneNum)[6:10]
			CleanedData.append(sec1 + "-" + sec2 + "-" + sec3)
		else:
			CleanedData.app(str(PhoneNum))

	return pd.Series(CleanedData)

def EmailTrans(data, reqData):
	CleanedData = []

	for email in data:
		if("@" not in str(email)):
			CleanedData.append("No-Email@Address.com")
		else:
			sub = str(email).split(" ")[0]
			CleanedData.append(str(sub).strip())
		
	return pd.Series(CleanedData)