/*
 * DummyServer.h
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#ifndef INCLUDE_DUMMYSERVER_H_
#define INCLUDE_DUMMYSERVER_H_

#undef GENERATE_XML
#include <ChimeraTK/ApplicationCore/ApplicationCore.h>

#define INPUT "dummy_server_input.xml"

#include <string>
#include <unordered_set>
#include "XMLParser.h"

namespace ctk = ChimeraTK;

/**
 * This module is used to trigger the mainloops of the modules to go on.
 */
struct TimerModule: public ctk::ApplicationModule {
  using ctk::ApplicationModule::ApplicationModule;
  /**
   * \remark
   * Observe this vaiable by other modules to obtain a trigger
   */
  ctk::ScalarOutput<int> trigger { this, "trigger", "", "Trigger counter",
    { "Timer" }};

  /**
   * Application core main loop.
   */
  void mainLoop();
};

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
 */
struct XMLModule: public ctk::ApplicationModule {
  /**
   * Constructor
   * The default constructor is not used, since the maps are filled during construction.
   */
  XMLModule(EntityOwner *owner, const std::string &name,
      const std::string &description, bool eliminateHierarchy = false,
      const std::unordered_set<std::string> &tags = { });

  ctk::ScalarPushInput<int> trigger { this, "trigger", "",
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


struct DummyServer: public ctk::Application {

  DummyServer() : Application(xml_parser::getRootDir(std::string(INPUT))) {}
  ~DummyServer() {
    shutdown();
  }

  XMLModule xml { this, "xmlModule",
    "Module publishing all variables from the input xml file" };

  TimerModule timer { this, "timer",
      "Module used to trigger the watchdog update" };

  ctk::ControlSystemModule cs;

  void defineConnections();
};



#endif /* INCLUDE_DUMMYSERVER_H_ */
