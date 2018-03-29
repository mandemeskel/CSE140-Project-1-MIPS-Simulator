# CSE140-Project-1-MIPS-Simulator
This was a fun C project for my Computer Architecture class - which I'm not fond off but I love C. I had a great deal of joy creating all the tests for this project and watching it run machine code.

In this project, you will create an instruction interpreter for a subset of MIPS code. Starting with the assembled instructions in memory, you will fetch, disassemble, decode, and execute MIPS machine instructions, simulating each stage in the computation. You're creating what is effectively a miniature version of MARS! There is one important difference, though—MARS takes in assembly language source Hiles, not .dump Hiles, so it contains an assembler, too.
You may work with a partner for this project. Each person will turn in the following for the project on CatCourses:
- Completed computer.c
- Testing.txt/doc which outlines your testing strategy
- All the *.s and *.dump Hiles you used to test your project in test.tgz
- Name of your partner in the submission box.


# Implement
- Decode - Given an instruction, Hill out the corresponding information in a DecodedInstr struct. Perform register reads and Hill the RegVals struct. The addr_or_immed Hield of the IRegs struct should contain the properly extended version of the 16 bits of the immediate Hield.
- Execute - Perform any ALU computation associated with the instruction, and return the value. For a lw instruction, for example, this would involve computing the base + the offset address. For this project, branch comparisons also occur in this stage.
- Mem - Perform any memory reads or writes associated with the instruction. Note that as in the Fetch function, we map the MIPS address 0x00400000 to index 0 in our internal memory array, MIPS address 0x00400004 to index 1, and so forth. If an instruction accesses an invalid memory address (outside of our data memory range, 0x00401000 - 0x00403fff, or not word aligned for lw or sw), your code must print the message, "Memory Access Exception at [PC val]: address [address]", where [PC val] is the current PC, and [address] is the offending address, both printed in hex (with leading 0x). Then you must call exit(0).
- RegWrite - Perform any register writes needed.
- UpdatePC - Perform the PC update associated with the current instruction. For most instructions, this corresponds with an increment of 4 (which we have already added).
- PrintInstruction - Print the current instruction and its operands in text. Here are the details on the output format and sample.output Hile contains the expected output for sample.dump:


# Setup and Testing
In project root, run the following commands.
1) Build simulator: `make sim`
2) Run sample test: `./sim tests/sample1.dump`
```
Executing instruction at 00400000: 24100005
addiu	$0, $16, 5
New pc = 00400004
Updated r16 to 00000005
No memory location was updated.
Executing instruction at 00400004: 24110006
addiu	$0, $17, 6
New pc = 00400008
Updated r17 to 00000006
No memory location was updated.
...OMMITTED FOR BREVITY...
Executing instruction at 00400024: 8d280000
lw	$9, 0($8)
New pc = 00400028
Updated r08 to 00000003
Updated memory at address 00401000 to 00000000
Executing instruction at 00400028: 20100000
```
3) Run sum test: `./sim tests/sum.dump`
```
Executing instruction at 00400000: 00408821
addu	$17, $0, $2
New pc = 00400004
Updated r17 to 00000000
No memory location was updated.
Executing instruction at 00400004: 02004021
addu	$8, $0, $16
New pc = 00400008
Updated r08 to 00000000
No memory location was updated.
Executing instruction at 00400008: 02204821
addu	$9, $0, $17
New pc = 0040000c
Updated r09 to 00000000
No memory location was updated.
Executing instruction at 0040000c: 20510005
```


# Unit Tests
In project root, run the following commands.
#### RegWrite()
1) `make testRegWrite`
2) `./testRegWrite`
#### Mem()
1) `make testMem`
2) `./testMem`
#### UpdatePC()
1) `make testUpdatePC`
2) `./testUpdatePC`
#### Execute()
1) `make testExecute`
2) `./testExecute`
#### Decode()
1) `make testDecode`
2) `./testDecode`


