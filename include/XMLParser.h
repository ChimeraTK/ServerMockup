/*
 * XMLParser.h
 *
 *  Created on: Nov 16, 2017
 *      Author: zenker
 */

#ifndef INCLUDE_XMLPARSER_H_
#define INCLUDE_XMLPARSER_H_

#include <libxml++/libxml++.h>

namespace xml_parser{

/**
 * Supported data types
 * Extend this in case other data types are used in you input xml file.
 */
enum varType { Int32, UInt32, Double, Float, String, Unknown};

/**
 * The struct holds all information stored in OPCUA for individual variables.
 */
struct variable{
  varType type;
  std::string desciption;
  std::string unit;
  std::string name;
  std::string directory;
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

class XMLParser{
private:
  std::string inputFile_;
  xmlpp::DomParser parser_;

  /**
   * Read all subelements from the given element. If subelements are directories (identified by its names: "directory")
   *  too readDirectory is called iteratively. This is done until variable elements are found (identified by
   *  its names: "variable").
   * \param element An element to start with. All elements inside the given element
   * are parsed and variables are put into the vars vector by calling readVariable.
   */
  void readDirectory(const xmlpp::Element *element, std::string directory);
  /**
   * Read variable information from the given element. Empty elements are ignored as well as
   * additional elements. Test elements are:
   * - description
   * - unit
   * - value_type
   * \param element The elements name is expected to be "variable" and not "directory"
   * \param directoy The directory in the global tree structure of the given element.
   * This information is stored in the variable.
   */
  void readVariable(const xmlpp::Element* element, std::string directory);

public:
  XMLParser(std::string inputFile){inputFile_ = inputFile;}
  /**
   * Vector of all variables read from the input xml file.
   * Only supported data types are read from the file (see varType).
   */
  std::vector<variable> vars;
  /**
   * Parse the input file and fill the vector of variables (see vars)
   */
  void parse();

};

}

#endif /* INCLUDE_XMLPARSER_H_ */
