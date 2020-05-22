using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.Link;
using Suna.Regions;
using Suna.Tokens;

namespace Suna
{
   /// <summary>
   /// Compiler class
   /// </summary>
   public sealed class SunaCompiler
   {
      #region Types / Constants

      private enum Error
      {
         ExpectedBlockStart
      }

      #endregion

      #region Private Fields

      private Tokenizer tokenizer = new Tokenizer();

      #endregion

      #region Public Methods

      /// <summary>
      /// Compiles the given module
      /// </summary>
      /// <param name="sourceModule"></param>
      public void Compile(SourceModule sourceModule)
      {
         Contract.Requires(sourceModule != null);

         // regionize
         var regionizedModule = new RegionizedModule(sourceModule);

         // parse the JavaScript sections
         Jint.Engine jsEngine = new Jint.Engine();
         foreach (var jsRegion in regionizedModule.JavascriptRegions)
            jsEngine.Execute(jsRegion.ToString());

         // tokenize
         foreach (var sunaRegion in regionizedModule.SunaRegions)
            tokenizer.Tokenize(sunaRegion);
         var tokenizedModule = tokenizer.TokenizedModule;

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

         // package up our inputs to the linker
         LinkContext linkContext = new LinkContext(blockifiedModule, jsEngine);

         // link
         Linker linker = new Linker();
         LinkedModule linkedModule = linker.Link(linkContext);

         // then something
         throw new NotImplementedException();
      }

      #endregion
   }
}
