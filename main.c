/*
Copyright (c) 2014 Peter Harrison

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "commands.h"
#include "testdata.h"
#include "makepath.h"

/*
 * Display the expected and generated command lists side by side in numeric form.
 * Displaying mnemonics would not be appropriate since the output may have no
 * corresponding mnemonic if there is an error.
 * One command is shown per line and any line that does not match
 * is highlighted.
 */
static void listComparison(COMMAND *pExpected, COMMAND *pActual, size_t n) {
  COMMAND c1;
  COMMAND c2;
  while (--n > 0) {
    c1 = *pExpected++;
    c2 = *pActual++;
    printf(" %s expected: %3d  actual: %3d\n", (c1 != c2) ? ">" : " ", c1, c2);
    if (c1 == CMD_STOP) {
      break;
    }
  }
}

/*
 * for each test pair in the test data list, use the input data to generate a
 * set of commands for the output path.
 * Each generated command list is then compared with the expected command
 * list.
 * the status of each comparison is displayed.
 * If they fail to match an additional list of paired expected and generated
 * values is shown.
 * The test does not stop when it encounters a failure since the same code error
 * may affect several test.
 */
static int runTestsSimple(void) {
  int errorPos;
  int failCount = 0;
  int test;
  for (test = 0; test < testCountSimple(); test++) {
    makeSimplePath(testPairsSimple[test].input);
    errorPos = compareCommands(testPairsSimple[test].expected, commandList, MAX_CMD_COUNT);
    printf("test %3d : ", test);
    if (errorPos == -1) {
      printf(" OK   %-8s  => ", testPairsSimple[test].input);
      listCommands();
    } else {
      failCount++;
      printf("FAIL  %-8s  => ", testPairsSimple[test].input);
      listCommands();
      listComparison(testPairsSimple[test].expected, commandList, MAX_CMD_COUNT);
    }
  }
  return failCount;
}

int main(int argc, char** argv) {
  int failures;
  failures = runTestsSimple();
  printf("\n\n%d tests complete, %d failed\n", testCountSimple(), failures);
  return (EXIT_SUCCESS);
}

