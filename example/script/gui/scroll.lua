local function scroll()
  local object = {
    drawer = {},
    max_drawer = {},
    positions = {},
    current_pos = 1,
    add = function(self, position)
      self.positions[self.current_pos] = position
      self.current_pos = self.current_pos + 1
    end,
    prev_mpos = {},
    t1 = {},
    t2 = {},
    pos = nil,
    is_drag = false,
    show = function(self, pos, scale)
      self.drawer = drawui(self.t2)
      self.drawer.scale = vector2(scale.x, scale.y * 0.1)
      if self.pos == nil then
        self.pos = vector2(pos.x, pos.y)
      end
      self.drawer.position = self.pos
      self.max_drawer = drawui(self.t1)
      self.max_drawer.scale = scale
      self.max_drawer.position = pos
      -- Mouse
      local mpos = mouse.position()
      mpos.x = mpos.x - window.center().x
      mpos.y = -(mpos.y - window.center().y)
      if not self.is_drag and mouse.is_pressed(mouse.LEFT) then
        local dpos = self.drawer.position
        local dscale = self.drawer.scale
        if mpos.x >= dpos.x - dscale.x / 2
            and
            mpos.x <= dpos.x + dscale.x / 2
            and
            mpos.y >= dpos.y - dscale.y / 2
            and
            mpos.y <= dpos.y + dscale.y / 2
        then
          self.is_drag = true
        end
      end
      if (mouse.is_released(mouse.LEFT)) then self.is_drag = false end
      if self.is_drag then
        self.pos.y = mpos.y
        if self.pos.y < self.max_drawer.position.y - self.max_drawer.scale.y / 2 + self.drawer.scale.y / 2 then
          self.pos.y = self.max_drawer.position.y - self.max_drawer.scale.y / 2 + self.drawer.scale.y / 2
        end
        if self.pos.y > self.max_drawer.position.y + self.max_drawer.scale.y / 2 - self.drawer.scale.y / 2 then
          self.pos.y = self.max_drawer.position.y + self.max_drawer.scale.y / 2 - self.drawer.scale.y / 2
        end
        for i, v in ipairs(self.positions) do
          v.y = mpos.y + v.y
        end
      end
      self.prev_mpos = vector2(mpos.x, mpos.y)
      self.positions = {}
      self.current_pos = 1
      GUI_MANAGER:add(self.drawer)
      GUI_MANAGER:add(self.max_drawer)
    end,
  }
  object.drawer = drawui(texture())
  object.t1 = texture()
  object.t1:fill_color(color(0, 0, 0, 0.5))
  object.t2 = texture()
  object.t2:fill_color(color(1, 1, 1, 0.5))

  return object
end

return scroll
