#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include <string.h>

// Local includes
#include <Components.hpp>
#include <ConfReader.hpp>

// float getIBU(hop h) 
// {
//    // grams x alpha x utilization x 0.55
//    return 0.0
// }

namespace kConst{
   float kkg2lbs = 2.20462262;
   float kGal2Litre = 3.78541178;
   float kEBC2SRM = 0.508;
}

float lovibondToSRM(float lovibond)
{
   return (1.3546*lovibond) - 0.76;
}

float srmToLovibond(float srm)
{
   return (srm + 0.76)/1.3546;
}

float getColorMoreyEBC(std::vector<fermentable> fermentables, float volume)
{
   // Calculates beer color in EBC using Morey's equation
   // MCU = (grain_color_in_lov * grain_weight_in_lbs) / volume_gallons;
   // SRM_color = 1.4922 * MCU^0.6859
   float mcu = 0.0;
   for ( fermentable f : fermentables ) {
      if ( !f.mash ) continue;
      mcu += 1e-3*f.weight*kConst::kkg2lbs*f.color*kConst::kEBC2SRM;
      std::cout << f.weight << '*' << kConst::kkg2lbs << '*' 
		<< f.color << '*' << kConst::kEBC2SRM << '\n';
   }
   mcu = mcu/(volume/kConst::kGal2Litre);
   return 1.4922*pow(mcu,0.6859)/kConst::kEBC2SRM; // Morey's formula
}


int main(int argc, char* argv[])
{
   std::string inputRecipe;
   std::string fermentablesFileName = "ferms.conf";
   // Read user input
   for ( int i = 1; i < argc; i++ ) {
      if ( strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
	 std::cout << "SYNOPSIS\n"
		   << "        mbrew [OPTION]...\n\n"
		   << "DESCRIPTION\n"
		   << "        -r [RECIPE], --recipe [RECIPE]\n"
		   << "                input recipe\n\n";
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
   std::map<std::string,std::string> metadata;
   std::vector<fermentable> fermentables;
   std::vector<hop> hops;
   std::vector<yeast> yeasts;
   std::vector<mash> mashes;
   std::string note;
   ConfReader confReader;
   confReader.readRecipe(inputRecipe, metadata, fermentables, hops, yeasts,
   			 mashes, note);
   confReader.readFermentables(fermentablesFileName, fermentables);
   std::cout << "# Metadata\n";
   for ( auto m : metadata ) {
      std::cout << m.first << "   " << m.second << '\n';
   }
   std::cout << '\n';
   std::cout << "# Fermentables\n";
   for ( auto f : fermentables ) {
      std::cout << f.name << "   " << f.weight << "   " << f.mash << "   "
		<< f.color << "   " << f.extract << '\n';
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

   std::cout << "\n# Calculated stuff\n";
   std::cout << "Color: " << getColorMoreyEBC(fermentables,12.0f) << " EBC\n";
   return 0;
}
