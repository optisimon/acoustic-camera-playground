//
// Copyright(C) 2020 Simon Gustafsson (optisimon.com)
//

#include "ArgumentParser.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sstream>

ArgumentParser::ArgumentParser(): 
  num_unparsed_(0), parse_errors_(""), help_("")
{
  clearAllTokens();
};

void ArgumentParser::clearAllTokens(){
  switches.clear();
  integers.clear();
  doubles.clear();
  strings.clear();
  unparsed.clear();
  //help_="Command line flags and arguments:\n\n";
  parse_errors_="";
  num_unparsed_=0;
}
void ArgumentParser::addHelp(const char* help){
  help_ += help;
  help_ += "\n";
}

void ArgumentParser::addSwitch(const char* token, int* dest, const char* description){
  switches[token]=dest;

  std::stringstream s;
  s.str("");
  s << "  " << std::left << std::setw(18) << token << "  ";
  if(description){ s << description << " "; }
  s << "\n";
  help_+=s.str();
}

void ArgumentParser::addInt(const char* token, int* dest, const char* description){
  integers[token]=dest;

  std::string tmp = token;
  tmp += " INT";

  std::stringstream s;
  s.str("");
  s << "  " << std::left << std::setw(18) << tmp << "  ";
  if(description){ s << description << " "; }
  s << "(default: " << *dest << ")\n";
  help_+=s.str();
}

void ArgumentParser::addDouble(const char* token, double* dest, const char* description){
  doubles[token]=dest;

  std::string tmp = token;
  tmp += " DOUBLE";

  std::stringstream s;
  s.str("");
  s << "  " << std::left << std::setw(18) << tmp << "  ";
  if(description){ s << description << " ";}
  s << "(default: " << *dest <<")\n";
  help_+=s.str();

}

void ArgumentParser::addString(const char* token, std::string* dest, const char* description){
  strings[token]=dest;

  std::string tmp = token;
  tmp += " STRING";

  std::stringstream s;
  s.str("");
  s << "  " << std::left << std::setw(18) << tmp << "  ";
  if(description){ s  << description << " "; }
  s << "(default: \"" << *dest << "\")\n";
  help_+=s.str();
}

void ArgumentParser::addUnparsed(std::string* dest, const char* description){
  unparsed.push_back(dest);


  std::stringstream t;
  t.str("");

  int last_digit = unparsed.size() % 10;
  int next_to_last_digit = (unparsed.size() % 100) - last_digit;
  t << "\"" << unparsed.size();
  if((last_digit>=1) && (last_digit<=3) && (next_to_last_digit != 1)){
    switch(last_digit){
    case 1:
      t << "'ST";
      break;
    case 2:
      t << "'ND";
      break;
    case 3:
      t << "'RD";
      break;
    }
  }else{
    t << "'TH";
  }
  t << " STRING\"";

  std::stringstream s;
  s.str("");
  s << "  " << std::left << std::setw(18) << t.str() << "  ";
  if(description) { s << description << " "; }
  s << "(default: \"" << *dest << "\")\n";
  help_+=s.str();
}

int ArgumentParser::parse(int argc, char* argv[]){

  //First, clear all switches (not allowed to have default values)
  for(std::map<std::string,int*>::iterator p 
	= switches.begin();
      p!= switches.end(); ++p)
    {
      *(p->second) = 0;
    } 

  for(int i=1; i<argc; i++){
    
    if((i+1)<argc){//if more then one argument left

      //first looking for string
      for(std::map<std::string,std::string*>::const_iterator p 
	    = strings.begin();
	  p!= strings.end(); ++p)
	{
	  if(0==p->first.compare(argv[i])){ //match
	    (*(p->second)) = argv[i+1];
	    goto swallow_extra;
	  }
	}
      
      //then looking for double
      for(std::map<std::string,double*>::const_iterator p 
	    = doubles.begin();
	  p!= doubles.end(); ++p)
	{
	  if(0==p->first.compare(argv[i])){ //match

	    (*(p->second)) = atof(argv[i+1]); //FIXME add fault detection?

	    ///@todo FIXME more like this, then checking s for success ?
	    //stringstream s; s.str(argv[i+1]); s >> *(p->second);

	    goto swallow_extra;
	  }
	}      
      
      //then looking for int
      for(std::map<std::string,int*>::const_iterator p 
	    = integers.begin();
	  p!= integers.end(); ++p)
	{
	  if(0==p->first.compare(argv[i])){ //match
	    (*(p->second)) = atoi(argv[i+1]); //FIXME add fault detection?
	    goto swallow_extra;
	  }
	} 
    }

    //then looking for switch
    for(std::map<std::string,int*>::const_iterator p 
	  = switches.begin();
	p!= switches.end(); ++p)
      {
	if(0==p->first.compare(argv[i])){ //match
	  (*(p->second)) = 1; //FIXME add fault detection?
	  goto swallow_normal;
	}
      } 
    
    //then assuming it to belong to unparsed
    if(((int)(unparsed.size()))>num_unparsed_){
      *unparsed[num_unparsed_++] = argv[i];
      goto swallow_normal;
    }

    
    parse_errors_ += "unrecognized argument \"";
    parse_errors_ += argv[i];
    parse_errors_ += "\"\n";

  swallow_normal:
    continue;
  swallow_extra:
    i++;
  }//for(int i=1;i<argc...
  return (parse_errors_.empty())?1:0;
}

std::string ArgumentParser::getErrors(){
  return parse_errors_;
}

std::string ArgumentParser::getHelp(){
  return help_;
}

int ArgumentParser::operator !(){
  return (parse_errors_.empty())?0:1;
}
