echo %2%
cd %2%

%1%\bin\llvm-objdump -d --print-imm-hex --source sh2cb.a2.elf > sh2cb.a2.disasm
