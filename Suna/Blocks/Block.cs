﻿using Suna.Tokens;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   /// <summary>
   /// Represents a block of code in a module; blocks are standalone sections that
   /// can be compiled/interpreted as a unit, such as a function
   /// </summary>
   public class Block
   {
      #region Private Fields

      private Token[] tokens;

      #endregion

      #region Contructor

      /// <summary>
      /// Initializes a new instance of class Block
      /// </summary>
      /// <param name="tokens"></param>
      public Block(Token[] tokens)
      {
         this.tokens = tokens;
      }

      #endregion

      #region Properties

      /// <summary>
      /// Gets the blocks tokens
      /// </summary>
      public IEnumerable<Token> Tokens
      {
         get
         {
            return tokens;
         }
      }

      #endregion
   }
}
