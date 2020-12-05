# Two-Stage Instruction Decoder
**ERAU  
CEC 470  
Fall 2020**  
**Tyler Wise & Oleksandr Hendrik**

### 1. What opcode will blank memory initialized to 0x00 look like to the processor?

0x00 would be treated as a memory op to store the value of ACC into the address specified by the operand (i.e. the next 16 bits), which in this case would be memory location 0.

### 2. Of the 256 possible opcodes we can get from and 8-bit opcode, how many are not being used in our instruction set, i.e., how many instructions could we add for future expansions of our processor? 

With the current setup we utilize 2^7 + 2^4 + (2^3 - 1) = 151 possible opcodes. 2^7 comes from the 7 bits of the math ops, 2^4 comes from the 4 bits of the memory ops, and 2^3 comes from the 3 bits of the branch ops, minus 1 since there is no branch corresponding to 111.
This leaves us with 105 more opcodes that we could implement.

### 3. What would we need to add to our simulator to be able to include the following instructions: 
compare ACC with a constant - this would require another branching instruction, possibly occupying the 111 branching slot.  

PUSH to or PULL from the stack - this would require an additional set of opcodes and a register to store the location of the top of the stack.  

take the 2's complement of ACC - this would require another compound math op (which would be tough to fit into the existing math-op bitmask) that takes the value of ACC, performs an XOR operation on it, and stores the result either in ACC or in a specified register/memory location.

### 4. If executeInstruction() were divided into two parts, decode and execute, what additional global resources would be needed for your simulator?
We would require additional registers to store the decoded operation data such as the source and destination of the operation.
