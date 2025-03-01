// Behavioral model for float addition
// CSE141L Fall 2024    NO ROUND   NO SUB
// Dummy DUT for float+float
module fltflt0(
  input        clk, reset,           // Clock and reset signals
               start,                // Start signal
  output logic done                  // Done signal
);

  // Internal logic signals
  logic       nil1,                  // Zero detect addend 1
              nil2,                  // Zero detect addend 2
              nil3,                  // Zero detect sum
              guard,                 // Needed for subtraction
              round,                 // Round flag
              sticky;                // Sticky flag
  
  logic[10:0] mant1,                 // Mantissa of addend 1
              mant2;                 // Mantissa of addend 2 (with room for hidden bit)
  
  logic[11:0] mant3;                 // Mantissa of sum, including overflow
  
  logic[5:0] exp1,                   // Exponent of addend 1
             exp2,                   // Exponent of addend 2
             exp3;                   // Exponent of sum
  
  logic       sign1,                 // Sign of addend 1
              sign2,                 // Sign of addend 2
              sign3;                 // Sign of sum (or difference)

  // Data memory wires
  logic       CLK;                   // Clock signal for data memory
  logic[7:0]  DataAddress;           // Data memory address
  logic       ReadMem;               // Read memory flag
  logic       WriteMem;              // Write memory flag
  logic[7:0]  DataIn;                // Data input to memory
  logic[7:0]  DataOut;               // Data output from memory
  int         pgm;                   // Program counter

  // Data memory instance
  data_mem data_mem1(.*);

  // Main always block
  always @(posedge clk) begin : main
    if (reset) 
      pgm++;  // Reset logic for program counter
    else if (start) begin
      // Reset internal signals for a new operation
      guard  = 1'b0;
      round  = 1'b0;
      sticky = 1'b0;
      
      // Load operands from data memory
      sign1  = data_mem1.mem_core[128][7];
      sign2  = data_mem1.mem_core[130][7];
      exp1   = data_mem1.mem_core[128][6:2];
      exp2   = data_mem1.mem_core[130][6:2];
      nil1   = !data_mem1.mem_core[128][6:2];  // Zero exponent trap
      nil2   = !data_mem1.mem_core[130][6:2];  // Zero exponent trap
      mant1  = {data_mem1.mem_core[128][1:0], data_mem1.mem_core[129]};
      mant2  = {data_mem1.mem_core[130][1:0], data_mem1.mem_core[131]};
      
      done   = 1'b0;  // Clear done flag
    end else begin : nonreset
      exp3 = exp1;  // Default to exp1; override if exp2 > exp1
      
      if (sign1 == sign2) begin : netadd  // Perform addition if signs are the same
        sign3 = sign1;  // Same sign, no need for guard
        mant1 = {!nil1, mant1[9:0]};  // Prepend hidden bit to mantissa
        mant2 = {!nil2, mant2[9:0]};
        
        // Adjust mantissas if exponents are different
        if (exp1 > exp2) begin
          exp3 = exp1;  // Larger exponent always wins
          for (int j = 0; j < (exp1 - exp2); j++) begin
            mant2 = mant2 >> 1;  // Right shift mant2
          end
        end else if (exp2 > exp1) begin
          exp3 = exp2;
          for (int j = 0; j < (exp2 - exp1); j++) begin
            mant1 = mant1 >> 1;  // Right shift mant1
          end
        end
        
        // Perform addition of mantissas
        mant3 = mant1 + mant2;
        
        // Overflow check
        if (mant3[11]) begin
          exp3++;  // Increment exponent
          mant3 = mant3 >> 1;  // Right shift mantissa
        end
        
        // Round-induced overflow check
        if (mant3[11]) begin
          mant3 = mant3 >> 1;
          exp3++;
        end
        
        nil3 = !exp3;  // Zero exponent trap for the sum
      end : netadd

      // Store results back into data memory
      data_mem1.mem_core[132][7] = sign3;
      data_mem1.mem_core[132][6:2] = exp3;
      {data_mem1.mem_core[132][1:0], data_mem1.mem_core[133]} = mant3;
      
      done = 1'b1;  // Set done flag
    end
  end : main

endmodule
