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
   std::vector<std::string> splitConfString(std::string s);
   void readFermentables(std::string fileName,
			 std::vector<fermentable> &fermentables);
   void readRecipe(std::string fileName,
		   std::map<std::string,std::string> &metadata,
		   std::vector<fermentable> &fermentables,
		   std::vector<hop> &hops,
		   std::vector<yeast> &yeasts,
		   std::vector<mash> &mashes,
		   std::string &note
		   );
private:
   std::string m_recipe;
};

#endif
