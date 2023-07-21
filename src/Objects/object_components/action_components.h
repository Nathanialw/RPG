#pragma once
#include <stdint.h>

namespace Action_Component {

    enum Action_State {
        isStatic,
        idle,
        idle2,
        idle6,
        walk,
        walkBow,
        run,
        attack,
        attack2,
        attack2H_1,
        attack2H_2,
        idle2H,
        idleCritical1,
        idleCritical2,
        talk,
        run2H,
        walk2H,
        casting,
        cast,
        struck,
        block,
        evade,
        stunned,
        idle_low_hp,
        idle_low_hp2,
        dying,
        dead,
        dead2,
        dead3,
        dead4,
        dead5,
        dead6,
        resting,
        ranged,
        ranged_bow1,
        idleBow,
        cheer,
        behavior,
        summoned,
        kneel,
        pray_kneeled,
        pray_standing,
        in_combat,
        in_combat2,
        climb,
        use_item,
        crouch
    };
  
    enum Action_Progress {
        start,
        mid,
        last
    };

    struct Action {
        Action_State state = walk;
        Action_Progress frameState = start;
        uint8_t frame = 0;
        u_int16_t frameTime = 0;
    };

    void Set_State(Action &action, Action_State state) {
        if (action.state == state) {
            return;
        }
        action.state = state;
        action.frameState = start;
        action.frameTime = 0;
        action.frame = 0;
    }

}
