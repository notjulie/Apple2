using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.Tokens;

namespace Suna
{
   /// <summary>
   /// Compiler class
   /// </summary>
   public class Compiler
   {
      #region Types / Constants

      private enum Error
      {
         ExpectedBlockStart
      }

      #endregion

      #region Public Methods

      public void Compile(SourceModule sourceModule)
      {
         // tokenize
         Tokenizer tokenizer = new Tokenizer();
         var tokenizedModule = tokenizer.Tokenize(sourceModule);

         // blockify
         var blockifiedModule = new BlockifiedModule();
         List<Token> currentBlock = new List<Token>();
         foreach (var token in tokenizedModule)
         {
            // see if this is the start of a new block
            if (token.IsBlockStart)
            {
               // add the current block
               if (currentBlock.Count != 0)
                  blockifiedModule.Add(currentBlock[0].CreateBlock(currentBlock.ToArray()));

               // start a new block
               currentBlock.Clear();
               currentBlock.Add(token);
            }
            else
            {
               // this can't be the first in a block
               if (currentBlock.Count == 0)
                  throw new CompileException(Error.ExpectedBlockStart);
               currentBlock.Add(token);
            }
         }
         if (currentBlock.Count != 0)
            blockifiedModule.Add(currentBlock[0].CreateBlock(currentBlock.ToArray()));

         // then something
         throw new NotImplementedException();
      }

      #endregion
   }
}
