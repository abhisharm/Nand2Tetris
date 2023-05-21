// Jack Analyzer class

#include <string>
#include <vector>

namespace AbhishekJackCompiler {

using std::string;
using std::vector;

class JackCompiler {
  public:
    enum ArgumentType {
      FILENAME = 0,
      DIRNAME = 1,
      UNMATCHED = 2,
    };

    // Struct to describe an argument (either directory or filename) passed to
    // the 'JackAnalyzer' class.
    struct ArgumentDescriptor {
      ArgumentType argument_type;

      // The path to the directory containing the file or directory name itself
      // (if directory was passed). Always ends in '/'
      string path;

      // The name of the file passed (not including the '.jack' extension).
      // Only present if the argument_type is 'FILENAME'.
      string name;
    };

    // Constructor for JackAnalyzer. Accepts a list of string 'args' which each
    // represents a directory path or a list of paths to directories containing
    // jack files.
    JackCompiler(vector<string> args);

    // Parse and Compile the file in the descriptor, writing the compiled output for
    // 'Xxx.jack' to 'Xxx.xml'. Compiled here means the parsed file in XML format..
    void ConstructOutputForFile(ArgumentDescriptor argument_descriptor);

    // Parse and Compile the file in the descriptor, writing the compiled output
    // for 'Xxx.jack' to 'XxxT.xml'. Compiled here means simply a list of
    // identifier tokens in the order that they appeared in the file.
    void ConstructSimplifiedOutputForFile(
      ArgumentDescriptor argument_descriptor);

    // Parse and Compile all files in the argument descriptor, calling
    // 'ConstructOutputForFile' for each one
    void ConstructOutputForDirectory(ArgumentDescriptor argument_descriptor);

    // Construct output for every argument provided in 'args'
    void ConstructOutputForAllArgs();

  private:
    vector<string> args_;

    ArgumentDescriptor GetArgumentType(string arg);
};

} // namespace JackAnalyzer
