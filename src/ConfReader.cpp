#include "ConfReader.hpp"

std::vector<std::string> ConfReader::splitConfString(std::string s) 
{
   std::vector<std::string> strVector;
   std::stringstream strStream;
   bool isInsideQuote = false;
   for ( char& c : s ) {
      if ( c == '#' ) break;
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

bool ConfReader::readYeasts(std::string fileName,
			    std::vector<yeast> &yeasts)
{
   std::ifstream confFile(fileName);
   if ( !confFile.is_open() ) return false;
   std::string line;
   std::vector<std::string> strings;
   // Read fermentables from file
   while ( std::getline(confFile,line) ) {
      if ( line.compare("# Yeast") == 0 ) {
      	 while ( std::getline(confFile,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
	    for ( std::vector<yeast>::iterator it = yeasts.begin();
		  it != yeasts.end(); ++it ) {
	       if ( it->name.compare(strings.at(0)) == 0 ) {
		  it->attenuationLow = stod(strings.at(1));
		  it->attenuationHigh = stod(strings.at(2));
	       }
	    }
      	 }
	 break;
      }
   }
   confFile.close();
   return true;
}

bool ConfReader::readFermentables(std::string fileName,
				  std::vector<fermentable> &fermentables)
{
   std::ifstream confFile(fileName);
   if ( !confFile.is_open() ) return false;
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
		  it->extract = stod(strings.at(1));
		  it->color = stod(strings.at(2));
	       }
	    }
      	 }
	 break;
      }
   }
   confFile.close();
   return true;
}
				  
bool ConfReader::readRecipe(std::string fileName,
			    std::map<std::string,std::string> &metadata,
			    std::vector<fermentable> &fermentables,
			    std::vector<hop> &hops,
			    std::vector<yeast> &yeasts,
			    std::vector<mash> &mashes,
			    std::string &note,
			    BrewSpecifications &observations
			    )
{
   std::ifstream recipe(fileName);
   if ( !recipe.is_open() ) return false;
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
      	    f.weight = stod(strings.at(1));
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
      	    h.alpha = stod(strings.at(1));
      	    h.weight = stod(strings.at(2));
      	    h.time = stod(strings.at(3));
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
      	    y.temperature = stod(strings.at(1));
      	    y.time = stod(strings.at(2));
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
      	    m.volume = stod(strings.at(1));
      	    m.temperature = stod(strings.at(2));
      	    m.time = stod(strings.at(3));
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
      else if ( line.compare("# Observations") == 0 ) {
      	 while ( std::getline(recipe,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
      	    if ( strings.at(0).compare("OG") == 0 ) {
	       observations.OG = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("FG") == 0 ) {
	       observations.FG = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("PreboilSG") == 0 ) {
	       observations.preboilSG = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("PreboilVolume") == 0 ) {
	       observations.preboilVolume = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("PostboilVolume") == 0 ) {
	       observations.postboilVolume = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("FermenterVolume") == 0 ) {
	       observations.fermenterVolume = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("Bitterness") == 0 ) {
	       observations.bitterness = stod(strings.at(1));
      	    }
      	    else if ( strings.at(0).compare("Color") == 0 ) {
	       observations.color = stod(strings.at(1));
      	    }
      	 }
      }
   }
   recipe.close();
   return true;
}

bool ConfReader::readBrewery(std::string fileName, brewery &b)
{
   std::ifstream confFile(fileName);
   if ( !confFile.is_open() ) return false;
   std::string line;
   std::vector<std::string> strings;
   // Read fermentables from file
   while ( std::getline(confFile,line) ) {
      if ( line.compare("# Brewery") == 0 ) {
      	 while ( std::getline(confFile,line) ) {
      	    if ( line.empty() ) break;
      	    else if ( line.at(0) == '#' ) continue;
      	    strings = splitConfString(line);
	    if ( strings.at(0).compare("Name") == 0 ) {
	       b.name = strings.at(1);
	    }
	    else if ( strings.at(0).compare("Efficiency") == 0 ) {
	       b.efficiency = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("BoilEvaporationRate") == 0 ) {
	       b.boilEvaporationRate = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("MashDeadSpace") == 0 ) {
	       b.mashDeadSpace = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("KettleDeadSpace") == 0 ) {
	       b.kettleDeadSpace = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("WaterLostToMalt") == 0 ) {
	       b.waterLostToMalt = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("WaterLostToHops") == 0 ) {
	       b.waterLostToHops = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("MashTunMass") == 0 ) {
	       b.mashTunMass = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("MashTunSpecificHeatCapacity")
		      == 0 ) {
	       b.mashTunSpecificHeatCapacity = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("HopsUtilization") == 0 ) {
	       b.hopsUtilization = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("MashTunTemperature")
		      == 0 ) {
	       b.mashTunTemperature = stod(strings.at(1));
	    }
	    else if ( strings.at(0).compare("GrainMashInTemperature")
		      == 0 ) {
	       b.grainMashInTemperature = stod(strings.at(1));
	    }
      	 }
	 break;
      }
   }
   confFile.close();
   return true;
}
