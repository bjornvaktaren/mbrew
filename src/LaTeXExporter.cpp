#include "LaTeXExporter.hpp"

std::string LaTeXExporter::formatQuantity(double quantity, int precision)
{
   
   std::stringstream ss;
   ss << std::fixed << std::setprecision(precision) << quantity;
   return ( quantity < kConst::kDoubleUndefined*0.999 && 
	    quantity > kConst::kDoubleUndefined*1.001 ? 
	    kConst::kStrUndefined : ss.str() );
}

void LaTeXExporter::save(std::string file)
{
   std::ofstream f;
   f.open(file);
   f << m_preample 
     << "\\subject{Beer Recipe and Logbook}\n"
     << "\\title{" << m_brew->getMetadata("Name") << "}\n"
     << "\\subtitle{" << m_brew->getMetadata("Style") << "}\n"
     << "\\author{" << m_brew->getMetadata("Brewer") << "}\n"
     << "\\date{" << m_brew->getMetadata("Date") << "}\n"
     << '\n'
     << "\\begin{document}\n"
     << "\\maketitle\n"
     << '\n'
     << "\\section*{Specifications}\n"
     << "\\begin{tabular}{l c c}\n"
     << "\\toprule\n"
     << " &  Estimations  & Observed \\\\\n"
     << "\\midrule\n"
     << std::fixed
     << "Color & " << formatQuantity(m_brew->getColorMoreyEBC(), 0)
     << " EBC & "
     << formatQuantity(m_brew->getObservedColor(), 0)
     << (m_brew->getObservedColor() > 0 ? " EBC" : " " ) << " \\\\\n"
     << "Preboil Volume & " << formatQuantity(m_brew->getPreboilVolume(), 1)
     << " liter & " << formatQuantity(m_brew->getObservedPreboilVolume(), 1)
     << " liter \\\\\n"
     << "Postboil Volume & " << formatQuantity(m_brew->getPostboilVolume(), 1)
     << " liter & " << formatQuantity(m_brew->getObservedPostboilVolume(), 1)
     << " liter \\\\\n"
     << "Fermenter Volume & "
     << formatQuantity(m_brew->getVolumeIntoFermenter(), 1) << " liter & "
     << formatQuantity(m_brew->getObservedFermenterVolume(), 1)
     << " liter\\\\\n"
     << "Preboil SG & " << formatQuantity(m_brew->getPreboilSG(), 3) << " & "
     << formatQuantity(m_brew->getObservedPreboilSG(), 3) << " \\\\\n"
     << "OG & " << formatQuantity(m_brew->getPostboilSG(), 3) << " & "
     << formatQuantity(m_brew->getObservedOG(), 3) << " \\\\\n"
     << "FG & " << formatQuantity(m_brew->getFGLow(), 3) << "--" 
     << formatQuantity(m_brew->getFGHigh(), 3) << " & " 
     << formatQuantity(m_brew->getObservedFG(), 3) << " \\\\\n"
     << "ABV & " << formatQuantity(m_brew->getABVLow(),1) << "--"
     << formatQuantity(m_brew->getABVHigh(),1) << "\\% & "
     << formatQuantity(m_brew->getABV(m_brew->getObservedOG(),
				      m_brew->getObservedFG()), 1)
     << "\\% \\\\\n"
     << "Bitterness & " << formatQuantity(m_brew->getTotalIBU(), 0)<< " IBU & "
     << formatQuantity(m_brew->getObservedBitterness(), 0) << " IBU \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Bewery}\n"
     << "\\begin{tabular}{l c c}\n"
     << "\\toprule\n"
     << " &  Specification  & Observed \\\\\n"
     << "\\midrule\n"
     << "Name  & " << m_brew->getBreweryName() << " & \\\\\n"
     << "Mash efficiency & "
     << formatQuantity(m_brew->getBreweryEfficiency(), 2) << " & "
     << formatQuantity(m_brew->getObservedEfficiency(), 2) << " \\\\\n"
     << "Boil evaporation rate & "
     << formatQuantity(m_brew->getBreweryBoilEvaporationRate(), 1) << " & "
     << formatQuantity(m_brew->getObservedBoilEvaporationRate(), 1)
     << " \\\\\n"
      // << "Hop utilization & " 
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Fermentables}\n"
     << "\\begin{tabular}{l c c c c c}\n"
     << "\\toprule\n"
     << "Name & Weight (g) & Added & Color (EBC) & Potential (\\%) & OG \\\\\n"
     << "\\midrule\n";
   double totalWeight = 0.0;
   for ( auto ferm : m_brew->getFermentables() ) {
      f << ferm.name << " & " << formatQuantity(ferm.weight, 0) << " & " 
	<< ( ferm.mash ? "Mash" : "Boil" ) << " & "
	<< formatQuantity(ferm.color, 1) << " & "
	<< formatQuantity(ferm.extract, 1) << " & "
	<< formatQuantity(1.0 +
			  m_brew->getOechle(ferm, m_brew->getPostboilVolume())
			  /1000.0, 3)
	<< " \\\\\n"; 
      totalWeight += ferm.weight;
   }
   f << "\\midrule\n"
     << "Total & " << formatQuantity(totalWeight, 0) << " & & & & \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Hops}\n"
     << "\\begin{tabular}{l c c c c c}\n"
     << "\\toprule\n"
     << "Name & Alpha acid (\\%) & Weight (g) &"
     << " Time (min) & Bitterness (IBU)\\\\\n"
     << "\\midrule\n";
   totalWeight = 0.0;
   for ( auto h : m_brew->getHops() ) {
      f << h.name << " & " << std::setprecision(1) << std::fixed
	<< h.alpha << " & "  << h.weight << " & " << std::setprecision(0)
	<< h.time << " & " << std::setprecision(1) << m_brew->getIBU(h)
	<< " \\\\\n"; 
      totalWeight += h.weight;
   }
   f << "\\midrule\n"
     << "Total & & " << totalWeight << " & & " << m_brew->getTotalIBU()
     << " \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Yeast}\n"
     << "\\begin{tabular}{l c c c}\n"
     << "\\toprule\n"
     << "Name & Attenuation (\\%) & Temperature (C) & Time (days)  \\\\\n"
     << "\\midrule\n";
   double longestTime = 0.0;
   for ( auto y : m_brew->getYeasts() ) {
      f << y.name << " & " << std::fixed << std::setprecision(0)
	<< y.attenuationLow*100.0 << " to " << y.attenuationHigh*100.0 << " & "
	<< std::setprecision(1) << y.temperature << " & "
	<< std::setprecision(0) << y.time << " \\\\\n";
      if ( longestTime < y.time ) longestTime = y.time;
   }
   f << "\\midrule\n"
     << "Total & & & " << std::fixed << std::setprecision(0) << longestTime
     << " \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Mash, Sparge, and Boil}\n"
     << "\\begin{tabular}{l c c c c}\n"
     << "\\toprule\n"
     << "Name & Water added (liter) & Temperature (C) & Time (min) & "
     << "Strike water temp. (C) \\\\\n"
     << "\\midrule\n";
   for ( auto m : m_brew->getMashes() ) {
      std::stringstream ss1;
      ss1 << std::fixed << std::setprecision(1) << m.mashInTemperature;
      std::stringstream ss2;
      ss2 << std::fixed << std::setprecision(1) << m.volume;
      f << m.name << " & " << std::fixed << std::setprecision(1)
	<< ( m.volume < 0.01 ? "  " : ss2.str() ) 
	<< " & " << m.temperature << " & "
	<< std::setprecision(0) << m.time << " & "
	<< ( std::abs(m.mashInTemperature) > 100.0 ? "  " : ss1.str() )
	<<" \\\\\n";
   }
   f << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Notes}\n"
     << m_brew->getNote()
     << '\n'
     << "\\end{document}\n";
}

   
