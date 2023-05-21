import unittest
import translate
from instruction import Instruction
from instruction import AInstruction
from instruction import CInstruction

class TestTranslate(unittest.TestCase):
	def test_a_instruction(self):
		instruction = AInstruction(17)
		self.assertEquals(translate.translate_instruction(instruction), '0000000000010001')

	def test_c_instruction(self):
		instruction = CInstruction('A-D', 'M', 'JNE')
		self.assertEquals(translate.translate_instruction(instruction), '1110000111001101')

if __name__ == '__main__':
	unittest.main()
