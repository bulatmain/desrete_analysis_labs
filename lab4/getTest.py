from patternAndTextGenerator import *
from bisect import bisect_right

class TestConfig:
    def __init__(self, patternSize, occurenceCount, occurenceRate):
        self.patternSize    = patternSize
        self.occurenceCount = occurenceCount
        self.occurenceRate  = occurenceRate

    def __str__(self):
        return f"Pattern size:    {self.patternSize},\
               \noccurence count: {self.occurenceCount},\
               \noccurence rate:  {self.occurenceRate}\n"

    def __repr__(self):
        return str(self)

def getTest(alphabet, testConfig):
    generator = PatternAndTextGenerator(alphabet)
    generator.setVariables(
        testConfig.patternSize, 
        testConfig.occurenceCount, 
        testConfig.occurenceRate
    )

    pattern          = generator.genPattern(testConfig.patternSize)
    patternPositions = generator.getPositions()
    text             = generator.genText(pattern, patternPositions)

    return pattern, patternPositions, text

def genLinePositions(textSize, linesCount):
    lineIndexes = set()
    lineIndexes.add(0)
    for i in range(linesCount):
        lineIndex = randint(1, textSize - 1)
        while lineIndex in lineIndexes:
            lineIndex = randint(1, textSize - 1)
        lineIndexes.add(lineIndex)
    return sorted(list(lineIndexes))


def lineifyTest(linePositions, patternPositions):
    linedPatternPositions = []
    for position in patternPositions:
        nextLineIndex = bisect_right(linePositions, position)
        currentLineIndex = nextLineIndex - 1
        linedPatternPositions.append([
            currentLineIndex + 1, 
            position - linePositions[currentLineIndex] + 1
        ])
    return linedPatternPositions

def saveTestToFile(filename, pattern, text, linePositions):
    with open(filename, 'w') as file:
        patternString = ''.join(str(letter) + ' ' for letter in pattern)
        file.write(patternString)
        start = 0
        for linePosition in linePositions:
            textPart = text[start:linePosition]
            start = linePosition
            textPartString = ''.join(str(letter) + ' ' for letter in textPart)
            file.write(textPartString + '\n')
