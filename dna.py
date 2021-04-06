# import modules
import csv
import sys


def main():
    # check for correct usage
    if len(sys.argv) != 3:
        print("Incorrect usage")

    # populate list of dictionaries "people" from csv file
    csvname = sys.argv[1]
    people = []
    with open(csvname) as csvfile:
        csvreader = csv.DictReader(csvfile)
        for row in csvreader:
            people.append(row)

    # read dna sequence into string "dna"
    txtname = sys.argv[2]
    with open(txtname) as txtfile:
        dna = txtfile.read()

    # populate list "keys" with keys from dictionaries in "people", pop off "name" key
    # "keys" is list containing each STR
    keys = list(people[0])
    length = len(keys)
    keys.pop(0)

    # create list "results", populate by calling count() to count number of each STR
    results = []
    for i in range(len(keys)):
        results.append(count(dna, keys[i]))

    # for each person, check if each element in "results" matches number of STRs from csv file
    # if match is found, add name to variable "match", set "found" to True
    match = ""
    found = False
    for i in people:
        matches = 0
        for j in range(len(keys)):
            if int(i[keys[j]]) == results[j]:
                matches += 1
        if matches == len(keys):
            match = i['name']
            found = True

    # if a match was found, print name in "match", otherwise print "No match"
    if found == False:
        print("No match")
    else:
        print(match)


# function to count the number of repeats of "short" (containing STR) in string "dna"
def count(dna, short):

    # set "string" to the STR to be counted
    string = short
    count = 0

    # if "string" is found in "dna", add one repeat of STR to "string", increment count by one
    while True:
        if string in dna:
            count += 1
            string = string + short
        # once "string" is not found in "dna", return count (will be 0 if "string" not found on first iteration)
        else:
            return count


if __name__ == "__main__":
    main()
