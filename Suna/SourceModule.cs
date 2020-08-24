using System;
using System.Collections.Generic;
using System.IO;

using Suna.Blocks;
using Suna.Regions;
using Suna.Tokens;

namespace Suna
{
   /// <summary>
   /// A source module.  Currently this is a single file, though it remains to be
   /// seen how I decide to structure things with multi-file builds.
   /// </summary>
   public class SourceModule
   {
      #region Types / Constants

      private enum Error
      {
         ExpectedBlockStart
      }

      #endregion

      #region Private Fields

      private Tokenizer tokenizer = new Tokenizer();
      private RegionizedModule regions;
      private BlockifiedModule blocks;

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class SourceModule
      /// </summary>
      /// <param name="text"></param>
      public SourceModule(string text)
      {
         this.ModuleText = text;
      }

      /// <summary>
      /// Initializes a new instance of class SourceModule
      /// </summary>
      /// <param name="stream"></param>
      public SourceModule(Stream stream)
         : this(
              new Func<string>(() => { 
                 using (var reader = new StreamReader(stream))
                    return reader.ReadToEnd();
              }) ()
              )
      {
      }

      #endregion

      #region Properties

      /// <summary>
      /// Gets the module's text
      /// </summary>
      public string ModuleText
      {
         get;
         private set;
      }

      /// <summary>
      /// Gets the module's regions
      /// </summary>
      public RegionizedModule Regions
      {
         get
         {
            // create if we haven't yet
            if (regions == null)
               regions = new RegionizedModule(this);
            return regions;
         }
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Gets the blocks
      /// </summary>
      public BlockifiedModule GetBlocks()
      {
         if (blocks == null)
         {
            // tokenize
            foreach (var sunaRegion in Regions.SunaRegions)
               tokenizer.Tokenize(sunaRegion);
            var tokenizedModule = tokenizer.TokenizedModule;

            // blockify
            blocks = new BlockifiedModule();
            List<Token> currentBlock = new List<Token>();
            foreach (var token in tokenizedModule)
            {
               // see if this is the start of a new block
               if (token.IsBlockStart)
               {
                  // add the current block
                  if (currentBlock.Count != 0)
                     blocks.Add(currentBlock[0].CreateBlock(currentBlock.ToArray()));

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
               blocks.Add(currentBlock[0].CreateBlock(currentBlock.ToArray()));
         }

         return blocks;
      }

      #endregion

      #region Internal Methods

      internal TextReader OpenReader()
      {
         return new StringReader(ModuleText);
      }

      #endregion
   }
}
