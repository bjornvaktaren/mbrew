#include "Brew.hpp"

Brew::Brew(brewery brewery,
	   std::vector<fermentable> fermentables, 
	   std::vector<mash> mashes, 
	   std::vector<hop> hops) : 
   m_brewery(brewery), 
   m_fermentables(fermentables), 
   m_mashes(mashes), 
   m_hops(hops)
{
   this->calculateStrikeWaterTemperatures();
}

Brew::Brew(brewery brewery,
	   std::vector<fermentable> fermentables, 
	   std::vector<mash> mashes, 
	   std::vector<hop> hops,
	   std::vector<yeast> yeasts) : 
   m_brewery(brewery), 
   m_fermentables(fermentables), 
   m_mashes(mashes), 
   m_hops(hops), 
   m_yeasts(yeasts)
{
   this->calculateStrikeWaterTemperatures();
}

void Brew::calculateStrikeWaterTemperatures()
{
   m_mashes.at(0).mashInTemperature = getStrikeWaterTemperature(m_mashes.at(0));
   for ( unsigned int i = 1; i < m_mashes.size(); ++i ) {
      m_mashes.at(i).mashInTemperature
	 = getStrikeWaterTemperature(m_mashes.at(i),
				     m_mashes.at(i-1).mashInTemperature);
   }
}

double Brew::getStrikeWaterTemperature(mash mash, double fromTemperature)
{
   // Calculate strike water temperature using specific heat capacity, mass,
   // and temperature. Assuming speficic heat capacity of the resulting mash is
   // the mass-average of the constituences (grain, tun, and water).
   double massFermentables = 0.0;
   for ( auto f : m_fermentables ) massFermentables += f.weight/1e3; // to kg
   double eGrain   = ( massFermentables
		       * kConst::kGrainSpecificHeat
		       * fromTemperature );
   double eMashTun = ( m_brewery.mashTunMass
		       * m_brewery.mashTunSpecificHeatCapacity
		       * fromTemperature );
   return ( ( ( kConst::kWaterSpecificHeat*mash.volume
		+ kConst::kGrainSpecificHeat*massFermentables
		+ m_brewery.mashTunSpecificHeatCapacity*m_brewery.mashTunMass )
	      * mash.temperature - eGrain - eMashTun )
	    / (mash.volume*kConst::kWaterSpecificHeat));
}

double Brew::getStrikeWaterTemperature(mash mash)
{
   // Calculate strike water temperature using specific heat capacity, mass,
   // and temperature. Assuming speficic heat capacity of the resulting mash is
   // the mass-average of the constituences (grain, tun, and water).
   double massFermentables = 0.0;
   for ( auto f : m_fermentables ) massFermentables += f.weight/1e3; // to kg
   double eGrain   = ( massFermentables
		       * kConst::kGrainSpecificHeat
		       * m_brewery.grainMashInTemperature );
   double eMashTun = ( m_brewery.mashTunMass
		       * m_brewery.mashTunSpecificHeatCapacity
		       * m_brewery.mashTunTemperature );
   return ( ( ( kConst::kWaterSpecificHeat*mash.volume
		+ kConst::kGrainSpecificHeat*massFermentables
		+ m_brewery.mashTunSpecificHeatCapacity*m_brewery.mashTunMass )
	      * mash.temperature - eGrain - eMashTun )
	    / (mash.volume*kConst::kWaterSpecificHeat));
}

double Brew::getPreboilVolume()
{
   double volume = 0.0;
   for ( auto m : m_mashes ) {
      volume += m.volume;
   }
   double maltMass = 0.0;
   for ( auto f : m_fermentables ) {
      if ( !f.mash ) continue;
      maltMass += f.weight;
   }
   return (volume - m_brewery.waterLostToMalt*maltMass*1e-3
	   - m_brewery.mashDeadSpace);
}

