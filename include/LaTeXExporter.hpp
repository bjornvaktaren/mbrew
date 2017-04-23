#ifndef LATEXEXPORTER_H
#define LATEXEXPORTER_H

#include <cmath>
#include <fstream>
#include <sstream>

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
   std::string m_preample = "\\documentclass[DIV=12,10pt]{scrartcl}\n"
      "\n"
      "\\usepackage{fontspec}\n"
      "\\usepackage{siunitx}\n"
      "\\usepackage{booktabs}\n"
      "\n"
      "\\setmainfont{LiberationSans}\n"
      "\n"
      "\\subtitle{Beer Recipe and Brewlog}\n";
   std::string formatQuantity(double quantity, int precision = 1);
};

#endif
