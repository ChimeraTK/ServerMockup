/*
 * XMLParser.h
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#ifndef INCLUDE_XMLPARSER_H_
#define INCLUDE_XMLPARSER_H_

#include <libxml++/libxml++.h>
#include "ChimeraTK/SupportedUserTypes.h"
#include "ChimeraTK/ApplicationCore/ApplicationCore.h"

namespace ctk = ChimeraTK;

namespace xml_parser{

/**
 * Possible directions of data flow
 */
enum direction{ IN, OUT };

/**
 * The struct holds all information for individual variables available from the xml file
 * produced by the xml-generator of chimeraTK servers.
 */
struct variable{
  ctk::DataType type;
  direction dir;
  std::string description;
  std::string unit;
  std::string name;
  std::string directory;
  size_t      nElements;
  void print();
};

/**
 * Open the xml file and retrive the root directory. This might be used to
 * define the server name.
 * The expected structure in the xml is:
 * \code{.xml}
 * <application xmlns="https://github.com/ChimeraTK/ApplicationCore" name="WatchdogServer">
 * ....
 * </application>
 * \endcode
 * In this example the returned name is WatchdogServer.
 * \param inputFile Input xml file produced with the srervers xml generator
 * \return The application name.
 * \throw xmlpp::exception In case no application element is found.
 */
std::string getRootDir(std::string inputFile);

/**
 * Analyse a variable node from the xml file and store data in a variable struct.
 * Data is added/taken from subnodes:
 * - description
 * - unit
 * - value_type
 * - direction
 * - numberOfElements
 *
 * \throws xmlpp::exception In case the data flow direction is unknown.
 */
variable analyseNode(const xmlpp::Element &element);

template<typename UserType>
using OutputList = std::list<ctk::ScalarOutput<UserType>>;
template<typename UserType>
using OutputArrayList = std::list<ctk::ArrayOutput<UserType>>;
template<typename UserType>
using InputList = std::list<ctk::ScalarPollInput<UserType>>;
template<typename UserType>
using InputArrayList = std::list<ctk::ArrayPollInput<UserType>>;

/**
 * Create a ChimeraTK Accessor and put it in the according list.
 *
 */
void addElement(const xmlpp::Element &element, ctk::Module* owner,
    ctk::TemplateUserTypeMap<OutputList>* l1,
    ctk::TemplateUserTypeMap<OutputArrayList>* l2,
    ctk::TemplateUserTypeMap<InputList>* l3,
    ctk::TemplateUserTypeMap<InputArrayList>* l4);
}
#endif /* INCLUDE_XMLPARSER_H_ */
