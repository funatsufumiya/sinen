#include "custom_logger.hpp"
#include "editor.hpp"
#include <sinen/sinen.hpp>

int main(int argc, char *argv[]) {
  sinen::scene::change_implements<sinen::editor>();
  sinen::scene::set_run_script(true);
  sinen::custom_logger();
  if (!sinen::initialize(argc, argv)) {
    return -1;
  }
  sinen::run();
  if (!sinen::shutdown()) {
    return -1;
  }
  return 0;
}