#include "SQLite_dialogue.h"

namespace Dialogue {

    std::string Get_Random_Dialogue(std::string &text_type) {
        if (SQLite_Dialogue::dialogue.contains(text_type)) {
            if (SQLite_Dialogue::dialogue[text_type].size() > 0) {
                int index = rand() % SQLite_Dialogue::dialogue[text_type].size();
                return SQLite_Dialogue::dialogue[text_type][index];
            }
            else {
                return "No text string found! | Get_Random_Dialogue()";
            }
        }
        else {
            return "No interaction type index found! | Get_Random_Dialogue()";
        }
    }


}