double Brew::getBoilDuration()
{
   double boilTime = 0.0;
   // look if total boil time is specified in mashes
   for ( auto m : m_mashes ) {
      if ( m.name.compare("Boil") == 0 || m.name.compare("boil") == 0 ||
	   m.name.compare("BOIL") == 0 ) {
	 boilTime = m.time;
      }
   }
   // if boil not specified in mashed, or if the longest hop addition is longer,
   // take from the longest hop addition
   for ( auto h : m_hops ) {
      if ( h.time > boilTime ) boilTime = h.time;
   }
   return boilTime;
}

double Brew::getPostboilVolume()
{
   return ( getPreboilVolume() 
	    - m_brewery.boilEvaporationRate*getBoilDuration()/60.0 );
}

double Brew::getVolumeAtTime(double boilTime)
{
   return getPreboilVolume() - m_brewery.boilEvaporationRate*boilTime/60.0;
}

double Brew::getVolumeIntoFermenter()
{
   double hopsMass = 0.0;
   for ( auto h : m_hops ) {
      hopsMass += h.weight;
   }
   return (getPostboilVolume() - m_brewery.kettleDeadSpace 
	   - m_brewery.waterLostToHops*hopsMass);
}

double Brew::getIBU(hop h)
{
   // Using the Tinseth formula.
   // IBU = milligrams per liter alpha acid, so
   // IBU = (milligrams_of_hops*alpha_percent/liters_of_wort)*utilization
   // utilization = 1.65*0.000125^(Gb-1) * (1 - exp(-0.04*T))/4.15
   // where Gb is the boil gravity and T is the boil time. (howtobrew.com)

   // calculate average gravity
   double gravity = 0.5*getSG( getVolumeAtTime( getBoilDuration() - h.time ) );
   gravity += 0.5*getSG( getPostboilVolume() );
   double utilization = ( 1.65 * pow(1.25e-4, gravity - 1.0) *
			  (1.0 - exp(-0.04 * h.time) ) / 4.15 );
   return ( m_brewery.hopsUtilization * 10.0 * h.weight * h.alpha * utilization
	    / getPostboilVolume() );
}

double Brew::getTotalIBU()
{
   double ibu = 0.0;
   for ( auto h : m_hops ) {
      ibu += getIBU(h);
   }
   return ibu;
}

double Brew::getOechle(fermentable f, double volume)
{
   double oechle = 0.0;
   oechle += 46.0*f.extract*f.weight*kConst::kkg2lbs*1e-5;
   return m_brewery.efficiency*oechle/volume*kConst::kGal2Litre;
}

double Brew::getPreboilSG()
{
   return getSG(getPreboilVolume());
}

double Brew::getPostboilSG()
{
   return getSG(getPostboilVolume());
}

double Brew::getSG(double volume)
{
   double oechle = 0.0;
   for ( auto f : m_fermentables ) {
      oechle += 46.0*f.extract*f.weight*kConst::kkg2lbs*1e-5;
   }
   return (1.0 + m_brewery.efficiency*oechle*1e-3/volume*kConst::kGal2Litre);
}

double Brew::getFGHigh()
{
   double attenuationLow = 1.0;
   for ( auto y : m_yeasts ) {
      if ( attenuationLow > y.attenuationLow ) {
	 attenuationLow = y.attenuationLow;
      }
   }
   return (1.0 + (getPostboilSG() - 1.0 )*(1.0 - attenuationLow));
}

double Brew::getFGLow()
{
   double attenuationHigh = 0.0;
   for ( auto y : m_yeasts ) {
      if ( attenuationHigh < y.attenuationHigh ) {
	 attenuationHigh = y.attenuationHigh;
      }
   }
   return (1.0 + (getPostboilSG() - 1.0 )*(1.0 - attenuationHigh));
}

double Brew::lovibondToSRM(double lovibond)
{
   return (1.3546*lovibond) - 0.76;
}

double Brew::srmToLovibond(double srm)
{
   return (srm + 0.76)/1.3546;
}

double Brew::getColorMoreyEBC()
{
   // Calculates beer color in EBC using Morey's equation
   // MCU = (grain_color_in_lov * grain_weight_in_lbs) / volume_gallons;
   // SRM_color = 1.4922 * MCU^0.6859
   double mcu = 0.0;
   for ( fermentable f : m_fermentables ) {
      if ( !f.mash ) continue;
      mcu += 1e-3*f.weight*kConst::kkg2lbs*f.color*kConst::kEBC2SRM;
   }
   mcu = mcu/(getPostboilVolume()/kConst::kGal2Litre);
   return 1.4922*pow(mcu,0.6859)/kConst::kEBC2SRM; // Morey's formula
}


