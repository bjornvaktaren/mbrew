#include "ConfReader.hpp"

std::vector<std::string> ConfReader::splitConfString(std::string s) 
{
   std::vector<std::string> strVector;
   std::stringstream strStream;
   bool isInsideQuote = false;
   for ( char& c : s ) {
      if ( c == '\"' ) {
	 isInsideQuote = !isInsideQuote;
	 if ( !strStream.str().empty() ) {
	    strVector.push_back(strStream.str());
	    strStream.str("");
	    continue;
	 }
	 continue;
      }
      if ( isInsideQuote ) strStream << c;
      else if ( c == ' ' || c == '\t' || c == '=' ) {
	 if ( !strStream.str().empty() ) {
	    strVector.push_back(strStream.str());
	    strStream.str("");
	    continue;
	 }
      }
      else strStream << c;
   }
   if ( !strStream.str().empty() ) strVector.push_back(strStream.str());
   return strVector;
}

void ConfReader::readFermentables(std::string fileName,
				  std::vector<fermentable> &fermentables)
{
   std::ifstream confFile(fileName);
   if ( !confFile.is_open() ) {
      std::cerr << "ERROR: Could not open " << fileName << '\n';
   }
   std::string line;
   std::vector<std::string> strings;
   // Read fermentables from file
   while ( std::getline(confFile,line) ) {
      if ( line.compare("# Fermentables") == 0 ) {
      	 while ( std::getline(confFile,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
	    for ( std::vector<fermentable>::iterator it = fermentables.begin();
		  it != fermentables.end(); ++it ) {
	       if ( it->name.compare(strings.at(0)) == 0 ) {
		  it->extract = stof(strings.at(1));
		  it->color = stof(strings.at(2));
	       }
	    }
      	 }
	 break;
      }
   }
}
				  
void ConfReader::readRecipe(std::string fileName,
			    std::map<std::string,std::string> &metadata,
			    std::vector<fermentable> &fermentables,
			    std::vector<hop> &hops,
			    std::vector<yeast> &yeasts,
			    std::vector<mash> &mashes,
			    std::string &note
			    )
{
   std::ifstream recipe(fileName);
   std::string line;
   std::vector<std::string> strings;
   // Read recipe from file
   while ( std::getline(recipe,line) ) {
      if ( line.compare("# Metadata") == 0 ) {
	 while ( !line.empty() ) {
	    std::getline(recipe,line);
	    if ( line.empty() ) break;
	    else if ( line.at(0) == '#' ) continue;
	    strings = splitConfString(line);
	    metadata[strings.at(0)] = strings.at(1);
	 }
      }
      else if ( line.compare("# Fermentables") == 0 ) {
      	 while ( std::getline(recipe,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
      	    fermentable f;
      	    f.name = strings.at(0);
      	    f.weight = stof(strings.at(1));
      	    if ( strings.size() > 2 ) {
      	       if ( strings.at(2).compare("mash") != 0 ) f.mash = false;
      	    }
      	    fermentables.push_back(f);
      	 }
      }
      else if ( line.compare("# Hops") == 0 ) {
      	 while ( std::getline(recipe,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
      	    hop h;
      	    h.name = strings.at(0);
      	    h.alpha = stof(strings.at(1));
      	    h.weight = stof(strings.at(2));
      	    h.time = stof(strings.at(3));
      	    hops.push_back(h);
      	 }
      }
      else if ( line.compare("# Yeast") == 0 ) {
      	 while ( std::getline(recipe,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
      	    yeast y;
      	    y.name = strings.at(0);
      	    y.temperature = stof(strings.at(1));
      	    y.time = stof(strings.at(2));
      	    yeasts.push_back(y);
      	 }
      }
      else if ( line.compare("# Mash") == 0 ) {
      	 while ( std::getline(recipe,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
      	    mash m;
      	    m.name = strings.at(0);
      	    m.volume = stof(strings.at(1));
      	    m.temperature = stof(strings.at(2));
      	    m.time = stof(strings.at(3));
      	    mashes.push_back(m);
      	 }
      }
      else if ( line.compare("# Notes") == 0 ) {
      	 std::stringstream noteStream;
      	 bool isInsideQuote = false;
      	 while ( std::getline(recipe,line) ) {
      	    if ( line.empty() && !isInsideQuote ) break;
      	    else if ( isInsideQuote ) {
      	       if ( line.empty() ) noteStream << '\n';
      	       else if ( line.at(line.size() - 1) == '"' ) {
      		  if ( !line.compare("\"") == 0 ) { 
      		     noteStream << line.substr(0, line.size() - 1) << '\n';
      		  }
      		  isInsideQuote = false;
      	       }
      	       else noteStream << line << '\n';
      	    }
      	    else if ( line.at(0) == '#' ) continue;
      	    else if ( line.at(0) == '"' ) {
      	       noteStream << line.substr(1, line.size() - 1) << '\n';
      	       isInsideQuote = true;
      	       continue;
      	    }
      	    else {
      	       noteStream << line;
      	       noteStream << '\n';
      	    }
      	 }
      	 note = noteStream.str();
      }
   }
}
