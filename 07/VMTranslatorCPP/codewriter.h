#include <iostream>
#include <fstream>
#include <string>

#include "parser.h"

namespace Translator {
  using std::ostream;
  using std::string;

  class CodeWriter {
    public:
    CodeWriter(ostream* stream);

    // Informs that the translation of a new VM file has started. Sets the
    // current filename.
    void set_file_name(string filename);

    // Compiles an arithmetic command to assembly. Does not add a newline at
    // the end of the command.
    void write_arithmetic(string command);

    // Compiles a 'push' or 'pop' command to assembly. Does not add a newline
    // at the end of the command.
    void write_push_pop(CommandType command_type, string segment, int index);

    // Adds an infinite loop at the end of the program.
    void add_infinite_loop();

    // Closes the input ofstream (requires that the ostream passed was an
    // ofstream). Performs a static_cast of the input ostream to an ofstream
    void close();

    private:
    // This section contains many private functions that are useful in doing
    // the repetitive operations required to compile VM code. Generally, the
    // names are fairly self-explanatory. Also, it should be noted that the
    // commands can only be expected to do what is described by the function-
    // names and no additional assumptions e.g. about the values of given
    // registers before, during or after completion can be made. In addition,
    // all commands will add appropriate newlines between commands. However,
    // they generally will not add newlines at the beginning of or at the end
    // of a given command.

    // Handle (i.e., compile) a single pop statement. Note that this stores
    // data in R15, so data in R15 will be overwritten during execution of
    // these statements. If the segment passed in is equal to "R" then the
    // value will be popped to the translator index passed in (either 13 or
    // 14 - 15 is unavailable since it will be used by the function).
    string HandlePopStatement(string segment, int index);    

    // Takes the top value of the stack and stores into D.
    string StoreTopOfStackIntoD();

    // Stores the Address of the given segment with an offset into the register
    // provided (R13, R14, or R15).
    string StoreAddrOfSegmentWithOffsetInRegister(string segment, int offset,
      string reg);

    // Stores the value in D into the RAM address contained in the register
    // provided.
    string StoreDInAddressPointedToByVariable(string reg);

    // Stores the value in D in the given variable.
    string StoreDInVariable(string variable);

    // Decrement the SP (stack-pointer) by 1.
    string DecrementSp();

    // Handle (compile) a single push statement. Does not store any data in
    // additional registers.
    string HandlePushStatement(string segment, int index);

    // Set D to the value in the given variable (can be static variable, THIS,
    // THAT, SP, or any other variable)
    string SetDToVariable(string variable);

    // Set D to the address represented by a variable. Intended for use in the
    // 'constant' commands.
    string SetDToVariableAddress(string variable);

    // Set D to the value pointed to by base-address represented segment with offset
    string SetDToValueInSegmentAddrWithOffset(string segment, int index);

    // Set the stack-pointer to D and increment the stack-pointer by 1.
    string SetSpToDAndIncr();

    // Add the variables in r1 and r2 and store the result in r1.
    string AddValuesInRegistersInR1(string r1, string r2);

    // Subtract the variable in r2 from the variable in r1 and store in r1.
    string SubtractValuesInRegistersInR1(string r1, string r2);

    // Negate the Value in r1 and store it there.
    string NegValueInRegisterAndStore(string r1);

    // Check equality of int values in r1 and r2 and store voolean in r1
    string EqValuesInRegistersInR1(string r1, string r2);

    // Check whether value in r1 is greater than r2 and store in r1
    string GtValuesInRegistersInR1(string r1, string r2);

    // Check whether value in r1 is less than r2 and store in r1
    string LtValuesInRegistersInR1(string r1, string r2);

    // Perform boolean and of values in r1 and r2 and store in r1
    string AndValuesInRegistersInR1(string r1, string r2);

    // Perform boolean or of values in r1 and r2 and store in r1
    string OrValuesInRegistersInR1(string r1, string r2);

    // Perform not of boolean value in r1 and store in r1
    string NotValueInRegisterAndStore(string r1);

    // Set the value in r1 based on the value of r1, as evaluated by condition.
    string SetBooleanValueInRegisterBasedOnCondition(string r1, string condition);

    // Add an infinite loop at the end of the program
    string AddInfiniteLoop();

    // Private Variables
    ostream* o_stream_;
    string filename_;
  };

}
