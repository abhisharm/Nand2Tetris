#include "codewriter.h"

#include <stdexcept>

namespace Translator {
  using std::ostream;
  using std::string;
  using std::cout;
  using std::ofstream;

  CodeWriter::CodeWriter(ostream* stream)
    : o_stream_(stream)
    {}

  void CodeWriter::set_file_name(string filename) {
    filename_ = filename;
  }

  void CodeWriter::write_push_pop(
    CommandType command_type,
    string segment,
    int index) {
      string output;
      string command_description;
      switch (command_type) {
        case CommandType::C_PUSH:
        {
          if (segment == "local" || segment == "argument" || segment == "static"
              || segment == "constant" || segment == "this" || segment == "that"
              || segment == "pointer" || segment == "temp") {
            output = HandlePushStatement(segment, index);
            command_description = "// push " + segment + " "
              + std::to_string(index);
            break;
          } else {
            string exception = "Incompatible segment: " + segment;
            exception += " passed to 'write_push_pop'\n";
            throw std::invalid_argument(exception); 
          }
        }
        case CommandType::C_POP:
        {
          if (segment == "local" || segment == "argument" || segment == "static"
              || segment == "constant" || segment == "this" || segment == "that"
              || segment == "pointer" || segment == "temp") {
            output = HandlePopStatement(segment, index);
            command_description = "// pop " + segment + " "
              + std::to_string(index);
            break;
          } else {
            string exception = "Incompatible segment: " + segment;
            exception += " passed to 'write_push_pop'\n";
            throw std::invalid_argument(exception); 
          }
        }
        default:
          string exception = "Incompatible command " + command_type;
          exception += " passed to 'write_push_pop'\n";
          throw std::invalid_argument(exception); 
      }

      output += "\n";
      command_description += "\n";
      o_stream_->write(command_description.c_str(), command_description.size());
      o_stream_->write(output.c_str(), output.size());
  }

