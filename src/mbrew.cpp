#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string.h>

// Local includes
#include <Components.hpp>
#include <ConfReader.hpp>
#include <Brew.hpp>
#include <Constants.hpp>

void help(std::string prog)
{
   std::cout << prog << "\n\n"
	     << "SYNOPSIS\n"
	     << "        " << prog << " [OPTION]...\n\n"
	     << "DESCRIPTION\n"
	     << "        -r FILE, --recipe FILE\n"
	     << "                input recipe file\n\n"
	     << "        -f FILE, --fermentables FILE\n"
	     << "                fermentables specfication file, "
	     << "default is ferms.conf\n\n"
	     << "        -y FILE, --yeast FILE\n"
	     << "                yeast specfication file, "
	     << "default is yeast.conf\n\n"
	     << "        -b FILE, --brewery FILE\n"
	     << "                brewery specfication file, "
	     << "default is brewery.conf\n\n";
}

int main(int argc, char* argv[])
{
   std::string inputRecipe;
   std::string fermentablesFileName = "ferms.conf";
   std::string yeastFileName = "yeast.conf";
   std::string breweryFileName = "brewery.conf";
   // Read user input
   for ( int i = 1; i < argc; i++ ) {
      if ( strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
	 help(argv[0]);
	 exit(EXIT_SUCCESS);
      }
      else if ( strcmp(argv[i], "--recipe") == 0 ||
		strcmp(argv[i], "-r") == 0 ) {
	 inputRecipe = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--fermentables") == 0 ||
		strcmp(argv[i], "-f") == 0 ) {
	 fermentablesFileName = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--yeast") == 0 ||
		strcmp(argv[i], "-y") == 0 ) {
	 yeastFileName = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--brewery") == 0 ||
		strcmp(argv[i], "-b") == 0 ) {
	 breweryFileName = std::string(argv[i+1]);
	 ++i;
      }
      else {
	 std::cout << "Unrecognized option: " << argv[i] << '\n';
	 exit(EXIT_FAILURE);
      }
   }
   if ( inputRecipe.empty() ) {
      std::cout << "Please specify an input recipe.\n";
      exit(EXIT_FAILURE);
   }
   if ( fermentablesFileName.compare("ferms.conf") == 0 ) {
      std::cout << "WARNING: fermentables configuration file not specified. "
		<< "Using default " << fermentablesFileName << '\n';
   }
   if ( breweryFileName.compare("brewery.conf") == 0 ) {
      std::cout << "WARNING: brewery configuration file not specified. "
		<< "Using default " << breweryFileName << '\n';
   }
   std::map<std::string,std::string> metadata;
   std::vector<fermentable> fermentables;
   std::vector<hop> hops;
   std::vector<yeast> yeasts;
   std::vector<mash> mashes;
   brewery brewery;
   std::string note;
   ConfReader confReader;
   if ( !confReader.readRecipe(inputRecipe, metadata, 
			       fermentables, hops, yeasts, mashes, note) ) {
      std::cerr << "ERROR: Could not open " << inputRecipe << '\n';
      exit(EXIT_FAILURE);
   }
   if ( !confReader.readFermentables(fermentablesFileName, fermentables) ) {
      std::cerr << "ERROR: Could not open " << fermentablesFileName << '\n';
      exit(EXIT_FAILURE);
   }
   if ( !confReader.readYeasts(yeastFileName, yeasts) ) {
      std::cerr << "ERROR: Could not open " << yeastFileName << '\n';
      exit(EXIT_FAILURE);
   }
   if ( !confReader.readBrewery(breweryFileName, brewery) ) {
      std::cerr << "ERROR: Could not open " << breweryFileName << '\n';
      exit(EXIT_FAILURE);
   }
   Brew brew(brewery, fermentables, mashes, hops, yeasts);
   brew.setNote(note);
   brew.setMetadata(metadata);
   std::cout << "# Metadata\n";
   for ( auto m : metadata ) {
      std::cout << m.first << "   " << m.second << '\n';
   }
   std::cout << '\n';
   std::cout << "# Fermentables\n";
   for ( auto f : fermentables ) {
      std::cout << f.name << "   " << f.weight << "   " << f.mash << "   "
		<< f.color << "   " << f.extract << "   " 
		<< brew.getOechle(f, 10.0) << '\n';
   }
   std::cout << '\n';
   std::cout << "# Hops\n";
   double totalIBU = 0.0;
   for ( auto h : hops ) {
      std::cout << h.name << "   " << h.alpha << "   " 
		<< h.weight << "   " << h.time << "   "
		<< brew.getIBU(h,brew.getSG(10.0)) 
		<< '\n';
      totalIBU += brew.getIBU(h,brew.getSG(10.0));
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
   std::cout << "\n# Note\n" << note
	     << '\n'
	     << "# Brewery\n"
	     << brewery.name << "   " << brewery.efficiency << "   " 
	     << brewery.waterLostToMalt << "   " << brewery.mashDeadSpace 
	     << '\n'
	     << "\n# Calculated stuff\n"
	     << "Color: " << brew.getColorMoreyEBC() << " EBC\n"
	     << "Preboil Volume: " << brew.getPreboilVolume() << " \n"
	     << "Postboil Volume: " << brew.getPostboilVolume() << " \n"
	     << "Fermenter Volume: " << brew.getVolumeIntoFermenter() << " \n"
	     << "Preboil SG: " << brew.getPreboilSG() << " \n"
	     << "OG: " << brew.getPostboilSG() << " \n"
	     << "Estimated FG: " << brew.getFGLow() << " to " 
	     << brew.getFGHigh() << '\n'
	     << "IBU: " << totalIBU << " \n";
   return EXIT_SUCCESS;
}
