/*
 * DummyServer.h
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#ifndef INCLUDE_SERVERMOCKUP_H_
#define INCLUDE_SERVERMOCKUP_H_

#undef GENERATE_XML
#include "ChimeraTK/ApplicationCore/ApplicationCore.h"
#include "ChimeraTK/ApplicationCore/PeriodicTrigger.h"

#define INPUT "server-mockup_input.xml"

#include <vector>
#include "XMLParser.h"

namespace ctk = ChimeraTK;

/**
 * This group is used to build the hierarchy read from the xml file.
 * The hierarchy is build recursive by adding a TemplateGroups to groups and so on.
 */
struct TemplateGroup: ctk::VariableGroup{
  using ctk::VariableGroup::VariableGroup;

  std::vector<TemplateGroup> groups;

  ctk::TemplateUserTypeMap<xml_parser::OutputList> _outputAccessorListMap;
  ctk::TemplateUserTypeMap<xml_parser::InputList> _inputAccessorListMap;
  ctk::TemplateUserTypeMap<xml_parser::OutputArrayList> _outputArrayAccessorListMap;
  ctk::TemplateUserTypeMap<xml_parser::InputArrayList> _inputArrayAccessorListMap;

  void addElement(const xmlpp::Element* element);

};

/**
 * This module holds all control system variables read from the input xml file.
 * \attention
 * So far only the following types are supported:
 * - string
 * - int32
 * - uint32
 * - double
 * - float
 * If other types are used in the input xml file the module needs to be extended!
 *
 * \remark
 * In principle no TimerModule is needed here. One could simply use a trigger variable
 * in the control system, which will never be changed. Thus the main loop of the XMLModule
 * would be blocked forever.
 *
 * \remark Can not inherit from TemplateGroup since both inherit from ctk::Module which is not
 * virtual inherited. Else one could get rid of defining addElement here and in TemplateGroup.
 */
struct TemplateModule: public ctk::ApplicationModule {
  using ctk::ApplicationModule::ApplicationModule;
  ctk::ScalarPushInput<uint64_t> trigger { this, "trigger", "",
      "Trigger used to update the watchdog" };

  std::vector<TemplateGroup> groups;

  ctk::TemplateUserTypeMap<xml_parser::OutputList> _outputAccessorListMap;
  ctk::TemplateUserTypeMap<xml_parser::InputList> _inputAccessorListMap;
  ctk::TemplateUserTypeMap<xml_parser::OutputArrayList> _outputArrayAccessorListMap;
  ctk::TemplateUserTypeMap<xml_parser::InputArrayList> _inputArrayAccessorListMap;

  void addElement(const xmlpp::Element* element);

  /**
   * Application core main loop.
   */
  void mainLoop(){
    while(true){
      trigger.read();
    }
  }
};

struct ServerMockup: public ctk::Application {

  ServerMockup();
  ~ServerMockup() {
    shutdown();
  }

  std::vector<TemplateModule> modules;

  ctk::PeriodicTrigger trigger{this, "Trigger", "Trigger used for other modules"};

  ctk::ControlSystemModule cs;

  void defineConnections();
};



#endif /* INCLUDE_SERVERMOCKUP_H_ */
