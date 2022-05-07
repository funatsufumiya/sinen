#ifndef NEN_INPUT_SYSTEM_HPP
#define NEN_INPUT_SYSTEM_HPP
#include "../event/process_event.hpp"
#include "../math/vector2.hpp"
#include "game_controller.hpp"
#include "game_controller_button.hpp"
#include "key_code.hpp"
#include "mouse_code.hpp"
#include <array>
#include <cstdint>

namespace nen {
// The different button states
enum class button_state { None, Pressed, Released, Held };

// Helper for keyboard input
class keyboard_state {
public:
  // Friend so InputSystem can easily update it
  friend class input_system;
  // Get just the boolean true/false value of key
  bool GetKeyValue(key_code _key) const;
  // Get a state based on current and previous frame
  button_state GetKeyState(key_code _key) const;

private:
  const uint8_t *mCurrState;
  std::array<uint8_t, static_cast<int>(key_code::NUM_KEYCODES)> mPrevState;
};

// Helper for mouse input
class mouse_state {
public:
  mouse_state(class manager &_manager);
  friend class input_system;

  // For mouse position
  void SetPosition(const vector2 &pos) const;
  void HideCursor(bool hide) const;
  const vector2 &GetPosition() const { return mMousePos; }
  const vector2 &GetScrollWheel() const { return mScrollWheel; }
  bool IsRelative() const { return mIsRelative; }

  // For buttons
  bool is_button_down(mouse_code _button) const;
  button_state get_button_state(mouse_code _button) const;

private:
  class manager &m_manager;
  // Store current mouse position
  vector2 mMousePos;
  // Motion of scroll wheel
  vector2 mScrollWheel;
  // Store button data
  uint32_t mCurrButtons;
  uint32_t mPrevButtons;
  // Are we in relative mouse mode
  bool mIsRelative;
};

// Helper for controller input
class joystick_state {
public:
  friend class input_system;

  // For buttons
  bool GetButtonValue(joystick_button j_button) const;
  button_state GetButtonState(joystick_button j_button) const;

  const vector2 &GetLeftStick() const { return mLeftStick; }
  const vector2 &GetRightStick() const { return mRightStick; }
  float GetLeftTrigger() const { return mLeftTrigger; }
  float GetRightTrigger() const { return mRightTrigger; }

  bool GetIsConnected() const { return mIsConnected; }

private:
  // Current/previous buttons
  uint8_t mCurrButtons[static_cast<int>(
      joystick_button::NUM_GAMECONTROLLER_BUTTON)];
  uint8_t mPrevButtons[static_cast<int>(
      joystick_button::NUM_GAMECONTROLLER_BUTTON)];
  // Left/right sticks
  vector2 mLeftStick;
  vector2 mRightStick;
  // Left/right trigger
  float mLeftTrigger;
  float mRightTrigger;
  // Is this controller connected?
  bool mIsConnected;
};

// Wrapper that contains current state of input
struct input_state {
  input_state(class manager &_manager) : Mouse(_manager) {}
  keyboard_state Keyboard;
  mouse_state Mouse;
  joystick_state Controller;
};

class input_system {
public:
  input_system(class manager &_manager)
      : m_manager(_manager), mState(_manager) {}
  bool Initialize();
  void Shutdown();

  // Called right before SDL_PollEvents loop
  void PrepareForUpdate();
  // Called after SDL_PollEvents loop
  void Update();
  void ProcessEvent();

  const input_state &GetState() const { return mState; }

  void SetRelativeMouseMode(bool value);

private:
  class manager &m_manager;
  float Filter1D(int input);
  vector2 Filter2D(int inputX, int inputY);
  input_state mState;
  joystick mController;
};
} // namespace nen

#endif