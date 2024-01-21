#include "corridor.hpp"
#include <algorithm>

namespace dts {
void corridor::connect(sinen::grid<int> &grid, std::vector<room> &rooms,
                       float floor_corridor) {

  for (int i = 0; i < rooms.size() - 1;) {
    // choice room edge
    sinen::point2i left = rooms[i].get_position();
    left.x += rooms[i].get_size().x - 1;
    left.y = sinen::random::get_int_range(left.y,
                                          left.y + rooms[i].get_size().y - 1);
    sinen::point2i right = rooms[i + 1].get_position();
    right.y = sinen::random::get_int_range(
        right.y, right.y + rooms[i + 1].get_size().y - 1);

    // connect
    sinen::point2i current = left;
    while (current.x < right.x) {
      grid[current.y][current.x] = floor_corridor;
      path.push_back(current);
      ++current.x;
    }

    while (current.y != right.y) {
      grid[current.y][current.x] = floor_corridor;
      if (current.y < right.y) {
        path.push_back(current);
        ++current.y;
      } else {
        --current.y;
      }
    }

    ++i;
  }
}
} // namespace dts
