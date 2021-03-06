#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <string.h>

// Local includes
// #include <Components.hpp>
#include <ConfReader.hpp>
#include <Brew.hpp>
#include <LaTeXExporter.hpp>
// #include <Constants.hpp>

void help(const std::string &prog)
{
   std::cout << prog << "\n\n"
	     << "SYNOPSIS\n"
	     << "        " << prog << " RECIPE [OPTION]...\n"
	     << "        where RECIPE is a recipe file\n\n"
	     << "DESCRIPTION\n"
	     << "        -v, --verbose\n"
	     << "                verbose mode\n\n"
	     << "        -l FILE, --latex FILE\n"
	     << "                export LaTeX file\n\n"
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
   std::string fermentablesFileName;
   std::string yeastFileName;
   std::string breweryFileName;
   std::string latexFile;
   bool verbose = false;
   
   if ( argc < 2 ) {
      std::cout << "Please supply a recipe file\n";
      exit(EXIT_FAILURE);
   }
   else if ( strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
      help(argv[0]);
      exit(EXIT_SUCCESS);
   }
   else inputRecipe = std::string(argv[1]);

   // Read user input
   for ( int i = 2; i < argc; i++ ) {
      if ( strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
	 help(argv[0]);
	 exit(EXIT_SUCCESS);
      }
      else if ( strcmp(argv[i], "--fermentables") == 0 ||
		strcmp(argv[i], "-f") == 0 ) {
         if ( argc == i+1 ) {
            std::cerr << "ERROR: " << argv[i] << " takes exactly 1 argument\n";
            exit(EXIT_FAILURE);
         }
	 fermentablesFileName = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--latex") == 0 ||
		strcmp(argv[i], "-l") == 0 ) {
         if ( argc == i+1 ) {
            std::cerr << "ERROR: " << argv[i] << " takes exactly 1 argument\n";
            exit(EXIT_FAILURE);
         }
	 latexFile = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--yeast") == 0 ||
		strcmp(argv[i], "-y") == 0 ) {
         if ( argc == i+1 ) {
            std::cerr << "ERROR: " << argv[i] << " takes exactly 1 argument\n";
            exit(EXIT_FAILURE);
         }
	 yeastFileName = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--brewery") == 0 ||
		strcmp(argv[i], "-b") == 0 ) {
         if ( argc == i+1 ) {
            std::cerr << "ERROR: " << argv[i] << " takes exactly 1 argument\n";
            exit(EXIT_FAILURE);
         }
	 breweryFileName = std::string(argv[i+1]);
	 ++i;
      }
      else if ( strcmp(argv[i], "--verbose") == 0 ||
		strcmp(argv[i], "-v") == 0 ) {
	 verbose = true;
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
   if ( fermentablesFileName.empty() ) {
      fermentablesFileName = "ferms.conf";
      if ( verbose ) {
	 std::cout << "WARNING: fermentables configuration file not specified. "
		   << "Using default " << fermentablesFileName << '\n';
      }
   }
   if ( breweryFileName.empty() ) {
      breweryFileName = "brewery.conf";
      if ( verbose ) {
	 std::cout << "WARNING: brewery configuration file not specified. "
		   << "Using default " << breweryFileName << '\n';
      }
   }
   if ( yeastFileName.empty() ) {
      yeastFileName = "yeast.conf";
      if ( verbose ) {
	 std::cout << "WARNING: yeast configuration file not specified. "
		   << "Using default " << yeastFileName << '\n';
      }
   }
   std::map<std::string,std::string> metadata;
   std::vector<fermentable> fermentables;
   std::vector<hop> hops;
   std::vector<yeast> yeasts;
   std::vector<mash> mashes;
   brewery brewery;
   std::string note;
   BrewSpecifications observations;
   ConfReader confReader;
   if ( !confReader.readRecipe(inputRecipe, metadata, fermentables, hops, 
			       yeasts, mashes, note, observations) ) {
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
   brew.setObservations(observations);
   brew.print();

   if ( ! latexFile.empty() ) {
      LaTeXExporter exporter(brew);
      exporter.save(latexFile);
   }

   return EXIT_SUCCESS;
}
