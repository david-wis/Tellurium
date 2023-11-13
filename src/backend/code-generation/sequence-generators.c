#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>
extern FILE * outputFile;

void SequenceGenerate(SequenceNode * sequence) {
    LogDebug("\tSequenceGenerate\n");
}
void ActionListGenerate(ActionListNode * actionList) {
    LogDebug("\tActionListGenerate\n");
}
void KeyActionGenerate(ActionNode * action) {
    LogDebug("\tKeyActionGenerate\n");
}
void StreamActionGenerate(ActionNode * action) {
    LogDebug("\tStreamActionGenerate\n");
}