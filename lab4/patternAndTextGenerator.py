from random import randint
from math import ceil, floor

MAX_TEXT_SIZE = 15

class Alphabet:
    def genLetter(self):
        return Alphabet.genLetter()
    
    def genLetter():
        pass

class PatternAndTextGenerator:
    def __init__(self, alphabet: Alphabet):
        self.alphabet = alphabet
    
    def genPattern(self, size: int):
        return [self.alphabet.genLetter() for i in range(size)]

    def trySplit(self, place, patternPosition, patternSize):
        if place[1] - place[0] < patternSize:
            return [], []

        llEdge = patternPosition - patternSize
        # print(f"llEdge: {llEdge}, freespace: {llEdge - place[0]}")
        if llEdge - place[0] < 0:
            leftSide = []
        else:
            leftSide = [place[0], llEdge]
            assert leftSide[1] - leftSide[0] >= 0, \
                f'Бля, левая часть {leftSide} не должна быть пустой'

        rrEdge = patternPosition + patternSize
        # print(f"rrEdge: {rrEdge}, freespace: {place[1] - rrEdge}")
        if place[1] - rrEdge < 0:
            rightSide = []
        else:
            rightSide = [rrEdge, place[1]]
            assert rightSide[1] - rightSide[0] >= 0, \
                f'Бля, правая часть {rightSide} не должна быть пустой'

        return leftSide, rightSide


    def updateFreePlace(self, freePlace, placePosition, leftPart, rightPart):
        # print(f"left: {leftPart}, len(left): {len(leftPart)},\
            #   \nright: {rightPart}, len(right): {len(rightPart)}")
        # print(f"place position: {placePosition},\nfreePlace old: {freePlace}")
        assert placePosition < len(freePlace), "Я ебал, нет такого индекса"
        freePlace[placePosition]
        if len(leftPart) == 0 and len(rightPart) == 0:
            del freePlace[placePosition]
        else:
            if leftPart:
                freePlace[placePosition] = leftPart
                if rightPart:
                    freePlace.insert(placePosition + 1, rightPart)
            else:
                freePlace[placePosition] = rightPart
        # print(f"freeSpace new: {freePlace}")

    def setVariables(self, patternSize, occurenceCount, occurenceRate):
        self.patternSize = patternSize
        self.occurenceCount = occurenceCount
        self.occurenceRate = occurenceRate

        assert 0 <= self.occurenceRate <= 1, \
            f'Ошибка при генерации текста: доля вхождений есть величина от 0 до 1, но не {self.occurenceRate}'
        
        self.textSize = ceil(patternSize * occurenceCount / occurenceRate)

    def getPositions(self):
        assert self.textSize > 0, \
            'Ошибка, длина текста не должна быть нулевой'
        assert self.patternSize > 0, \
            'Ошибка при генерации позиций паттерна: паттерн не должен быть пустым'

        freePlace = [[0, self.textSize - self.patternSize - 1]]
        patternPositions = []

        occurenceCount = self.occurenceCount

        while len(freePlace) != 0 and occurenceCount != 0:
            placePosition = randint(0, len(freePlace) - 1)
            # print(f"Next place position: {placePosition}")
            place = freePlace[placePosition]
            patternPosition = randint(place[0], place[1])
            # print(f"Pattern position: {patternPosition}")

            leftPart, rightPart = self.trySplit(place, patternPosition, self.patternSize)

            self.updateFreePlace(freePlace, placePosition, leftPart, rightPart)

            patternPositions.append(patternPosition)
            occurenceCount -= 1

        patternPositions.sort()
        return patternPositions


    def extendBySpaceSize(self, text, size):
        space = self.genPattern(size)
        # print(f"Space generated: {space}")
        text.extend(space)


    def genText(self, pattern, patternPositions):
        assert len(pattern) > 0, \
            'Ошибка при генерации текста: паттерн не должен быть пустым'

        # print(f"Pattern size: {self.patternSize},\
        #       \noccurence count: {self.occurenceCount},\
        #       \noccurence rate: {self.occurenceRate},\
        #       \ntext size: {self.textSize}")

        text = []

        if len(patternPositions) == 0:
            self.extendBySpaceSize(text, self.textSize)
        else:
            for position in patternPositions:
                self.extendBySpaceSize(text, position - len(text))
                text.extend(pattern)

            self.extendBySpaceSize(text, self.textSize - patternPositions[-1])

        return text

