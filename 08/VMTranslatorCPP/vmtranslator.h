#include <string>

#include "codewriter.h"

namespace Translator {
  using std::string;

  class VMTranslator {
    public:
    VMTranslator(string filename);

    int64_t Run();

    private:
    // Handles the translation into the output of a single file. Opens the
    // file, passes the prefix to 'code_writer.set_file_name' and
    // does all neccessary other set up steps in order to start writing into
    // the output via the code_writer.
    int64_t handle_translation_for_file(
      string infile,
      string prefix,
      CodeWriter& code_writer);

    string input_filename_;
    string output_filename_;
  };
}
