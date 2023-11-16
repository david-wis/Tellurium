#include "bison-actions.h"

SequenceNode * SequenceGrammarAction(ActionListNode * actionList) {
    SequenceNode * sequence = gcCalloc(sizeof(*sequence));
    sequence->actionList = actionList;
    return sequence;
}

ActionListNode * ActionListGrammarAction(ActionListNode * next, ActionNode * action) {
    ActionListNode * actionList = gcCalloc(sizeof(* actionList));
    actionList->next = next;
    actionList->action = action;
    return actionList;
}

ActionNode * KeyActionGrammarAction(action_key_t key, ActionKeyState state) {
    Key * keyAction = gcCalloc(sizeof(*keyAction));
    keyAction->key_value = key;
    keyAction->state = state;
    ActionNode * action = gcCalloc(sizeof(*action));
    action->type = ACTION_KEY;
    action->action = (ActionUnion) {.key = keyAction};
    return action;
}

ActionNode * StreamActionGrammarAction(char* stream) {
    ActionNode * action = gcCalloc(sizeof(*action));
    action->type = ACTION_STREAM;
    action->action = (ActionUnion) {.stream = stream} ;
    return action;
}
