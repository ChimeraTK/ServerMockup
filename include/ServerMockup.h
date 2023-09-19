// SPDX-FileCopyrightText: Helmholtz-Zentrum Dresden-Rossendorf, FWKE, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once
/*
 * DummyServer.h
 *
 *  Created on: Nov 16, 2017
 *      Author: Klaus Zenker (HZDR)
 */

#include "ChimeraTK/ApplicationCore/ApplicationCore.h"
#include "ChimeraTK/ApplicationCore/PeriodicTrigger.h"

constexpr auto INPUT = "server-mockup_input.xml";

#include "XMLParser.h"

#include <vector>

namespace ctk = ChimeraTK;

/**
 * This group is used to build the hierarchy read from the xml file.
 * The hierarchy is build recursive by adding TemplateGroups to groups and so on.
 */
struct TemplateGroup : ctk::VariableGroup {
  using ctk::VariableGroup::VariableGroup;

  std::vector<TemplateGroup> groups;

  ctk::TemplateUserTypeMapNoVoid<xml_parser::OutputList> _outputAccessorListMap;
  ctk::TemplateUserTypeMapNoVoid<xml_parser::InputList> _inputAccessorListMap;
  ctk::TemplateUserTypeMapNoVoid<xml_parser::OutputArrayList> _outputArrayAccessorListMap;
  ctk::TemplateUserTypeMapNoVoid<xml_parser::InputArrayList> _inputArrayAccessorListMap;

  void addElement(const xmlpp::Element* element);
};

/**
 * This module holds all control system variables read from the input xml file.
 *
 * \remark
 * In principle no TimerModule is needed here. One could simply use a trigger variable
 * in the control system, which will never be changed. Thus the main loop of the XMLModule
 * would be blocked forever.
 *
 * \remark Can not inherit from TemplateGroup since both inherit from ctk::Module which is not
 * virtual inherited. Else one could get rid of defining addElement here and in TemplateGroup.
 */
struct TemplateModule : public ctk::ApplicationModule {
  using ctk::ApplicationModule::ApplicationModule;
  ctk::ScalarPushInput<uint64_t> trigger{this, "/DummyTrigger/tick", "",
      "Dummy trigger without functionality. It is just just used to keep the server alive."};

  std::vector<TemplateGroup> groups;

  ctk::TemplateUserTypeMapNoVoid<xml_parser::OutputList> _outputAccessorListMap;
  ctk::TemplateUserTypeMapNoVoid<xml_parser::InputList> _inputAccessorListMap;
  ctk::TemplateUserTypeMapNoVoid<xml_parser::OutputArrayList> _outputArrayAccessorListMap;
  ctk::TemplateUserTypeMapNoVoid<xml_parser::InputArrayList> _inputArrayAccessorListMap;

  void addElement(const xmlpp::Element* element);

  /**
   * Application core main loop.
   */
  void mainLoop() {
    while(true) {
      trigger.read();
    }
  }
};

struct ServerMockup : public ctk::Application {
  ServerMockup();
  ~ServerMockup() { shutdown(); }

  std::vector<TemplateModule> modules;

  void initialise() override;
};
