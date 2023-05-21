import translate
from instruction import Instruction
from instruction import InstructionType
from instruction import AInstruction
from instruction import CInstruction
import argparse

# adds default values to the symbol table
def populate_symbol_table(symbol_table: dict):
	symbol_table['R0'] = 0
	symbol_table['R1'] = 1
	symbol_table['R2'] = 2
	symbol_table['R3'] = 3
	symbol_table['R4'] = 4
	symbol_table['R5'] = 5
	symbol_table['R6'] = 6
	symbol_table['R7'] = 7
	symbol_table['R8'] = 8
	symbol_table['R9'] = 9
	symbol_table['R10'] = 10
	symbol_table['R11'] = 11
	symbol_table['R12'] = 12
	symbol_table['R13'] = 13
	symbol_table['R14'] = 14
	symbol_table['R15'] = 15
	symbol_table['SCREEN'] = 16384
	symbol_table['KBD'] = 24576
	symbol_table['SP'] = 0
	symbol_table['LCL'] = 1
	symbol_table['ARG'] = 2
	symbol_table['THIS'] = 3
	symbol_table['THAT'] = 4

# assembles the given '.asm' in 'infile' and outputs it to 'outfile.hack'
def assemble(infile: str, outfile: str):
	with open(infile, 'r') as i:
		with open(outfile, 'w') as o:
			instructions = []
			symbol_table = {}
			populate_symbol_table(symbol_table)
			line_ctr = 0
			sym_ctr = 16
			# first pass - discover bookmarks
			while line := i.readline():
				nwln_iloc = line.find('\n')
				if nwln_iloc >= 0:
					line = line[:nwln_iloc]
				cmt_iloc = line.find('//')
				if cmt_iloc >= 0:
					line = line[:cmt_iloc]
	# eliminate whitespaces in the instruction
				line.replace(' ', '')
				line.replace('\t', '')
				line = ''.join([char for char in line if ord(char) > 32])
				if len(line) == 0:
					continue
	# create instruction based on if it is a- or c- instruction
				paren_iloc = line.find('(')
				if paren_iloc == 0:
					sym = line[1:-1]
					symbol_table[sym] = line_ctr
					continue
				a_iloc = line.find('@')
				if a_iloc >= 0:
					addr_str = line[a_iloc+1:]
					if (addr_str.isdigit()):
						addr = int(addr_str)
					else:
						addr = addr_str
					instruction = AInstruction(addr)
				# else, it is a c-instruction
				else:
					eq_iloc = line.find('=')
					if eq_iloc>0:
						dst = line[:eq_iloc]
						sem_iloc = line.find(';')
						if sem_iloc>0:
							cmp = line[eq_iloc+1:sem_iloc]
							jmp = line[sem_iloc+1:]
						else:
							cmp = line[eq_iloc+1:]
							jmp = None
					else:
						dst = None
						sem_iloc = line.find(';')
						cmp = line[eq_iloc+1:sem_iloc]
						jmp = line[sem_iloc+1:]
					instruction = CInstruction(cmp, dst, jmp)
				instructions.append(instruction)
				line_ctr+=1
			# second pass
			for instruction in instructions:
				if instruction.get_type() == InstructionType.A:
					if instruction.has_symbols():
						if instruction.addr in symbol_table:
							instruction.addr = symbol_table[instruction.addr]
						else:
							symbol_table[instruction.addr] = sym_ctr
							instruction.addr = sym_ctr
							sym_ctr += 1

				translated = translate.translate_instruction(instruction)
				o.write(translated + '\n')
			
				
parser = argparse.ArgumentParser()
parser.add_argument('infile', help='the input file (*.asm)')
parser.add_argument('outfile', help='the output file (*.hack)')
args = parser.parse_args()
infile = args.infile
outfile = args.outfile
if __name__ == '__main__':
	assemble(infile, outfile)	
