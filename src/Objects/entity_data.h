#pragma once
#include "string"
#include "map"
#include "iostream"


namespace Entity_Data {
  int template_ID = 0;
  std::map<std::string, int> savedTemplate_IDs;

  int Check_For_Template_ID(std::string &name) {
    if (savedTemplate_IDs.count(name) == 0) {
      savedTemplate_IDs[name] = template_ID;
      int unit_ID = template_ID;
      template_ID++;
      //  std::cout << "created and assigned template key: " << unit_ID << std::endl;
      return unit_ID;
    } else {
      // std::cout << "assigned existing template key: " << savedTemplate_IDs[name] << std::endl;
      return savedTemplate_IDs[name];
    }
    std::cout << "failed to assign template key for: " << name << std::endl;
  }

  int Get_Existing_Template_ID(std::string &name, std::string &entity_class) {
    if (savedTemplate_IDs.count(name) == 0) {
      //if it is not found save the class as the name (the class will always have the name of the base unit of the class and thus will default to the base unit if the random unit is not found)
      name = entity_class;
      if (savedTemplate_IDs.count(name) == 0) {
        savedTemplate_IDs[name] = template_ID;
        int unit_ID = template_ID;
        template_ID++;
        //  std::cout << "created and assigned template key: " << unit_ID << std::endl;
        return unit_ID;
      }
      return savedTemplate_IDs[name];
    } else {
      // std::cout << "assigned existing template key: " << savedTemplate_IDs[name] << std::endl;
      return savedTemplate_IDs[name];
    }
    std::cout << "failed to assign template key for: " << name << std::endl;
  }
}