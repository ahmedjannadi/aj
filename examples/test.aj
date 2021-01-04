#test case string if statement

test = "notValid"

if (test != "isValid")
	print(test)
end

#test case append element to array 

testArray = [0,1,2,"string"]

testArray = append(testArray, 3)
elemToAdd = input("Insert element to add on array")
testArray = append(testArray, elemToAdd)

i=0
while(i < len(testArray))
	print(type(testArray[i]) + ": " + testArray[i])
	i++
end
