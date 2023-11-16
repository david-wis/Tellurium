#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>

#define SEQUENCE_TYPE_KEYDOWN "keyDown"
#define SEQUENCE_TYPE_KEYUP "keyUp"
#define SEQUENCE_TYPE_PRESS "press"

extern FILE * outputFile;

void SequenceGenerate(SequenceNode * sequence) {
    LogDebug("\tSequenceGenerate\n");
    fputs("newSequence([", outputFile);
    ActionListGenerate(sequence->actionList);
    fputs("])", outputFile);
}

void ActionListGenerate(ActionListNode * actionList) {
    LogDebug("\tActionListGenerate\n");
    if (actionList->action != NULL) {
        if (actionList->next->action != NULL) {
            ActionListGenerate(actionList->next);
            fputs(", ", outputFile);
        }
        ActionGenerate(actionList->action);
    }
}

static char * keyNames[] = { "CONTROL", "SHIFT", "ALT", "LEFT", "RIGHT", "UP", "DOWN", "ENTER", "SPACE", "BACK_SPACE", "DELETE", "TAB", "ESCAPE", "PAGE_UP", "PAGE_DOWN", "END", "HOME" };
static char * keyStates[] = { SEQUENCE_TYPE_KEYDOWN, SEQUENCE_TYPE_KEYUP, SEQUENCE_TYPE_PRESS };

void ActionGenerate(ActionNode * action) {
    LogDebug("\tActionGenerate\n");
    if (action->type == ACTION_STREAM) {
        fprintf(outputFile, "{ value : %s, type: \"%s\" }", action->action.stream, SEQUENCE_TYPE_PRESS);
        return;
    }    
    LogDebug("\tKeyActionGenerate %d\n", action->action.key->key_value);
    Key * key = action->action.key;
    fprintf(outputFile, "{ value : Selenium.Key.%s, type: \"%s\" }", keyNames[key->key_value], keyStates[key->state]);
}
