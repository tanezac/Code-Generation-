# Code-Generation-
• Generate assembly code for an input program. 
• Program must compile and run on clark.rnet.missouri.edu 
Grammar Semantics 
• Delimiters: 
o { } . 
o Place Home 
• Operators: 
o << is the same as standard less than symbol (<) 
o <- is the same as greater than or equal to (>=)  
o / is a decrement operator. It reduces the value by one  
 Number may be immediate or stored at location indicated by identifier 
o  + is the same as standard plus sign 
o  % means divide the first number by the second number, truncating the remainder  
 e.g. 11 % 5 = 2 
o & means multiply the two numbers together 
• Note that operators have no precedence and are applied from left to right  
• ‘Name identifier’ allocates memory for given identifier and initializes its value to zero 
• ‘Assign identifier M’ assigns the value of M to the given identifier 
• ‘{ If Identifier T W D }’  
o Means to do D if and only if ‘Identifier T W’ is true  
•  ‘{Do Again D T W }’ 
o Means to repeat D until ‘T W zero’ is false 
 If T is <<, repeat D until W is zero or more 
 If T is <-, repeat D until W is less than zero 
• ‘Spot number’ means to load the immediate number into the ACCumulator 
• ‘Spot identifier’ means to read in an integer from the user and store the value for the given new 
identifier (this operations allocated memory for the identifier) 
• ‘Show number’ means to write out the number to the monitor 
• ‘Show identifier’ means to write out the value of the given identifier to the monitor 
• ‘Move identifier’ means to load the given identifier’s value into the ACCumulator 
• ‘Flip identifier’ means to multiply the value for the given identifier by -1 and store the value  
• ‘Here number There’ means print the given number to the screen the given number of times (e.g. 
‘Here 3 There’ will print the number 3 to the screen 3 times) 
Data 
 
• All data is 2-byte signed integers 
• Assume no overflow in operations 
 
Target Language 
 
• VM ACCumulator assembly language 
• Executable is available on Canvas (virtMach) or you can use the online implementation: 
https://comp.umsl.edu/assembler/interpreter  
• Description provided in VM_Architecture.pdf and VM_Language.pdf on Canvas 
 
Requirements 
• Invocation:  
> genCode [file] 
 
• Call code generation function on the tree after calling static semantics function in main. Note that 
the  container must be created in static semantics but is also accessed in code generation 
• The nodes of the parse tree are equivalent to the program in left to right traversal  
o Therefore, perform left to right traversal to generate the code 
• When visiting code-generating nodes 
o temporary variables may be needed - generate global pool variables 
o if value is produced, always leave the result in the ACCumulator 
o each node-kind generates the same code 
 regardless of parents 
 may be one of multiple cases 
• At the end of the traversal, print STOP to target, followed by global variables plus temporaries 
in  storage allocation 
 
Suggestions 
• In order to avoid naming temporary variables with an identifier that is given in an incoming program, 
use an upper-case letter for the first character of the temporary variable name (e.g. T1, T2, etc.) 
• Your assembly programs can be run using either the executable, virtMach, or the online 
implementation at https://comp.umsl.edu/assembler/interpreter  
o If you use the executable, upload ‘virtMach’ into the desired directory on clark and make it 
executable by typing ‘chmod 700 virtMach’. You can then run programs by typing ‘virtMach 
filename.asm’ 
o If you use the online browser, you can upload your .asm files or copy and paste the text in the 
provided window. You may need to click on ‘Reset Runtime’ in the top menu bar between runs. 
Test Files 
Example 1 input: 
 Name prog1 
 Spot prog2 
 Place 
 Name id1   
 Home 
 Show prog2 
 
Example 1 .asm file produced: 
 LOAD 0 
 STORE prog1 
 READ prog2 
 LOAD 0 
 STORE id1 
 WRITE prog2 
 STOP 
 prog1 0 
 prog2 0 
 id1 0 
  
Example 2 input: 
 Name p1 
 Spot p2 
 Place 
 Name p3 
 . 
 Here 4 There 
 . 
  Show p2  
 Home 
 Show p3 
 
Example 2 .asm file produced: 
 LOAD 0 
 STORE p1 
 READ p2 
 LOAD 0 
 STORE p3 
 LOAD 4 
 STORE T1 
 BRZNEG Done 
 Repeat: WRITE T1 
 SUB 1 
 BRPOS Repeat 
 Done: NOOP 
 WRITE p2 
 WRITE p3 
 STOP 
 p1 0 
 p2 0 
 p3 0 
 T1 0 
