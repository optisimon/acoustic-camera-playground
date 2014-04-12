#ifndef ARGUMENTPARSER_HH
#define ARGUMENTPARSER_HH

#include <string>
#include <map>
#include <vector>

/** Class that facilitates parsing of command line arguments 
    passed to main. Example usage:
    @code
#include "ArgumentParser.hh"
#include <iostream.h>

using namespace std;

int main(int argc, char* argv[]){

  int on = 0;
  int off = 0;
  int one_int = 0;
  double one_double = 3.2;
  string one_string = "";
  string unparsed_1 = "";
  string unparsed_2 = "some default string";

  ArgumentParser p;
  p.addHelp("This is some test program for argument parsing\n");
  p.addHelp("Try it with some of these binary switches:");
  p.addSwitch("-on",&on,"To turn something on...");
  p.addSwitch("-off",&off,"To turn something off...");
  p.addHelp("\nTry some of these other switches, expecting numbers and strings:");
  p.addInt("-one_int",&one_int,"Stored into local variable one_int");
  p.addDouble("-one_double", &one_double,"This is some unimportant number" );
  p.addString("-one_string", &one_string,"comment...");
  p.addHelp("\nAnd its also possible to catch arguments which are not set as special tokens:");
  p.addUnparsed(&unparsed_1,"only this has comment, not the other");
  p.addUnparsed(&unparsed_2);

  if(argc==1){ 
    cout << p.getHelp() << endl;
    exit(0);
  }
  p.parse(argc,argv);


  cout << "on=" << on 
       << "\noff=" << off 
       << "\none_int=" << one_int
       << "\none_double=" << one_double 
       << "\none_string=" << one_string 
       << "\nunparsed_1=" << unparsed_1 
       << "\nunparsed_2=" << unparsed_2 
       << endl
       << "------------------------" 
       << endl
       << "parse errors: \n"
       << p.getErrors() 
       << endl;
}
    @endcode

    The code above (which checks for argc==1) would generate this 
    message when no argument is supplied:
    <pre>
This is some test program for argument parsing

Try it with some of these binary switches:
  -on                 To turn something on...
  -off                To turn something off...

Try some of these other switches, expecting numbers and strings:
  -one_int INT        Stored into local variable one_int (default: 0)
  -one_double DOUBLE  This is some unimportant number (default: 3.2)
  -one_string STRING  comment... (default: "")

And its also possible to catch arguments which are not set as special tokens:
  "1'ST STRING"       only this has comment, not the other (default: "")
  "2'ND STRING"       (default: "some default string")
</pre>

    @author Simon Gustafsson
*/
class ArgumentParser{
private:
  std::map<std::string,int*> switches;
  std::map<std::string,int*> integers;
  std::map<std::string,double*> doubles;
  std::map<std::string,std::string*> strings;
  std::vector<std::string*> unparsed;

  int num_unparsed_;
  std::string parse_errors_;
  std::string help_;
public:

  /** Setup */  
  ArgumentParser();

  /** Discard all tokens prevoiusly passed to this class. */
  void clearAllTokens();

  /** Add extra information to be returned by getHelp()
      @param help extra help text. Should not end in a newline,
      except if one wants blank lines in the help display.*/
  void addHelp(const char* help);

  /** Add detection of specified token to parse(). 
      @param token token in arguments to watch for
      @param dest set to 1 if token detected by parse(), 
      othervise set to 0. 
      @param description additional description of this switch*/
  void addSwitch(const char* token, int* dest, const char* description=0);


  /** Add detection of specified token followed by integer to parse(). 
      @param token token in arguments to watch for
      @param dest set to the integer following the token by parse(), 
      not modified if token not present. 
      @param description additional description of this int*/
  void addInt(const char* token, int* dest, const char* description=0);

  /** Add detection of specified token followed by a double to parse(). 
      @param token token in arguments to watch for
      @param dest set to the double following the token by parse(), 
      not modified if token not present. 
      @param description additional description of this double */
  void addDouble(const char* token, double* dest, const char* description=0);

  /** Add detection of specified token followed by a string to parse(). 
      @param token token in arguments to watch for
      @param dest set to the string following the token by parse(), 
      not modified if token not present. 
      @param description additional description of this string */
  void addString(const char* token, std::string* dest, const char* description=0);

  /** Arguments not catched by any token can be catched 
      by using this function. Calling this functions more then once 
      with different destination strings makes it possible to get 
      the first unmatched argument in one string, the second unmatched
      in another string, and so on.
      @param dest will be filled with the first string not matched 
      by any token finders (including this one) when parse() is called.
      @param description additional description of this unparsed value */
  void addUnparsed(std::string* dest, const char* description=0);

  /** Start parsing for tokens and unmatched strings, and modify
      destination values for tokens passed.
      @returns 1 on complete success, 0 when there was any errors*/
  int parse(int argc, char* argv[]);

  /** Show parse errors. Currently only shows unrecognized arguments,
      which was not captured as an unparsed, because not enough
      unparsed elements was available */
  std::string getErrors();

  /** Return a string describing all flags which can be parsed.
      If description not supplied when adding tokens, only available
      tokens and their types are returned. */
  std::string getHelp();

  /** Determine if there was any error.
      @returns 1 if there was some error, otherwise 0*/
  int operator !();
};


#endif
