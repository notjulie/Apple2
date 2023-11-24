// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Memcpy2D.h"


namespace c6502 {


   /// <summary>
   /// Sets Memcpy2D to its initial state; call this before configuration functions
   /// </summary>
   __attribute((noinline)) void Memcpy2D::Init()
   {
      asm volatile (
         "PHA\n"

         // set our next row functions to nothing
         "LDA\t#mos16lo(MCP2_RTS)\n"
         "STA\tMCP2_getDestAddress+1\n"
         "STA\tMCP2_getSourceAddress+1\n"

         "LDA\t#mos16hi(MCP2_RTS)\n"
         "STA\tMCP2_getDestAddress+2\n"
         "STA\tMCP2_getSourceAddress+2\n"

         // clear the terminate flag
         "LDA\t#0\n"
         "STA\tMCP2_terminate\n"

         "PLA\n"
      : // outputs
      : // inputs
      : // clobbers
      );
   }


   /// <summary>
   /// Terminates a 2D memcpy; this can be called by a row function to terminate
   /// a copy before the last row
   /// </summary>
   void Memcpy2D::Terminate()
   {
      asm volatile (
         "PHA\n"

         // set the terminate flag
         "LDA\t#1\n"
         "STA\tMCP2_terminate\n"

         "PLA\n"
      : // outputs
      : // inputs
      : // clobbers
      );
   }


   /// <summary>
   /// Performs 2D mem-copy... prior to calling, the source and destination
   /// rows must be set using the set-pointer or set-function methods
   /// </summary>
   __attribute((noinline)) void Memcpy2D::Copy(uint8_t rows, uint8_t rowLength)
   {
      asm volatile (
         // store our params
         "STA\tMCP2_cmpRowLength + 1\n"
         "TXA\n"
         "BEQ\tMCP2_RTS\n"
         "STX\tMCP2_rows\n"

         // check our terminate flag
         "LDA\tMCP2_terminate\n"
         "BNE\t99f\n"

         // copy a row
      "copyRow:\n"
         "LDY\t#0\n"
         "JMP\tMCP2_cmpRowLength\n"

      "MCP2_rows:\n"
         "NOP\n"
      "MCP2_terminate:\n"
         "NOP\n"
      "MCP2_RTS:\n"
         "RTS\n"

      "MCP2_loadSource:\n"
         "LDA\t$1111,y\n"
      "MCP2_storeDest:\n"
         "STA\t$1111,y\n"
         "INY\n"
      "MCP2_cmpRowLength:\n"
         "CPY\t#0\n"
         "BNE\tMCP2_loadSource\n"

         // next row
         "DEC\tMCP2_rows\n"
         "BEQ\t99f\n"

         // we have a function we call to update the source address
      "MCP2_getSourceAddress:\n"
         "JSR\t$1111\n"

         // we have a function we call to update the dest address
      "MCP2_getDestAddress:\n"
         "JSR\t$1111\n"

         // check our terminate flag
         "LDA\tMCP2_terminate\n"
         "BEQ\tcopyRow\n"

      "99:\n"

      : // outputs
      : "x"(rows), "a"(rowLength) // inputs
      : "y" // clobbers
      );
   }


   /// <summary>
   /// sets the function that sets the destination row pointer for each row
   /// </summary>
   void Memcpy2D::SetDestFunction(Function *function)
   {
      c6502::Int16 f;
      f.i16 = (int16_t)function;

      asm volatile (
         "STX\tMCP2_getDestAddress+1\n"
         "STY\tMCP2_getDestAddress+2\n"
      : // outputs
      : "x"(f.lo), "y"(f.hi) // inputs
      : // clobbers
      );
   }


   /// <summary>
   /// sets the function that sets the source row pointer for each row
   /// </summary>
   void Memcpy2D::SetSourceFunction(Function *function)
   {
      c6502::Int16 f;
      f.i16 = (int16_t)function;

      asm volatile (
         "STX\tMCP2_getSourceAddress+1\n"
         "STY\tMCP2_getSourceAddress+2\n"
      : // outputs
      : "x"(f.lo), "y"(f.hi) // inputs
      : // clobbers
      );
   }


   /// <summary>
   /// sets the source row pointer
   /// </summary>
   void Memcpy2D::SetSourcePointer(Int16 p)
   {
      asm volatile (
         "STX\tMCP2_loadSource+1\n"
         "STY\tMCP2_loadSource+2\n"
      : // outputs
      : "x"(p.lo), "y"(p.hi) // inputs
      : // clobbers
      );
   }


   /// <summary>
   /// sets the destination row pointer
   /// </summary>
   void Memcpy2D::SetDestPointer(Int16 p)
   {
      asm volatile (
         "STX\tMCP2_storeDest+1\n"
         "STY\tMCP2_storeDest+2\n"
      : // outputs
      : "x"(p.lo), "y"(p.hi) // inputs
      : // clobbers
      );
   }


   /// <summary>
   /// Increments the destination pointer by the width of one row; a useful
   /// function to pass to SetDestFunction
   /// </summary>
   void Memcpy2D::IncrementDest()
   {
      asm volatile (
         "LDA\tMCP2_cmpRowLength + 1\n"
         "CLC\n"
         "ADC\tMCP2_storeDest + 1\n"
         "STA\tMCP2_storeDest + 1\n"
         "BCC\t1f\n"
         "INC\tMCP2_storeDest+2\n"
      "1:\n"
      : // outputs
      : // inputs
      : "a" // clobbers
      );
   }


   /// <summary>
   /// Increments the source pointer by the width of one row; a useful
   /// function to pass to SetSourceFunction
   /// </summary>
   void Memcpy2D::IncrementSource()
   {
      asm volatile (
         "LDA\tMCP2_cmpRowLength + 1\n"
         "CLC\n"
         "ADC\tMCP2_loadSource + 1\n"
         "STA\tMCP2_loadSource + 1\n"
         "BCC\t1f\n"
         "INC\tMCP2_loadSource+2\n"
      "1:\n"
      : // outputs
      : // inputs
      : "a" // clobbers
      );
   }

}


