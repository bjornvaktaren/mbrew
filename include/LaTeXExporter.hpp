#ifndef LATEXEXPORTER_H
#define LATEXEXPORTER_H

// #include <vector>
// #include <map>
// #include <math.h>
#include <fstream>
// #include <iomanip>
// #include <string>

// Local includes
#include <Brew.hpp>

class LaTeXExporter
{
public:
   LaTeXExporter(){}
   LaTeXExporter(Brew *brew) { m_brew = brew; }
   ~LaTeXExporter(){}
   void save(std::string file);
private:
   // class members
   Brew *m_brew;
   std::string m_preample = "\\documentclass{scrartcl}\n"
      "\n"
      "\\usepackage{fontspec}\n"
      "\\usepackage{siunitx}\n"
      "\\usepackage{booktabs}\n"
      "\n"
      "\\setmainfont{LiberationSans}\n"
      "\n"
      "\\subtitle{Beer Recipe and Brewlog}\n";
   
   // private function

};

#endif
