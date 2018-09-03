# UnrealUtilityAI
Simple Utility AI implementation for Unreal Engine 4

Utility AI is a pretty opaque definition in the gaming world. At the end is very similiar to what you would have called 'fuzzy logic' in the old days.

At every 'brain' iteration, a series of 'actions' is evaluated. Each evalutation will return a value. This value determines which action to execute. This value should define the 'fuzzyness' of your choice. As an example, you may define a level of 'hunger', with 0 being no-hunger at all, 50 as moderately hungry, and 100 totally in hunger. At the same time you may have another level (well an action) that measure the 'rage' of an agent. Again 0 means no rage at all and 30 means berserk.

Why we choose a max of 100 for hunger and 30 for rage ? Well, because being in berserk mode without the energy to fight it would be meaningless. If our UtilityAI chooses the next action to execute based on the highest value, we would know that our agent will never go in berserk mode before having eaten something.

## UUtilityAIAction

This is the C++ class (can be a parent for blueprints too) defining an 'action'.

An action is composed by the following methods/events (all overridable):

* Spawn (called when the action is instantiated, generally one time per brain)
* Tick (called at every world tick if the action is the currently choosen one)
* CanRun (returns a bool, if true, the default, the action will be taken into account for the final computation)
* Score (returns a float, the score of the action)
* Enter (called whenever the brain switches from another action to this one)
* Exit (called whenever the brain switches to another action)

To create UtilityAIAction's with blueprints, just create a new blueprint inheriting from UtilityAIAction and override the related functions/events.

## UUtilityAIComponent

This is the C++ ActorComponent (can be a parent for blueprints too) you will attach to AIController's.

At every Tick this component will compute the score of each valid action (read: an action with CanRun() returning true) and will choose the next action to run. The default algorithm choose the highest value with highest priority (see below). You are free to change the default algorithm using the following flags (you can change them via blueprint too):

* IgnoreZeroScore (do not take into account actions returning a score of zero, same effect of CanRun() returning false)
* UseLowestScore (invert the algorithm by choosing the lowest value)
* InvertPriority (actions are evaluated in the order they have been inserted in the set. The first inserted action will have the highest priority. By inverting priority, in the case of equivalent values, the last inserted action will win)
* RandomizeOnEquality (instead of using prioriry, simply randomize the choice for equivalent scores)
* EqualityTolerance (as the Score() function returns floats, you may want to add a threshold value when comparing scores for equality)

## Functional Tests

In the Content folder is included a level (UtilityAI_Tests) for functional tests. Just click on 'Refresh Tests' in the Automation tool to discover them (if not already discovered) and run them.
