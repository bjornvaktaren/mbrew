#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include <string.h>

// Local includes
#include <Components.hpp>
#include <ConfReader.hpp>

namespace kConst{
   double kkg2lbs = 2.20462262;
   double kGal2Litre = 3.78541178;
   double kEBC2SRM = 0.508;
}

double getIBU(hop h, double gravity)
{
   // Using the Tinseth formula.
   // grams x alpha x utilization x 0.55
   // utilization = 1.65*0.000125^(Gb-1) * (1 - exp(-0.04*T))/4.15
   // where Gb is the boil gravity and T is the boil time. (howtobrew.com)
   double utilization = ( 1.65*pow(0.000125, gravity - 1.0) *
			 (1.0 - exp(-0.04 * h.time)) );
   return h.weight*h.alpha*utilization*0.55;
}

double getOechle(fermentable f, double volume, double eff)
{
   double oechle = 0.0;
   oechle += 46.0*f.extract*f.weight*kConst::kkg2lbs*1e-5;
   return eff*oechle/volume*kConst::kGal2Litre;
}

double getSG(std::vector<fermentable> fermentables, double volume, double eff)
{
   double oechle = 0.0;
   for ( auto f : fermentables ) {
      oechle += 46.0*f.extract*f.weight*kConst::kkg2lbs*1e-5;
   }
   return (1.0 + eff*oechle*1e-3/volume*kConst::kGal2Litre);
}

double lovibondToSRM(double lovibond)
{
   return (1.3546*lovibond) - 0.76;
}

double srmToLovibond(double srm)
{
   return (srm + 0.76)/1.3546;
}

double getColorMoreyEBC(std::vector<fermentable> fermentables, double volume)
{
   // Calculates beer color in EBC using Morey's equation
   // MCU = (grain_color_in_lov * grain_weight_in_lbs) / volume_gallons;
   // SRM_color = 1.4922 * MCU^0.6859
   double mcu = 0.0;
   for ( fermentable f : fermentables ) {
      if ( !f.mash ) continue;
      mcu += 1e-3*f.weight*kConst::kkg2lbs*f.color*kConst::kEBC2SRM;
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
		   << "                input recipe file\n\n"
		   << "        -f [FERMENTABLES], --recipe [FERMENTABLES]\n"
		   << "                fermentables specfication file, "
		   << "default is ferms.conf\n\n";
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
		<< f.color << "   " << f.extract << "   " 
		<< getOechle(f,10.0,0.8) << '\n';
   }
   std::cout << '\n';
   std::cout << "# Hops\n";
   double totalIBU = 0.0;
   for ( auto h : hops ) {
      std::cout << h.name << "   " << h.alpha << "   " 
		<< h.weight << "   " << h.time << "   "
		<< getIBU(h,getSG(fermentables,10.0,0.8)) << '\n';
      totalIBU += getIBU(h,getSG(fermentables,10.0,0.8));
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
   std::cout << "Color: " << getColorMoreyEBC(fermentables,12.0) << " EBC\n";
   std::cout << "SG: " << getSG(fermentables,10.0,0.8) << " \n";
   std::cout << "IBU: " << totalIBU << " \n";
   return 0;
}
