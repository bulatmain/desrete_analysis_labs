from getTest import *
import os
from functools import cmp_to_key

PATTERN_SIZE = 5
OCCURENCE_COUNT = 10
OCCURENCE_RATE = 0.3
LINES_COUNT = 5

EXEC_PATH = './build/exec'
TEST_FILENAME = 'test'
RESULT_FILENAME = 'result'

class AlphabetUINT64(Alphabet):
    UINT64T_MAX: int = 10 #int(2**32 - 1)

    def genLetter(self):
        return randint(0, AlphabetUINT64.UINT64T_MAX)


def genTestFile(testConfig: TestConfig, alphabet: Alphabet):
    pattern, patternPositions, text = getTest(alphabet, testConfig)

    linesCount = LINES_COUNT
    linePositions = genLinePositions(len(text), linesCount)
    linedPatternPositions = lineifyTest(linePositions, patternPositions)

    print(testConfig)
    print(f"Pattern generated:        {pattern},")
    print(f"Positions generated:      {patternPositions},")
    # print(f"Text generated with size: {len(text)}, \n{text}")
    # print(f"Line positions:           {linePositions}")
    print(f"Positions by lines:       {linedPatternPositions}")

    saveTestToFile(TEST_FILENAME, pattern, text, linePositions)

    return linedPatternPositions

def runExecByTest(execPath: str, testFilename: str, resultFilename: str):
    os.system(f'{execPath} < {testFilename} > {resultFilename}')

def readResultFromFile(resultFilename):
    with open(resultFilename, 'r') as file:
        result = []
        for line in file:
            line, word = map(int, line.split(', '))
            result.append([line, word])
        result = sorted(result, key = cmp_to_key(lambda a, b: a[0] < b[0] and a[1] < b[1]))
        return result

def checkResult(resultFilename, wanted):
    result = readResultFromFile(resultFilename)
    leastSize = min(len(result), len(wanted))

    fucked: bool = False
    for i in range(leastSize):
        if result[i] != wanted[i]:
            print(f"WA: line: {i}, \nresult: {result[i]}, \nwanted: {wanted[i]}")
            fucked = True
    if not fucked:
        print("Test passed!")
    

testConfig = TestConfig(
    PATTERN_SIZE,      
    OCCURENCE_COUNT,
    OCCURENCE_RATE
)

alphabet = AlphabetUINT64()


linedPatternPositions = genTestFile(testConfig, alphabet)
runExecByTest(EXEC_PATH, TEST_FILENAME, RESULT_FILENAME)
result = readResultFromFile(RESULT_FILENAME)
checkResult(RESULT_FILENAME, linedPatternPositions)