  void CodeWriter::write_arithmetic(string command) {
    string output;
    string command_description = "// " + command;
    if (command == "add") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + AddValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "sub") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + SubtractValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "neg") {
      output += HandlePopStatement("R", 13);
      output += "\n" + NegValueInRegisterAndStore("R13");
      output += "\n" + HandlePushStatement("R", 13);
    } else if (command == "eq") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + EqValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "gt") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + GtValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "lt") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + LtValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "and") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + AndValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "or") {
      output += HandlePopStatement("R", 13);
      output += "\n" + HandlePopStatement("R", 14);
      output += "\n" + OrValuesInRegistersInR1("R14", "R13");
      output += "\n" + HandlePushStatement("R", 14);
    } else if (command == "not") {
      output += HandlePopStatement("R", 13);
      output += "\n" + NotValueInRegisterAndStore("R13");
      output += "\n" + HandlePushStatement("R", 13);
    } else {
      string exception = "Incompatible command " + command;
      exception += " passed to 'write_arithmetic'\n";
      throw std::invalid_argument(exception);
    }

    output += "\n";
    command_description += "\n";
    o_stream_->write(command_description.c_str(), command_description.size());
    o_stream_->write(output.c_str(), output.size());
  }

  void CodeWriter::add_infinite_loop() {
    string output = AddInfiniteLoop();
    output += "\n";
    o_stream_->write(output.c_str(), output.size());
  }

  string CodeWriter::HandlePopStatement(string segment, int index) {
    string output;
    if (segment == "local") {
      output += StoreAddrOfSegmentWithOffsetInRegister("LCL", index, "R15");
      output += "\n" + StoreTopOfStackIntoD();
      output += "\n" + StoreDInAddressPointedToByVariable("R15");
      output += "\n" + DecrementSp();
    } else if (segment == "argument") {
      output += StoreAddrOfSegmentWithOffsetInRegister("ARG", index, "R15");
      output += "\n" + StoreTopOfStackIntoD();
      output += "\n" + StoreDInAddressPointedToByVariable("R15");
      output += "\n" + DecrementSp();
    } else if (segment == "static") {
      output += StoreTopOfStackIntoD();
      string variable_name = filename_ + '.' + std::to_string(index);
      output += "\n" + StoreDInVariable(variable_name);
      output += "\n" + DecrementSp();
    } else if (segment == "this") {
      output += StoreAddrOfSegmentWithOffsetInRegister("THIS", index, "R15");
      output += "\n" + StoreTopOfStackIntoD();
      output += "\n" + StoreDInAddressPointedToByVariable("R15");
      output += "\n" + DecrementSp();
    } else if (segment == "that") {
      output += StoreAddrOfSegmentWithOffsetInRegister("THAT", index, "R15");
      output += "\n" + StoreTopOfStackIntoD();
      output += "\n" + StoreDInAddressPointedToByVariable("R15");
      output += "\n" + DecrementSp();
    } else if (segment == "pointer") {
      if (index == 0) {
        output += StoreTopOfStackIntoD();
        output += "\n" + StoreDInVariable("THIS");
        output += "\n" + DecrementSp();
      } else if (index == 1) {
        output += StoreTopOfStackIntoD();
        output += "\n" + StoreDInVariable("THAT");
        output += "\n" + DecrementSp();
      } else {
        string exception = "Incompatible index: " + index;
        exception += " passed to 'HandlePopStatement' with 'pointer' as segment\n";
        throw std::invalid_argument(exception); 
      } 
    } else if (segment == "temp") {
      // temp is a special segment and requires specil code.
      int index_to_write_to = 5 + index;
      output += StoreTopOfStackIntoD();
      output += "\n" + StoreDInVariable(std::to_string(index_to_write_to));
      output += "\n" + DecrementSp();
    } else if (segment == "R") {
      string pop_loc = "R" + std::to_string(index);
      output += StoreTopOfStackIntoD();
      output += "\n" + StoreDInVariable(pop_loc);
      output += "\n" + DecrementSp();
    } else {
      string exception = "Incompatible segment-id: " + segment;
      exception += " passed to 'HandlePopStatement'\n";
      throw std::invalid_argument(exception); 
    }

    return output;
  }

  string CodeWriter::HandlePushStatement(string segment, int index) {
    string output;
    if (segment == "local") {
      output += SetDToValueInSegmentAddrWithOffset("LCL", index);
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "argument") {
      output += SetDToValueInSegmentAddrWithOffset("ARG", index);
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "static") {
      string variable_name = filename_ + '.' + std::to_string(index);
      output += SetDToVariable(variable_name);
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "constant") {
      string index_str = std::to_string(index);
      output += SetDToVariableAddress(index_str);
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "this") {
      output += SetDToValueInSegmentAddrWithOffset("THIS", index);
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "that") {
      output += SetDToValueInSegmentAddrWithOffset("THAT", index);
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "pointer") {
      if (index == 0) {
        output += SetDToVariable("THIS");
        output += "\n" + SetSpToDAndIncr();
      } else if (index == 1) {
        output += SetDToVariable("THAT");
        output += "\n" + SetSpToDAndIncr();
      } else {
        string exception = "Incompatible index: " + index;
        exception += " passed to 'HandlePushStatement' with 'pointer' as segment\n";
        throw std::invalid_argument(exception); 
      } 
    } else if (segment == "temp") {
      int loc_to_pop_to = 5 + index;
      string pop_loc_as_string = std::to_string(loc_to_pop_to);
      output += SetDToVariable(pop_loc_as_string );
      output += "\n" + SetSpToDAndIncr();
    } else if (segment == "R") {
      string pop_loc = "R" + std::to_string(index);
      output += SetDToVariable(pop_loc); 
      output += "\n" + SetSpToDAndIncr();
    } else {
      string exception = "Incompatible segment-id: " + segment 
        + " passed to 'HandlePushStatement'\n";
      throw std::invalid_argument(exception); 
    }
  
    return output;

  }

  //TODO: add newlines before every plus in the handle functions

  string CodeWriter::StoreTopOfStackIntoD() {
    string output;
    output += "@SP";
    output += "\nA=M-1";
    output += "\nD=M";
    return output;
  }

  string CodeWriter::StoreAddrOfSegmentWithOffsetInRegister(
    string segment,
    int index,
    string reg) {

    string output;
    output += "@" + segment;
    output += "\nD=M";
    output += "\n@" + std::to_string(index);
    output += "\nD=D+A";
    output += "\n@" + reg;
    output += "\nM=D";
    return output;
  }

  string CodeWriter::StoreDInAddressPointedToByVariable(string reg) {
    string output;
    output += "@" + reg;
    output += "\nA=M";
    output += "\nM=D";
    return output;
  }

  string CodeWriter::StoreDInVariable(string variable) {
    string output;
    output += "@" + variable;
    output += "\nM=D";
    return output;
  }

  string CodeWriter::DecrementSp() {
    string output;
    output += "@SP";
    output += "\nM=M-1";
    return output;
  }

  string CodeWriter::SetDToVariable(string variable) {
    string output;
    output += "@"+variable;
    output += "\nD=M";
    return output;
  }

  string CodeWriter::SetDToVariableAddress(string variable) {
    string output;
    output += "@"+variable;
    output += "\nD=A";
    return output;
  }

  string CodeWriter::SetDToValueInSegmentAddrWithOffset(
    string segment,
    int index) {

    string output;
    output += "@"+segment;
    output += "\nD=M";
    output += "\n@"+std::to_string(index);
    output += "\nA=A+D";
    output += "\nD=M";
    return output;
  }

  string CodeWriter::SetSpToDAndIncr() {
    string output;
    output += "@SP";
    output += "\nA=M";
    output += "\nM=D";
    output += "\n@SP";
    output += "\nM=M+1";
    return output;
  }

  string CodeWriter::AddValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SetDToVariable(r1);
    output += std::string("\n") + "@" + r2;
    output += std::string("\n") + "D=D+M";
    output += std::string("\n") + StoreDInVariable(r1);
    return output;
  }

  string CodeWriter::SubtractValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SetDToVariable(r1);
    output += std::string("\n") + "@" + r2;
    output += std::string("\n") + "D=D-M";
    output += std::string("\n") + StoreDInVariable(r1);
    return output;
  }

  string CodeWriter::NegValueInRegisterAndStore(string r1) {
    string output;
    output += "@" + r1;
    output += std::string("\n") + "D=-M";
    output += "\n" + StoreDInVariable(r1);
    return output;
  }

  string CodeWriter::EqValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SubtractValuesInRegistersInR1(r1, r2);
    output += "\n" + SetBooleanValueInRegisterBasedOnCondition(r1, "JEQ");
    return output;
  }

  string CodeWriter::GtValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SubtractValuesInRegistersInR1(r1, r2);
    output += "\n" + SetBooleanValueInRegisterBasedOnCondition(r1, "JGT");
    return output;
  }

  string CodeWriter::LtValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SubtractValuesInRegistersInR1(r1, r2);
    output += "\n" + SetBooleanValueInRegisterBasedOnCondition(r1, "JLT");
    return output;
  }

  string CodeWriter::AndValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SetDToVariable(r1);
    output += std::string("\n") + "@" + r2;
    output += std::string("\n") + "D=D&M";
    output += "\n" + StoreDInVariable(r1);
    return output;
  }

  string CodeWriter::OrValuesInRegistersInR1(string r1, string r2) {
    string output;
    output += SetDToVariable(r1);
    output += std::string("\n") + "@" + r2;
    output += std::string("\n") + "D=D|M";
    output += "\n" + StoreDInVariable(r1);
    return output;
  }

  string CodeWriter::NotValueInRegisterAndStore(string r1) {
    string output;
    output += "@" + r1;
    output += std::string("\n") + "D=!M";
    output += "\n" + StoreDInVariable(r1);
    return output;
  }

  string CodeWriter::SetBooleanValueInRegisterBasedOnCondition(
    string r1,
    string condition) {

    static int iteration = 1;
    string output;
    output += "@" + r1;
    output += std::string("\n") + "D=M";
    output += std::string("\n") + std::string("@")
      + "TRUE_" + std::to_string(iteration);
    output += std::string("\n") + "D;" + condition;
    output += std::string("\n") + "@" + r1; // False condition
    output += std::string("\n") + "M=0";
    output += std::string("\n") + std::string("@")
      + std::string("END_") + std::to_string(iteration);
    output += std::string("\n") + "0;JMP";
    output += std::string("\n") + "(TRUE_" + std::to_string(iteration) + ")"; // True cond
    output += std::string("\n") + "@" + r1;
    output += std::string("\n") + "M=-1";
    output += std::string("\n") + "(END_" + std::to_string(iteration) + ")"; // end
    
    iteration += 1;
    return output;
  }

  string CodeWriter::AddInfiniteLoop() {
    string loop_name = filename_ + ".END";
    string output;
    output += "// end of program";
    output += "\n(" + loop_name + ")";
    output += "\n@" + loop_name;
    output += "\n0;JMP";
    return output;
  }

  void CodeWriter::close() {
    ofstream* of_stream = static_cast<ofstream*>(o_stream_);
    of_stream->close();
  }
}

