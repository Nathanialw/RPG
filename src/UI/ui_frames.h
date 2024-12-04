#pragma once

#include "Objects/Buildings/building.h"
#include "graphics.h"
#include "menu.h"
#include "unit_frames.h"
#include <string>
#include <vector>


namespace UI_Frames {

    class Grid {
    private:
        int x = 0;
        int y = 0;
        int w = 0;

    public:
        void Update(int w) {
            x++;
            if ((x > 0) && x % w == 0) {
                y++;
                x = 0;
            }
        }

        [[nodiscard]] float Get_X() const {
            return (float) x;
        };

        [[nodiscard]] float Get_Y() const {
            return (float) y;
        };
    };

    enum Menu_Tab {
        build,
        train,
        army,
        serfs,
        SIZE
    };

    int Build(entt::registry &zone, int &state, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y, std::string objectName) {
        Build::Orc::House(zone, state, objectName);
        return 0;
    }

    struct Building {
        Component::Icon icon = {Graphics::default_icon};
        std::string name;
        Spells::castSpell build{};
    };

    struct Menu_Button {
        UI::Text_Frame button;
        std::vector<Building> objects;
    };

    struct Menu_Frame {
        UI::Image_Frame background;
        UI::Image_Frame submenu;
        std::array<Menu_Button, Menu_Tab::SIZE> buttons;
        Menu_Tab currentTab = SIZE;
        bool open = false;
    };

    std::vector<std::string> tabs = {"Build", "Train", "Army", "Serfs"};
    Menu_Frame topFrame;

    const int numHouses = 63;

    void Load_Buildings() {
        std::array<std::string, numHouses> houses;

        houses = {"Medieval RTP Building Exteriors Building1_1"};

        Building building;
        for (const auto &house: houses) {
            building.name = house;
            building.icon.clipSprite = Icons::buildingIconClipRects[building.name].clipRect;
            building.icon.pTexture = Graphics::medievelBuildingIcons;
            building.build = Build;
            topFrame.buttons[Menu_Tab::build].objects.emplace_back(building);
        }
    }

    void Update_Frame_Data(f2 &scale, std::string &text, UI::Text_Frame &frame) {
        if (frame.textTexture == nullptr) {
            Unit_Frames::Populate_Frame(frame, text, {100, 255, 50});
        }
        Unit_Frames::Update_Frame_Text(scale, frame, text, 40.0f);
    }

    //set the background frame in a position with a size and evenly space the buttons within
    void Init_Button_Menu() {
        //x needs to be screen width -frame width
        //needs to update on screen resize
        topFrame.background.frame = {0.0f, 0.0f, 512.0f, 64.0f};
        topFrame.submenu.frame = {topFrame.background.frame.x, topFrame.background.frame.y + topFrame.background.frame.h, topFrame.background.frame.w, ((topFrame.background.frame.h * 2.0f) * (ceil(numHouses / 4.0f)))};
    }

    void Build_Buttons(Component::Camera &camera) {
        topFrame.background.frame = UI::Update_Scale(camera.scale, topFrame.background.frame);
        topFrame.submenu.frame = UI::Update_Scale(camera.scale, topFrame.submenu.frame);
        topFrame.background.frame.x = camera.screen.w - topFrame.background.frame.w;
        topFrame.submenu.frame.x = camera.screen.w - topFrame.submenu.frame.w;

        float xPos = topFrame.background.frame.x;
        for (int i = 0; i < tabs.size(); i++) {
            topFrame.buttons[i].button.backgroundTexture = Graphics::tooltipBackground;
            topFrame.buttons[i].button.frame = {
                    xPos,
                    topFrame.background.frame.y,
                    topFrame.background.frame.w / topFrame.buttons.size(),
                    topFrame.background.frame.h,
            };
            Update_Frame_Data(camera.scale, tabs[i], topFrame.buttons[i].button);
            xPos += topFrame.background.frame.w / topFrame.buttons.size();
        }
    }

    void Show_Submenu(Menu_Frame &menu) {
        SDL_RenderCopyF(Graphics::renderer, menu.buttons[0].button.backgroundTexture, nullptr, &menu.submenu.frame);
        Grid grid;
        for (const auto &building: menu.buttons[menu.currentTab].objects) {
            SDL_FRect rect = {menu.submenu.frame.x + (grid.Get_X() * menu.background.frame.h * 2.0f), menu.submenu.frame.y + (grid.Get_Y() * menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f), (menu.background.frame.h * 2.0f)};
            rect.x += (rect.w * 0.1f);
            rect.y += (rect.h * 0.1f);
            rect.w -= (rect.w * 0.1f);
            rect.w -= (rect.w * 0.1f);
            SDL_RenderCopyF(Graphics::renderer, building.icon.pTexture, &building.icon.clipSprite, &rect);
            grid.Update(4);
        }
    }

    void Show_Menu_Frame(entt::registry &zone, Component::Camera &camera) {
        Init_Button_Menu();
        Build_Buttons(camera);

        for (auto &menu: topFrame.buttons) {
            Update_Frame_Data(camera.scale, menu.button.text, menu.button);
            SDL_RenderCopyF(Graphics::renderer, menu.button.backgroundTexture, nullptr, &menu.button.frame);
            SDL_RenderCopyF(Graphics::renderer, menu.button.textTexture, nullptr, &menu.button.textFrame);
        }

        //render submenu
        switch (topFrame.currentTab) {
            case build: {
                Show_Submenu(topFrame);
                break;
            }
            case train: {
                Show_Submenu(topFrame);
                break;
            }
            case army: {
                Show_Submenu(topFrame);
                break;
            }
            case serfs: {
                Show_Submenu(topFrame);
                break;
            }
            case SIZE: {
                break;
            }
        }
    }
}// namespace UI_Frames