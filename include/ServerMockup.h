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

#include <string>
#include <unordered_set>
#include "XMLParser.h"

namespace ctk = ChimeraTK;

/**
 * This module holds all control system variables read from the input xml file.
 * \attention
 * So far only the following types are supported:
 * - string
 * - int
 * - uint
 * - double
 * - float
 * If other types are used in the input xml file the module needs to be extended!
 *
 * \remark
 * In principle no TimerModule is needed here. One could simply use a trigger variable
 * in the control system, which will never be changed. Thus the main loop of the XMLModule
 * would be blocked forever.
 */
struct XMLModule: public ctk::ApplicationModule {
  /**
   * Constructor
   * The default constructor is not used, since the maps are filled during construction.
   */
  XMLModule(EntityOwner *owner, const std::string &name,
      const std::string &description, bool eliminateHierarchy = false,
      const std::unordered_set<std::string> &tags = { });

  ctk::ScalarPushInput<uint64_t> trigger { this, "trigger", "",
      "Trigger used to update the watchdog" };

  /**
   * The variable is needed to make correct connections. Name and directory information
   * is used to reconstruct the variable tree. Don't use a map, which avoids to define the
   * comparison operator of variables.
   * \remark
   * At the time of defining the connections (defineConnections()) the name of the
   * ScalarOutput is not yet available!
   */
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<std::string> > >    out_stringParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<int> > >            out_intParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<uint> > >           out_uintParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<double> > >         out_doubleParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarOutput<float> > >          out_floatParameter;

  std::vector<std::pair<xml_parser::variable, ctk::ScalarPollInput<std::string> > > in_stringParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarPollInput<int> > >         in_intParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarPollInput<uint> > >        in_uintParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarPollInput<double> > >      in_doubleParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ScalarPollInput<float> > >       in_floatParameter;

  std::vector<std::pair<xml_parser::variable, ctk::ArrayOutput<std::string> > >     aout_stringParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayOutput<int> > >             aout_intParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayOutput<uint> > >            aout_uintParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayOutput<double> > >          aout_doubleParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayOutput<float> > >           aout_floatParameter;

  std::vector<std::pair<xml_parser::variable, ctk::ArrayPollInput<std::string> > >  ain_stringParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayPollInput<int> > >          ain_intParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayPollInput<uint> > >         ain_uintParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayPollInput<double> > >       ain_doubleParameter;
  std::vector<std::pair<xml_parser::variable, ctk::ArrayPollInput<float> > >        ain_floatParameter;

  /**
   * Application core main loop.
   */
  void mainLoop();
};


struct ServerMockup: public ctk::Application {

  ServerMockup() : Application(xml_parser::getRootDir(std::string(INPUT))) {}
  ~ServerMockup() {
    shutdown();
  }

  XMLModule xml { this, "xmlModule",
    "Module publishing all variables from the input xml file" };

  ctk::PeriodicTrigger trigger{this, "Trigger", "Trigger used for other modules"};

  ctk::ControlSystemModule cs;

  void defineConnections();
};



#endif /* INCLUDE_SERVERMOCKUP_H_ */