void Brew::print()
{
   std::cout << "# Metadata\n";
   unsigned int maxStringLength = 0;
   for ( auto m : m_metadata ) {
      if ( m.first.length() > maxStringLength ) {
	 maxStringLength = m.first.length();
      }
   }
   for ( auto m : m_metadata ) {
      std::cout << std::setw(maxStringLength) << std::left
		<< m.first << "   " << m.second << '\n';
   }

   std::cout << '\n'
	     << std::setw(30) << std::left << "# Fermentables"
	     << std::setw(12) << "Weight (g)" << std::setw(7) << "Added"
      	     << std::setw(13) << "Color (EBC)" << std::setw(11) << "Potential"
	     << "OG\n";
   for ( auto f : m_fermentables ) {
      std::cout << std::setw(30) << std::left
		<< f.name << std::setw(12)<< f.weight << std::setw(7)
		<< (f.mash ? "Mash" : "Boil") << std::setw(13)
		<< f.color << std::setw(11)
		<< f.extract << std::setw(7) << std::setprecision(0) 
		<< int(this->getOechle(f, this->getPostboilVolume())) << '\n';
   }
   std::cout << '\n'
	     << "# Hops                           Alpha (%)  Weight (g)  "
	     << "Time (min)  IBU\n";
   for ( auto h : m_hops ) {
      std::cout << std::setw(30) << h.name << "   " << std::left
		<< std::fixed << std::setw(11) << std::setprecision(1)	 
		<< h.alpha << std::setw(12) << h.weight << std::setw(12)
		<< h.time << std::setw(7) << this->getIBU(h) << '\n';
   }
   std::cout << '\n'
	     << "# Yeast                          Temperature (C)  Time (days)"
	     << '\n';
   for ( auto y : m_yeasts ) {
      std::cout << std::setw(30) << y.name << "   "
		<< std::fixed << std::setw(17) << std::setprecision(1)
		<< y.temperature << std::setw(7) << y.time << '\n';
   }
   std::cout << '\n'
	     << "# Mash                           Vol. (l)  Temp (C)  Time (min)"
	     << "  Strike temp (C)\n";
   for ( auto m : m_mashes ) {
      std::cout << std::setw(30) << m.name << "   "
		<< std::fixed << std::setw(10) << std::setprecision(1)
		<< m.volume << std::setw(10) << m.temperature << std::setw(12)
		<< m.time << std::setw(7) << this->getStrikeWaterTemperature(m)
		<< '\n';
   }
   std::cout << "\n# Note\n" << m_recipeNote
	     << '\n'
	     << "# Brewery\n"
	     << m_brewery.name << "   " << m_brewery.efficiency << "   " 
	     << m_brewery.waterLostToMalt << "   " << m_brewery.mashDeadSpace 
	     << '\n'

	     << "\n# Estimations\n" << std::setiosflags(std::ios::fixed) 
	     << std::setprecision(0) 
	     << "Color:               " << this->getColorMoreyEBC() << " EBC\n"
	     << std::setprecision(1)
	     << "Preboil Volume:      " << this->getPreboilVolume() 
	     << " liter\n"
	     << "Postboil Volume:     " << this->getPostboilVolume() 
	     << " liter\n"
	     << "Fermenter Volume:    " << this->getVolumeIntoFermenter() 
	     << " liter\n"
	     << std::setprecision(3)
	     << "Preboil Gravity:     " << this->getPreboilSG() << " \n"
	     << "Postboil Gravity:    " << this->getPostboilSG() << " \n"
	     << "Postferment Gravity: " << this->getFGLow() << " to " 
	     << this->getFGHigh() << '\n'
	     << std::setprecision(0)
	     << "Bitterness:          " << this->getTotalIBU() << " IBU\n";

}
