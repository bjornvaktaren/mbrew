#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

struct fermentable
{
   std::string name;
   float weight = 0.0;
   bool mash = true;
};

struct hop
{
   std::string name;
   float alpha = 0.0;
   float weight = 0.0;
   float time = 0.0;
};

struct mash
{
   std::string name;
   float volume = 0.0;
   float temperature = 0.0;
   float time = 0.0;   
};

struct yeast
{
   std::string name;
   float temperature = 0.0;
   float time = 0.0;   
};


std::vector<std::string> splitConfString(std::string s) 
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

int main()
{
   std::map<std::string,std::string> metadata;
   std::vector<fermentable> fermentables;
   std::vector<hop> hops;
   std::vector<yeast> yeasts;
   std::vector<mash> mashes;
   std::string note;

   std::ifstream recipe("recipe.conf");
   std::string line;
   std::string name;
   std::string style;
   std::vector<std::string> strings;
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
   std::cout << "# Metadata\n";
   for ( auto m : metadata ) {
      std::cout << m.first << "   " << m.second << '\n';
   }
   std::cout << '\n';
   std::cout << "# Fermentables\n";
   for ( auto f : fermentables ) {
      std::cout << f.name << "   " << f.weight << "   " << f.mash << '\n';
   }
   std::cout << '\n';
   std::cout << "# Hops\n";
   for ( auto h : hops ) {
      std::cout << h.name << "   " << h.alpha << "   " 
		<< h.weight << "   " << h.time << '\n';
   }
   std::cout << '\n';
   std::cout << "# Yeast\n";
   for ( auto y : yeasts ) {
      std::cout << y.name << "   " << y.temperature << "   " 
		<< y.time << '\n';
   }
   std::cout << '\n';
   std::cout << "# Mash\n";
   for ( auto m : mashes ) {
      std::cout << m.name << "   " << m.volume << "   " 
		<< m.temperature << "   " << m.time << '\n';
   }
   std::cout << "\n# Note\n" << note;
   return 0;
}
