
namespace UI_Info {

  struct Unit_Info {
    std::vector<std::string> traits = {"fair", "hard"};
    std::vector<std::string> description = {"this is flavour text", "this is more flavour text"};
    bool b_isOpen = false;
    SDL_FRect panelRect;
    SDL_Rect backgroundFrame = {0, 0, 1137, 700};
    SDL_Texture *background = NULL;
    SDL_Rect leftPageFrame = {0, 0, 1137, 700};
    SDL_Texture *leftPage = NULL;
    SDL_Rect rightPageFrame = {0, 0, 1137, 700};
    SDL_Texture *rightPage = NULL;
    int currentTab = 0;
    entt::entity spell;
  };
  Unit_Info spellbook;

  void Info_Toggle() {
    if (spellbook.b_isOpen) {
      spellbook.b_isOpen = false;
    } else {
      spellbook.b_isOpen = true;
    }
  }

  void Update_Position() {
    spellbook.panelRect = UI::Center_Rect(spellbook.backgroundFrame);
    spellbook.background = Graphics::spellbook;
  }

  void Draw_Spell_Page(Component::Camera &camera, SDL_FRect &draw, Spell_Data::Skill_Tree tree) {
    float xSpacing = 32.0f;
    float ySpacing = 32.0f;
    float x = draw.x;
    float y = draw.y;
    float i = 1;
    float w = 0;
    SDL_FRect renderRect = {x + 192.0f, y + 128.0f, 0.0f, 0.0f};
    SDL_FRect statBox = Camera_Control::Convert_FRect_To_Scale(renderRect, camera);

    for (int j = 0; j < spellbook.traits.size(); ++j) {
      FC_Scale scale = {1.0f / camera.scale.x, 1.0f / camera.scale.y};
      SDL_Rect textRect = FC_DrawScaleRight(Graphics::fcFont, Graphics::renderer, statBox.x, statBox.y, scale, spellbook.traits[j].c_str());
      statBox.y += 20.0f / camera.scale.y;

      SDL_FRect textRectF = Utilities::SDL_Rect_To_SDL_FRect(textRect);
      if (Mouse::bRect_inside_Cursor(textRectF)) {
        SDL_FPoint mousePoint = {Mouse::iXMouse, Mouse::iYMouse};
        Tooltip::Tooltip_Text_Data tooltip = {spellbook.description[j], mousePoint};
        tooltip.charWidth = (10.0f / camera.scale.x);
        tooltip.charHeight = (20.0f / camera.scale.y);
        float xoffset = (mousePoint.x + (50.0f / camera.scale.x));

        SDL_FRect tooltipBackground = {xoffset, mousePoint.y + tooltip.charHeight, (tooltip.text.size() * tooltip.charWidth), tooltip.charHeight};
        Tooltip::Render_Tooltip_Background(tooltipBackground, camera);
        SDL_Color color = {255, 255, 255, 255};
        FC_DrawScale_Center(Graphics::fcFont, Graphics::renderer, xoffset + ((tooltip.text.size() * tooltip.charWidth) / 2.0f), mousePoint.y, scale, color, spellbook.description[j].c_str());
      }
    }
  }

  void Draw_Pages(Component::Camera &camera, SDL_FRect &draw) {
    Draw_Spell_Page(camera, draw, Spell_Data::Skill_Tree(spellbook.currentTab));
  }

  void Draw_Attributes(Component::Camera &camera) {
    if (spellbook.b_isOpen) {
      SDL_FRect draw = UI::Update_Scale(camera.scale, spellbook.panelRect);
      SDL_RenderCopyF(Graphics::renderer, spellbook.background, &spellbook.backgroundFrame, &draw);
      Draw_Pages(camera, spellbook.panelRect);
    }
  }
}// namespace Info