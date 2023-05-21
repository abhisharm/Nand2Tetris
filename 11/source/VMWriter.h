// VMWriter class definition

#include <iostream>
#include <string>

namespace AbhishekJackCompiler {

using std::ostream;
using std::string;

class VMWriter {
 public:
  enum Segment {
    CONSTANT,
    ARGUMENT,
    LOCAL,
    STATIC,
    THIS,
    THAT,
    POINTER,
    TEMP,
  };

  enum Command {
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT,
  };

  VMWriter(std::ostream* o_stream);

  void writePush(Segment segment, int index);

  void writePop(Segment segment, int index);

  void writeArithmetic(Command command);

  void writeLabel(string label);

  void writeGoto(string label);

  void writeIf(string label);

  void writeCall(string name, int nArgs);

  void writeFunction(string name, int nVars);

  void writeReturn();

  void close();
 private:
  ostream* o_stream_;
};

}
