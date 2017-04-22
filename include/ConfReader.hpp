#ifndef CONFREADER_H
#define CONFREADER_H

#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>

// Local includes
#include <Components.hpp>

class ConfReader
{
public:
   ConfReader(){};
   ~ConfReader(){};
   bool readFermentables(std::string fileName,
			 std::vector<fermentable> &fermentables);
   bool readYeasts(std::string fileName,
		   std::vector<yeast> &yeasts);
   bool readRecipe(std::string fileName,
		   std::map<std::string,std::string> &metadata,
		   std::vector<fermentable> &fermentables,
		   std::vector<hop> &hops,
		   std::vector<yeast> &yeasts,
		   std::vector<mash> &mashes,
		   std::string &note,
		   BrewSpecifications &observations
		   );
   bool readBrewery(std::string fileName, brewery &b);
private:
   std::string m_recipe;
   std::vector<std::string> splitConfString(std::string s);
};

#endif
