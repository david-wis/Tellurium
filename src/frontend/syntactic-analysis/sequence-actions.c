#include "bison-actions.h"

SequenceNode * SequenceGrammarAction(ActionListNode * actionList) {
    SequenceNode * sequence = calloc(1, sizeof(*sequence));
    sequence->actionList = actionList;
    return sequence;
}

ActionListNode * ActionListGrammarAction(ActionListNode * next, ActionNode * action) {
    ActionListNode * actionList = calloc(1, sizeof(* actionList));
    actionList->next = next;
    actionList->action = action;
    return actionList;
}

ActionNode * KeyActionGrammarAction(action_key_t key, ActionKeyState state) {
    Key * keyAction = calloc(1, sizeof(*keyAction));
    keyAction->key_value = key;
    keyAction->state = state;
    ActionNode * action = calloc(1, sizeof(*action));
    action->type = ACTION_KEY;
    action->action = (ActionUnion) {.key = keyAction};
    return action;
}

ActionNode * StreamActionGrammarAction(char* stream) {
    ActionNode * action = calloc(1, sizeof(*action));
    action->type = ACTION_STREAM;
    action->action = (ActionUnion) {.stream = stream} ;
    return action;
}
