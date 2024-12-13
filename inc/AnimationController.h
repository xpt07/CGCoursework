#include <string>
#include <unordered_map>
#include <functional>

// The AnimationController defines a simple state machine for controlling animations.
// This class manages animation states and transitions, allowing for dynamic changes based on game logic.
class AnimationController {
private:
    std::unordered_map<std::string, std::function<void()>> states;  // Map of animation states to their actions.
    std::string currentState;   // Tracks the currently active state.
public:
    // Adds a new animation state with a corresponding on-enter action.
    void addState(const std::string& state, std::function<void()> onEnter) {
        states[state] = onEnter;    // Map the state name to its callback function.
    }

    // Transitions to a new animation state if it's different from the current state.
    // Executes the on-enter action for the new state.
    void transitionTo(const std::string& newState) {
        if (currentState != newState && states.find(newState) != states.end()) {
            currentState = newState;
            states[newState]();
        }
    }

    // Retrieves the name of the current animation state.
    const std::string& getCurrentState() const {
        return currentState;
    }
};
