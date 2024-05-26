import random
import string
import os

import time

INT_MAX = 2^64-1
LINE_COUNT = 200000
WORD_COUNT = 50
WORD_LENGTH = 7

TEST_FILENAME = 'test.txt'
TREE_FILE_PATH = 'rbtree'

RESULT_FILENAME = 'res.log'


class TestDataConfig:
    def __init__(self, line_count, word_count, word_length, filename, tree_file_path):
        self.line_count = line_count
        self.word_count = word_count
        self.word_length = word_length

        self.filename = filename
        self.tree_file_path = tree_file_path

def prepareTestData(config: TestDataConfig):
    words_list = []

    for _ in range(config.word_count):
        word = ''.join(random.choices(string.ascii_lowercase, k=random.randint(1, config.word_length)))
        words_list.append(word)

    output_file = open(config.filename, 'w')

    saved = False

    for _ in range(LINE_COUNT):
        random_word = words_list[random.randint(0, config.word_count - 1)]
        command = random.randint(0, 70)
        if 0 <= command < 40: # + word number
            s = f'+ {random_word} {random.randint(0, INT_MAX)}\n'
            output_file.write(s)
        if 40 <= command < 50: # - word
            s = f'- {random_word}\n'
            output_file.write(s)
        if 50 <= command < 70: # word
            s = f'{random_word}\n'
            output_file.write(s)
        # if 80 <= command < 90: # save
        #     output_file.write(f'! Save {config.tree_file_path}\n')
        #     saved = True
        # if 90 <= command < 100 and saved: # load
        #     output_file.write(f'! Load {config.tree_file_path}\n')

    output_file.close()

def runTests():
    os.system('rm ' + TREE_FILE_PATH)

    config = TestDataConfig(LINE_COUNT, WORD_COUNT, WORD_LENGTH, TEST_FILENAME, TREE_FILE_PATH)
    prepareTestData(config)

    time_start = time.time_ns()
    os.system('./exec < ' + TEST_FILENAME + ' > ' + RESULT_FILENAME)
    time_end = time.time_ns()
    fdns = (time_end - time_start)
    duration_s = fdns // 10**9
    duration_ms = (fdns - duration_s*10**9) // 10**6
    print(f"time: {duration_s}s, {duration_ms}ms")

if __name__ == '__main__':
    runTests()
