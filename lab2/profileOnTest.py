from genAndRunTests import *

runTests()

os.system('cat test.txt | gprof ./exec > profile')
os.system('cat profile | gprof2dot | dot -Tsvg -o output.svg')