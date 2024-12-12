#include <string>
#include <unordered_map>
#include <functional>

class AnimationController {
private:
    std::unordered_map<std::string, std::function<void()>> states;
    std::string currentState;
public:
    void addState(const std::string& state, std::function<void()> onEnter) {
        states[state] = onEnter;
    }

    void transitionTo(const std::string& newState) {
        if (currentState != newState && states.find(newState) != states.end()) {
            currentState = newState;
            states[newState]();
        }
    }

    const std::string& getCurrentState() const {
        return currentState;
    }
};
