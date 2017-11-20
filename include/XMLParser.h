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

enum varType { Int32, UInt32, Double, Float, String, Unknown};

struct variable{
  varType type;
  std::string desciption;
  std::string unit;
  std::string name;
  std::string directory;
  void print();
};

std::string getRootDir(std::string inputFile);

class XMLParser{
private:
  std::string inputFile_;
  xmlpp::DomParser parser_;


  void readDirectory(const xmlpp::Element *element, std::string directory);
  void readVariable(const xmlpp::Element* element, std::string directory);

public:
  XMLParser(std::string inputFile){inputFile_ = inputFile;}
  std::vector<variable> vars;
  void parse();

};

}

#endif /* INCLUDE_XMLPARSER_H_ */
